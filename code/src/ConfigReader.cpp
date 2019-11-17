/* This file is part of C++ Config Framework.
 *
 * C++ Config Framework is free software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * C++ Config Framework is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with C++ Config
 * Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * \file
 *
 * Contains a class for reading configuration files
 */

// Own header
#include <CppConfigFramework/ConfigReader.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigDerivedObjectNode.hpp>
#include <CppConfigFramework/ConfigNodeReference.hpp>
#include <CppConfigFramework/ConfigObjectNode.hpp>
#include <CppConfigFramework/ConfigValueNode.hpp>

// Qt includes
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QList>
#include <QtCore/QRegularExpression>
#include <QtCore/QStringBuilder>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

ConfigReader::ConfigReader()
    : m_referenceResolutionMaxCycles(100u)
{
}

// -------------------------------------------------------------------------------------------------

uint32_t ConfigReader::referenceResolutionMaxCycles() const
{
    return m_referenceResolutionMaxCycles;
}

// -------------------------------------------------------------------------------------------------

void ConfigReader::setReferenceResolutionMaxCycles(const uint32_t referenceResolutionMaxCycles)
{
    m_referenceResolutionMaxCycles = referenceResolutionMaxCycles;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReader::read(
        const QString &filePath,
        const QDir &workingDir,
        const ConfigNodePath &sourceNodePath,
        const ConfigNodePath &destinationNodePath,
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        QString *error) const
{
    // Validate source node path
    if ((!sourceNodePath.isAbsolute()) ||
        (!sourceNodePath.isValid()))
    {
        if (error != nullptr)
        {
            *error = "Invalid source node path: " % sourceNodePath.path();
        }
        return {};
    }

    // Validate destination node
    if ((!destinationNodePath.isAbsolute()) ||
        (!destinationNodePath.isValid()))
    {
        if (error != nullptr)
        {
            *error = "Invalid destination node path: " % destinationNodePath.path();
        }
        return {};
    }

    // Prepare absolute path to the file using the working path if needed
    QString absoluteFilePath;

    if (QDir::isAbsolutePath(filePath))
    {
        absoluteFilePath = QDir::cleanPath(filePath);
    }
    else
    {
        absoluteFilePath = QDir::cleanPath(workingDir.absoluteFilePath(filePath));
    }

    // Check external configs
    for (const auto *externalConfig : externalConfigs)
    {
        if (externalConfig == nullptr)
        {
            *error = "Invalid external configs for file at path: " % absoluteFilePath;
            return {};
        }
    }

    // Open file
    if (!QFile::exists(absoluteFilePath))
    {
        if (error != nullptr)
        {
            *error = "File at path was not found: " % absoluteFilePath;
        }
        return {};
    }

    QFile file(absoluteFilePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        if (error != nullptr)
        {
            *error = "Failed to open file at path: " % absoluteFilePath;
        }
        return {};
    }

    // Read the contents (JSON format)
    QJsonParseError jsonParseError {};
    const QByteArray fileContents = file.readAll();
    const auto doc = QJsonDocument::fromJson(fileContents, &jsonParseError);

    if (jsonParseError.error != QJsonParseError::NoError)
    {
        constexpr int contextMaxLength = 20;
        const int contextBeforeIndex = std::max(0, jsonParseError.offset - contextMaxLength);
        const int contextBeforeLength = std::min(jsonParseError.offset, contextMaxLength);

        if (error != nullptr)
        {
            *error = QString(
                         "Failed to parse the file contents:"
                         "\n    file path: %1"
                         "\n    offset: %2"
                         "\n    error: [%3]"
                         "\n    context before error: [%4]"
                         "\n    context at error: [%5]")
                     .arg(absoluteFilePath,
                          QString::number(jsonParseError.offset),
                          jsonParseError.errorString(),
                          QString::fromUtf8(fileContents.mid(contextBeforeIndex,
                                                             contextBeforeLength)),
                          QString::fromUtf8(fileContents.mid(jsonParseError.offset,
                                                             contextMaxLength)));
        }
        return {};
    }

    if (!doc.isObject())
    {
        if (error != nullptr)
        {
            *error = "Config file does not contain a JSON object: " % absoluteFilePath;
        }
        return {};
    }

    const auto rootObject = doc.object();

    // TODO: add "environment variables" member?

    // Read 'includes' member
    auto completeConfig = readIncludesMember(rootObject,
                                             QFileInfo(absoluteFilePath).absoluteDir(),
                                             externalConfigs,
                                             error);

    if (!completeConfig)
    {
        if (error != nullptr)
        {
            *error = QString("Failed to read the 'includes' member:"
                             "\n    file path: %1"
                             "\n    inner error: [%2]").arg(absoluteFilePath, *error);
        }
        return {};
    }

    // Read 'config' member
    auto configMember = readConfigMember(rootObject, externalConfigs, *completeConfig, error);

    if (!configMember)
    {
        if (error != nullptr)
        {
            *error = QString("Failed to read the 'config' member:"
                             "\n    file path: %1"
                             "\n    inner error: [%2]").arg(absoluteFilePath, *error);
        }
        return {};
    }

    // Apply the overloads from 'config' member to the read configuration
    completeConfig->apply(*configMember);

    // Resolve references
    if (!resolveReferences(externalConfigs, completeConfig.get(), error))
    {
        if (error != nullptr)
        {
            *error = QString("Failed to resolve references:"
                             "\n    file path: %1"
                             "\n    inner error: [%2]").arg(absoluteFilePath, *error);
        }
        return {};
    }

    // Transform the configuration node based on source and destination node paths
    auto transformedConfig = transformConfig(std::move(completeConfig),
                                             sourceNodePath,
                                             destinationNodePath,
                                             error);

    if (!transformedConfig)
    {
        if (error != nullptr)
        {
            *error = QString("Failed to transform the config:"
                             "\n    file path: %1"
                             "\n    inner error: [%2]").arg(absoluteFilePath, *error);
        }
        return {};
    }

    return transformedConfig;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReader::readIncludesMember(
        const QJsonObject &rootObject,
        const QDir &workingDir,
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        QString *error) const
{
    // Check if the root object has any includes
    const auto includesValue = rootObject.value(QStringLiteral("includes"));

    if (includesValue.isNull() || includesValue.isUndefined())
    {
        // No includes
        return std::make_unique<ConfigObjectNode>();
    }

    if (!includesValue.isArray())
    {
        if (error != nullptr)
        {
            *error = "The 'includes' member in the root JSON Object is not a JSON array!";
        }
        return {};
    }

    // Iterate over all includes and extract their value
    auto includesConfig = std::make_unique<ConfigObjectNode>();
    const auto includesArray = includesValue.toArray();

    for (int i = 0; i < includesArray.size(); i++)
    {
        // Include item must be a JSON Object
        const auto includeValue = includesArray.at(i);

        if (!includeValue.isObject())
        {
            if (error != nullptr)
            {
                *error = QString("Include at index [%1] is not a JSON Object!").arg(i);
            }
            return {};
        }

        const auto includeObject = includeValue.toObject();

        // Extract configuration file type
        QString type = QStringLiteral("CppConfigFramework");
        const auto typeValue = includeObject.value(QStringLiteral("type"));

        if ((!typeValue.isNull()) && (!typeValue.isUndefined()))
        {
            if (!typeValue.isString())
            {
                if (error != nullptr)
                {
                    *error = QString("The 'type' member is not a string for include at index [%1]")
                             .arg(i);
                }
                return {};
            }

            // Extract the type
            type = typeValue.toString();
        }

        if (type != QStringLiteral("CppConfigFramework"))
        {
            if (error != nullptr)
            {
                *error = QString("Unsupported type [%1] for include at index [%2]")
                         .arg(type).arg(i);
            }
            return {};
        }

        // Extract file path
        const auto filePathValue = includeObject.value(QStringLiteral("file_path"));

        if (filePathValue.isUndefined())
        {
            if (error != nullptr)
            {
                *error = QString("The 'file_path' member is missing for include at index [%1]")
                         .arg(i);
            }
            return {};
        }

        if (!filePathValue.isString())
        {
            if (error != nullptr)
            {
                *error = QString("The 'file_path' member must be a string for include at index "
                                 "[%1]").arg(i);
            }
            return {};
        }

        const QString filePath = filePathValue.toString();

        // Extract source node
        auto sourceNodePath = ConfigNodePath::ROOT_PATH;
        const auto sourceNodeValue = includeObject.value(QStringLiteral("source_node"));

        if ((!sourceNodeValue.isNull()) && (!sourceNodeValue.isUndefined()))
        {
            if (!sourceNodeValue.isString())
            {
                if (error != nullptr)
                {
                    *error = QString("The 'source_node' member must be a string for include at "
                                     "index [%1]").arg(i);
                }
                return {};
            }

            sourceNodePath.setPath(sourceNodeValue.toString());

            if (sourceNodePath.isRelative() || (!sourceNodePath.isValid()))
            {
                if (error != nullptr)
                {
                    *error = QString("The 'source_node' member [%1] is not valid for include at "
                                     "index [%2]").arg(sourceNodePath.path()).arg(i);
                }
                return {};
            }
        }

        // Extract destination node
        auto destinationNodePath = ConfigNodePath::ROOT_PATH;
        const auto destinationNodeValue = includeObject.value(QStringLiteral("destination_node"));

        if ((!destinationNodeValue.isNull()) && (!destinationNodeValue.isUndefined()))
        {
            if (!destinationNodeValue.isString())
            {
                if (error != nullptr)
                {
                    *error = QString("The 'destination_node' member must be a string for include "
                                     "at index [%1]").arg(i);
                }
                return {};
            }

            destinationNodePath.setPath(destinationNodeValue.toString());

            if (destinationNodePath.isRelative() || (!destinationNodePath.isValid()))
            {
                if (error != nullptr)
                {
                    *error = QString("The 'destination_node' member [%1] is not valid for include "
                                     "at index [%2]").arg(destinationNodePath.path()).arg(i);
                }
                return {};
            }
        }

        // Extend the external configs with the includesConfig value if necessary
        auto extendedExternalConfigs = externalConfigs;

        if (includesConfig->count() > 0)
        {
            extendedExternalConfigs.push_back(includesConfig.get());
        }

        // Read config file
        // TODO: limit the includes depth to prevent an endless include loop?
        auto config = read(filePath,
                           workingDir,
                           sourceNodePath,
                           destinationNodePath,
                           extendedExternalConfigs,
                           error);

        if (!config)
        {
            if (error != nullptr)
            {
                *error = QString("Failed to read config for include:"
                                 "\n    index: %1"
                                 "\n    inner error: [%2]").arg(i).arg(*error);
            }
            return {};
        }

        // Apply the config file contents to the "includes" configuration node
        includesConfig->apply(*config);
    }

    return includesConfig;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReader::readConfigMember(
        const QJsonObject &rootObject,
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        const ConfigObjectNode &includesConfig,
        QString *error) const
{
    // The root object must contain the 'config' member (but it can be an empty object)
    const auto configValue = rootObject.value(QStringLiteral("config"));

    if (configValue.isNull())
    {
        // No configuration
        return std::make_unique<ConfigObjectNode>();
    }

    if (!configValue.isObject())
    {
        if (error != nullptr)
        {
            *error = "The 'config' member in the root JSON Object is not a JSON Object!";
        }
        return {};
    }

    // Read 'config' object
    auto config = readObjectNode(configValue.toObject(), ConfigNodePath::ROOT_PATH, error);

    if (!config)
    {
        if (error != nullptr)
        {
            *error = QString("Failed to read the 'config' member in the root JSON Object:"
                             "\n    error: [%1]").arg(*error);
        }
        return {};
    }

    // Extend the external configs with the includesConfig value if necessary
    auto extendedExternalConfigs = externalConfigs;

    if (includesConfig.count() > 0)
    {
        extendedExternalConfigs.push_back(&includesConfig);
    }

    // Resolve references
    if (!resolveReferences(extendedExternalConfigs, config.get(), error))
    {
        if (error != nullptr)
        {
            *error = QString("Failed to resolve references. Inner error: [%1]").arg(*error);
        }
        return {};
    }

    return config;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigValueNode> ConfigReader::readValueNode(const QJsonValue &jsonValue,
                                                             const ConfigNodePath &currentNodePath)
{
    return std::make_unique<ConfigValueNode>(jsonValue.toVariant());
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReader::readObjectNode(
        const QJsonObject &jsonObject,
        const ConfigNodePath &currentNodePath,
        QString *error)
{
    auto objectNode = std::make_unique<ConfigObjectNode>();

    for (auto it = jsonObject.begin(); it != jsonObject.end(); it++)
    {
        QString memberName = it.key();

        // Check for "decorators" in the member name (reference type or Value node)
        QChar decorator('\0');

        if (hasDecorator(memberName))
        {
            decorator = memberName.at(0);
            memberName = memberName.mid(1);
        }

        // Validate member name
        if (!ConfigNodePath::validateNodeName(memberName))
        {
            if (error != nullptr)
            {
                *error = QString("Invalid member name [%1] in path [%2]")
                         .arg(memberName, currentNodePath.path());
            }
            return {};
        }

        // Create a node based considering the decorator
        std::unique_ptr<ConfigNode> memberNode;
        const ConfigNodePath memberNodePath = currentNodePath.append(memberName);

        switch (decorator.toLatin1())
        {
            case '#':
            {
                // Explicit Value node (even if it is an Array or an Object JSON type)
                memberNode = readValueNode(it.value(), memberNodePath);
                Q_ASSERT(memberNode);
                break;
            }

            case '&':
            {
                // One of the reference types
                if (it.value().isString())
                {
                    memberNode = readNodeReferenceNode(it.value().toString(),
                                                       memberNodePath,
                                                       error);

                    if (!memberNode)
                    {
                        if (error != nullptr)
                        {
                            *error = QString("Failed to read the a NodeReference node member:"
                                             "\n    member node path: %1"
                                             "\n    error: [%2]")
                                     .arg(memberNodePath.path(), *error);
                        }
                        return {};
                    }
                }
                else if (it.value().isObject())
                {
                    memberNode = readDerivedObjectNode(it.value().toObject(),
                                                       memberNodePath,
                                                       error);

                    if (!memberNode)
                    {
                        if (error != nullptr)
                        {
                            *error = QString("Failed to read the a NodeReference node member:"
                                             "\n    member node path: %1"
                                             "\n    error: [%2]")
                                     .arg(memberNodePath.path(), *error);
                        }
                        return {};
                    }
                }
                else
                {
                    if (error != nullptr)
                    {
                        *error = "Unsupported reference type at path: " % memberNodePath.path();
                    }
                    return {};
                }
                break;
            }

            default:
            {
                // No decorators, just an ordinary node
                if (it.value().isObject())
                {
                    memberNode = readObjectNode(it.value().toObject(), memberNodePath, error);

                    if (!memberNode)
                    {
                        if (error != nullptr)
                        {
                            *error = QString("Failed to read the an ordinary Object node member:"
                                             "\n    member node path: %1"
                                             "\n    error: [%2]")
                                     .arg(memberNodePath.path(), *error);
                        }
                        return {};
                    }
                }
                else
                {
                    memberNode = readValueNode(it.value(), memberNodePath);
                    Q_ASSERT(memberNode);
                }
                break;
            }
        }

        // Add member to the object
        Q_ASSERT(memberNode);
        objectNode->setMember(memberName, std::move(memberNode));
    }

    return objectNode;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNodeReference> ConfigReader::readNodeReferenceNode(
        const QString &reference,
        const ConfigNodePath &currentNodePath,
        QString *error)
{
    const ConfigNodePath referencePath(reference);

    if (!referencePath.toAbsolute(currentNodePath).isValid())
    {
        if (error != nullptr)
        {
            *error = QString("Invalid node reference [%1] with current path [%2]")
                     .arg(reference, currentNodePath.path());
        }
        return {};
    }

    return std::make_unique<ConfigNodeReference>(referencePath);
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigDerivedObjectNode> ConfigReader::readDerivedObjectNode(
        const QJsonObject &jsonObject,
        const ConfigNodePath &currentNodePath,
        QString *error)
{
    // Extract bases
    const auto baseValue = jsonObject.value(QStringLiteral("base"));

    if (baseValue.isUndefined())
    {
        if (error != nullptr)
        {
            *error = "A derived object doesn't have the 'base' member at path: " %
                     currentNodePath.path();
        }
        return {};
    }

    QList<ConfigNodePath> bases;

    if (baseValue.isString())
    {
        // Single base
        bases.append(ConfigNodePath(baseValue.toString()));
    }
    else if (baseValue.isArray())
    {
        for (const auto &item : baseValue.toArray())
        {
            if (!item.isString())
            {
                if (error != nullptr)
                {
                    *error = "Unsupported JSON type for an item in the 'base' member at path: " %
                             currentNodePath.path();
                }
                return {};
            }

            bases.append(ConfigNodePath(item.toString()));
        }

        if (bases.isEmpty())
        {
            if (error != nullptr)
            {
                *error = "The 'base' member is empty at path: " % currentNodePath.path();
            }
            return {};
        }
    }
    else
    {
        if (error != nullptr)
        {
            *error = "Unsupported JSON type for an item in the 'base' member at path: " %
                     currentNodePath.path();
        }
        return {};
    }

    for (const auto &item : bases)
    {
        if (!item.toAbsolute(currentNodePath).isValid())
        {
            if (error != nullptr)
            {
                *error = QString("Invalid node path in base item at path:"
                                 "\n    base item's node path: %1"
                                 "\n    node path: %2")
                         .arg(item.path(), currentNodePath.path());
            }
            return {};
        }
    }

    // Extract config
    auto config = std::make_unique<ConfigObjectNode>();
    const auto configValue = jsonObject.value(QStringLiteral("config"));

    if (!configValue.isUndefined())
    {
        if (configValue.isObject())
        {
            // Read overrides for the object derived from bases
            config = readObjectNode(configValue.toObject(), currentNodePath, error);

            if (!config)
            {
                if (error != nullptr)
                {
                    *error = QString("Failed to read the overrides for the object derived from "
                                     "bases at path:"
                                     "\n    node path: %1"
                                     "\n    error: [%2]")
                             .arg(currentNodePath.path(), *error);
                }
                return {};
            }
        }
        else if (configValue.isNull())
        {
            // No overrides for the object derived from bases
        }
        else
        {
            if (error != nullptr)
            {
                *error = "Unsupported JSON type for the 'config' member at path: " %
                         currentNodePath.path();
            }
            return {};
        }
    }

    // Create derived object node
    return std::make_unique<ConfigDerivedObjectNode>(bases, *config);
}

// -------------------------------------------------------------------------------------------------

bool ConfigReader::hasDecorator(const QString &memberName)
{
    static QRegularExpression regex("^[&#]");

    return regex.match(memberName).hasMatch();
}

// -------------------------------------------------------------------------------------------------

bool ConfigReader::isFullyResolved(const ConfigNode &node)
{
    switch (node.type())
    {
        case ConfigNode::Type::Value:
        {
            return true;
        }

        case ConfigNode::Type::Object:
        {
            const auto &objectNode = node.toObject();

            for (const auto &name : objectNode.names())
            {
                if (!isFullyResolved(*objectNode.member(name)))
                {
                    return false;
                }
            }

            return true;
        }

        default:
        {
            break;
        }
    }

    return false;
}

// -------------------------------------------------------------------------------------------------

QStringList ConfigReader::unresolvedReferences(const ConfigObjectNode &node)
{
    QStringList references;

    // Iterate over all members and add all nodes of a reference type to the list
    for (const QString &name : node.names())
    {
        const auto *member = node.member(name);

        if (member->isNodeReference() || member->isDerivedObject())
        {
            references.append(member->nodePath().path());
        }
        else if (member->isObject())
        {
            references.append(unresolvedReferences(member->toObject()));
        }
        else
        {
            // No unresolved references
        }
    }

    return references;
}

// -------------------------------------------------------------------------------------------------

bool ConfigReader::resolveReferences(const std::vector<const ConfigObjectNode *> &externalConfigs,
                                     ConfigObjectNode *config,
                                     QString *error) const
{
    auto result = ReferenceResolutionResult::Unchanged;
    uint32_t resolutionCycle;

    for (resolutionCycle = 0;
         (resolutionCycle < m_referenceResolutionMaxCycles) &&
         (result != ReferenceResolutionResult::Resolved);
         resolutionCycle++)
    {
        // Try to resolve references without external configuration nodes
        auto newResult = resolveObjectReferences({}, config, error);

        switch (newResult)
        {
            case ReferenceResolutionResult::Resolved:
            case ReferenceResolutionResult::PartiallyResolved:
            {
                result = newResult;
                break;
            }

            case ReferenceResolutionResult::Unchanged:
            {
                // Check if external configuration nodes are provided
                if (externalConfigs.empty())
                {
                    result = ReferenceResolutionResult::Unchanged;
                    break;
                }

                // External configuration nodes are provided, try to resolve references with them
                newResult = resolveObjectReferences(externalConfigs, config, error);

                switch (newResult)
                {
                    case ReferenceResolutionResult::Resolved:
                    case ReferenceResolutionResult::PartiallyResolved:
                    {
                        result = newResult;
                        break;
                    }

                    case ReferenceResolutionResult::Unchanged:
                    {
                        // Still unchanged
                        if (error != nullptr)
                        {
                            *error = QString("No references were resolved in the last cycle even "
                                             "after using the external configuration nodes:"
                                             "\n    cycle no.: %1"
                                             "\n    unresolved references: [%2]")
                                     .arg(resolutionCycle)
                                     .arg(unresolvedReferences(*config).join("; "));
                        }
                        return false;
                    }

                    case ReferenceResolutionResult::Error:
                    {
                        if (error != nullptr)
                        {
                            *error = QString("Failed to resolve references when using the external "
                                             "configuration nodes:"
                                             "\n    inner error: [%1]"
                                             "\n    unresolved references: [%2]")
                                     .arg(*error, unresolvedReferences(*config).join("; "));
                        }
                        return false;
                    }
                }
                break;
            }

            case ReferenceResolutionResult::Error:
            {
                if (error != nullptr)
                {
                    *error = QString("Failed to resolve references:"
                                     "\n    inner error: [%1]"
                                     "\n    unresolved references: [%2]")
                             .arg(*error, unresolvedReferences(*config).join("; "));
                }
                return false;
            }
        }
    }

    if (result != ReferenceResolutionResult::Resolved)
    {
        if (error != nullptr)
        {
            *error = QString("Failed to fully resolve references:"
                             "\n    cycle no.: %1"
                             "\n    unresolved references: [%2]")
                     .arg(resolutionCycle)
                     .arg(unresolvedReferences(*config).join("; "));
        }
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

ConfigReader::ReferenceResolutionResult ConfigReader::resolveObjectReferences(
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        ConfigObjectNode *node,
        QString *error)
{
    // Iterate over all members and try to resolve their references
    auto result = ReferenceResolutionResult::Unchanged;

    for (const QString &name : node->names())
    {
        // Try to resolve the member's references
        auto *member = node->member(name);

        switch (member->type())
        {
            case ConfigNode::Type::Value:
            {
                // Not a reference type, leave the result as is
                break;
            }

            case ConfigNode::Type::Object:
            {
                // Try to resolve the member's (Object node) references
                auto newResult = resolveObjectReferences(externalConfigs,
                                                         &member->toObject(),
                                                         error);

                result = updateObjectResolutionResult(result, newResult);
                break;
            }

            case ConfigNode::Type::NodeReference:
            {
                // Try to resolve the member's (NodeReference node) reference
                auto newResult = resolveNodeReference(externalConfigs,
                                                      &member->toNodeReference(),
                                                      error);

                result = updateObjectResolutionResult(result, newResult);
                break;
            }

            case ConfigNode::Type::DerivedObject:
            {
                // Try to resolve the member's (DerivedObject node) references
                auto newResult = resolveDerivedObjectReferences(externalConfigs,
                                                                &member->toDerivedObject(),
                                                                error);

                result = updateObjectResolutionResult(result, newResult);
                break;
            }
        }

        // Stop on error
        if (result == ReferenceResolutionResult::Error)
        {
            return ReferenceResolutionResult::Error;
        }
    }

    // Check if the object is fully resolved
    if (isFullyResolved(*node))
    {
        return ReferenceResolutionResult::Resolved;
    }

    return result;
}

// -------------------------------------------------------------------------------------------------

ConfigReader::ReferenceResolutionResult ConfigReader::updateObjectResolutionResult(
        const ConfigReader::ReferenceResolutionResult currentResult,
        const ConfigReader::ReferenceResolutionResult newResult)
{
    auto result = currentResult;

    switch (newResult)
    {
        case ReferenceResolutionResult::Resolved:
        {
            if (result == ReferenceResolutionResult::Unchanged)
            {
                result = ReferenceResolutionResult::PartiallyResolved;
            }
            break;
        }

        case ReferenceResolutionResult::Unchanged:
        {
            if (result == ReferenceResolutionResult::Resolved)
            {
                result = ReferenceResolutionResult::PartiallyResolved;
            }
            break;
        }

        case ReferenceResolutionResult::PartiallyResolved:
        {
            if ((result == ReferenceResolutionResult::Resolved) ||
                (result == ReferenceResolutionResult::Unchanged))
            {
                result = ReferenceResolutionResult::PartiallyResolved;
            }
            break;
        }

        case ReferenceResolutionResult::Error:
        {
            result = ReferenceResolutionResult::Error;
        }
    }

    return result;
}

// -------------------------------------------------------------------------------------------------

ConfigReader::ReferenceResolutionResult ConfigReader::resolveNodeReference(
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        ConfigNodeReference *node,
        QString *error)
{
    // Try to get the referenced node
    auto *parentNode = node->parent();
    const auto *referencedNode = findReferencedConfigNode(node->reference(),
                                                          *parentNode,
                                                          externalConfigs);

    if (referencedNode == nullptr)
    {
        return ReferenceResolutionResult::Unchanged;
    }

    auto result = (isFullyResolved(*referencedNode) ? ReferenceResolutionResult::Resolved
                                                    : ReferenceResolutionResult::PartiallyResolved);

    // Replace the current node with the referenced node
    if (!parentNode->setMember(parentNode->name(*node), *referencedNode))
    {
        if (error != nullptr)
        {
            *error = QString("Failed to store the resolved NodeReference node [%1] to the parent "
                             "object at node path [%2]")
                     .arg(node->reference().path(), parentNode->nodePath().path());
        }
        return ReferenceResolutionResult::Error;
    }

    return result;
}

// -------------------------------------------------------------------------------------------------

ConfigReader::ReferenceResolutionResult ConfigReader::resolveDerivedObjectReferences(
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        ConfigDerivedObjectNode *node,
        QString *error)
{
    // Derive the config node from the all of the base nodes
    auto *parentNode = node->parent();
    ConfigObjectNode derivedObjectNode(parentNode);

    for (const auto &baseNodePath : node->bases())
    {
        // Try to find the base node
        const auto *baseNode = findReferencedConfigNode(baseNodePath, *parentNode, externalConfigs);

        if (baseNode == nullptr)
        {
            return ReferenceResolutionResult::Unchanged;
        }

        // Check if the base node is fully resolved
        if (!isFullyResolved(*baseNode))
        {
            // Base node is not resolved (yet)
            return ReferenceResolutionResult::Unchanged;
        }

        // Apply the base to the derived object node
        if (!baseNode->isObject())
        {
            if (error != nullptr)
            {
                *error = QString("Base node [%1] in a DerivedObject node [%2] is referencing a "
                                 "node that is not an Object node!")
                         .arg(baseNodePath.path(), node->nodePath().path());
            }
            return ReferenceResolutionResult::Error;
        }

        derivedObjectNode.apply(baseNode->toObject());
    }

    // Apply overrides to the derived object
    if (node->config().count() > 0)
    {
        derivedObjectNode.apply(node->config());
    }

    auto result = (isFullyResolved(derivedObjectNode)
                   ? ReferenceResolutionResult::Resolved
                   : ReferenceResolutionResult::PartiallyResolved);


    // Replace the current node with the referenced node
    if (!parentNode->setMember(parentNode->name(*node), derivedObjectNode))
    {
        if (error != nullptr)
        {
            *error = QString("Failed to store the resolved DerivedObject node [%1] to the parent "
                             "object at node path [%2]")
                     .arg(node->nodePath().path(), parentNode->nodePath().path());
        }
        return ReferenceResolutionResult::Error;
    }

    return result;
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigReader::findReferencedConfigNode(
        const ConfigNodePath &referenceNodePath,
        const ConfigObjectNode &parentNode,
        const std::vector<const ConfigObjectNode *> &externalConfigs)
{
    const auto *referencedNode = parentNode.nodeAtPath(referenceNodePath);

    if (referencedNode == nullptr)
    {
        // Unable to find the node reference, try to find it in one of the external configuration
        // nodes (use last found node)
        for (const auto *externalConfig : externalConfigs)
        {
            if (referenceNodePath.isAbsolute())
            {
                const auto *foundNode = externalConfig->nodeAtPath(referenceNodePath);

                if (foundNode != nullptr)
                {
                    referencedNode = foundNode;
                }
            }
            else
            {
                // First try to find the equivalent parent node from the external config and then
                // try to get the node using the relative path
                const auto *externalConfigParent =
                        externalConfig->nodeAtPath(parentNode.nodePath());

                if (externalConfigParent != nullptr)
                {
                    const auto *foundNode = externalConfigParent->nodeAtPath(referenceNodePath);

                    if (foundNode != nullptr)
                    {
                        referencedNode = foundNode;
                    }
                }
            }
        }
    }

    return referencedNode;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReader::transformConfig(
        std::unique_ptr<ConfigObjectNode> &&config,
        const ConfigNodePath &sourceNodePath,
        const ConfigNodePath &destinationNodePath,
        QString *error)
{
    Q_ASSERT(config);
    Q_ASSERT(sourceNodePath.isAbsolute());
    Q_ASSERT(destinationNodePath.isAbsolute());

    // Check if transformation is needed
    if (sourceNodePath.isRoot() && destinationNodePath.isRoot())
    {
        // Transformation is not needed, just return the original configuration node
        return std::move(config);
    }

    // Take the source node
    std::unique_ptr<ConfigNode> sourceConfig;

    if (sourceNodePath.isRoot())
    {
        sourceConfig = std::move(config);
    }
    else
    {
        const auto *node = config->nodeAtPath(sourceNodePath);

        if (node == nullptr)
        {
            if (error != nullptr)
            {
                *error = "Failed to get the source config node at node path: " %
                         sourceNodePath.path();
            }
            return {};
        }

        sourceConfig = node->clone();
    }

    // For "root" destination just return the source node
    if (destinationNodePath.isRoot())
    {
        if (!sourceConfig->isObject())
        {
            if (error != nullptr)
            {
                *error = "Source config node at node path is not an Object: " %
                         sourceNodePath.path();
            }
            return {};
        }

        return std::make_unique<ConfigObjectNode>(std::move(sourceConfig->toObject()));
    }

    // Create a new config node from the destination node path and set the source config node to it
    auto transformedConfig = std::make_unique<ConfigObjectNode>();

    const QStringList nodeNames = destinationNodePath.nodeNames();
    ConfigObjectNode *currentNode = transformedConfig.get();

    for (int i = 0; i < nodeNames.size(); i++)
    {
        // Create the next node in the node path
        const QString nodeName = nodeNames.at(i);

        if (i == (nodeNames.size() - 1))
        {
            // For the last element just store the source config node
            currentNode->setMember(nodeName, std::move(sourceConfig));
        }
        else
        {
            // For all intermediate node create an empty Object config node
            currentNode->setMember(nodeName, std::make_unique<ConfigObjectNode>());

            auto *member = currentNode->member(nodeName);
            Q_ASSERT(member != nullptr);
            Q_ASSERT(member->isObject());

            currentNode = &(member->toObject());
        }
    }

    return transformedConfig;
}

} // namespace CppConfigFramework
