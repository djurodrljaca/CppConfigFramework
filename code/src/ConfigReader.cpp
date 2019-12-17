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
#include <CppConfigFramework/ConfigReaderFactory.hpp>
#include <CppConfigFramework/ConfigValueNode.hpp>

// Qt includes
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QRegularExpression>
#include <QtCore/QStringBuilder>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

std::unique_ptr<ConfigObjectNode> ConfigReader::read(
        const QString &filePath,
        const QDir &workingDir,
        const ConfigNodePath &sourceNodePath,
        const ConfigNodePath &destinationNodePath,
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        EnvironmentVariables *environmentVariables,
        QString *error) const
{
    // Make sure that file path is not empty
    if (filePath.isEmpty())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("File path is empty!");
        }
        return {};
    }

    // Validate source node path
    if ((!sourceNodePath.isAbsolute()) ||
        (!sourceNodePath.isValid()))
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Invalid source node path: ") % sourceNodePath.path();
        }
        return {};
    }

    // Validate destination node
    if ((!destinationNodePath.isAbsolute()) ||
        (!destinationNodePath.isValid()))
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Invalid destination node path: ") % destinationNodePath.path();
        }
        return {};
    }

    // Check if environment variables were provided
    if (environmentVariables == nullptr)
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Environment variables are not provided!");
        }
        return {};
    }

    // Expand references to environment variables in the file path
    const QString expandedFilePath = environmentVariables->expandText(filePath);

    if (expandedFilePath.isEmpty())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Failed to expand file path: ") % filePath;
        }
        return {};
    }

    // Prepare absolute path to the file using the working path if needed
    QString absoluteFilePath;

    if (QDir::isAbsolutePath(expandedFilePath))
    {
        absoluteFilePath = QDir::cleanPath(expandedFilePath);
    }
    else
    {
        absoluteFilePath = QDir::cleanPath(workingDir.absoluteFilePath(expandedFilePath));
    }

    // Check external configs
    for (const auto *externalConfig : externalConfigs)
    {
        if (externalConfig == nullptr)
        {
            *error = QStringLiteral("Invalid external configs for file at path: ") %
                     absoluteFilePath;
            return {};
        }
    }

    // Open file
    if (!QFile::exists(absoluteFilePath))
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("File at path was not found: ") %
                     absoluteFilePath;
        }
        return {};
    }

    QFile file(absoluteFilePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Failed to open file at path: ") % absoluteFilePath;
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
            *error = QStringLiteral("Config file does not contain a JSON object: ") %
                     absoluteFilePath;
        }
        return {};
    }

    const auto rootObject = doc.object();

    // Read 'environment_variables' member
    if (!readEnvironmentVariablesMember(rootObject, environmentVariables, error))
    {
        if (error != nullptr)
        {
            *error = QString("Failed to read the 'environment_variables' member:"
                             "\n    file path: %1"
                             "\n    error: [%2]").arg(absoluteFilePath, *error);
        }
        return {};
    }

    // Read 'includes' member
    auto completeConfig = readIncludesMember(rootObject,
                                             QFileInfo(absoluteFilePath).absoluteDir(),
                                             externalConfigs,
                                             environmentVariables,
                                             error);

    if (!completeConfig)
    {
        if (error != nullptr)
        {
            *error = QString("Failed to read the 'includes' member:"
                             "\n    file path: %1"
                             "\n    error: [%2]").arg(absoluteFilePath, *error);
        }
        return {};
    }

    // Read 'config' member
    auto configMember = readConfigMember(rootObject,
                                         externalConfigs,
                                         *completeConfig,
                                         *environmentVariables,
                                         error);

    if (!configMember)
    {
        if (error != nullptr)
        {
            *error = QString("Failed to read the 'config' member:"
                             "\n    file path: %1"
                             "\n    error: [%2]").arg(absoluteFilePath, *error);
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
                             "\n    error: [%2]").arg(absoluteFilePath, *error);
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
                             "\n    error: [%2]").arg(absoluteFilePath, *error);
        }
        return {};
    }

    return transformedConfig;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReader::read(
        const QDir &workingDir,
        const ConfigNodePath &destinationNodePath,
        const QVariantMap &otherParameters,
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        EnvironmentVariables *environmentVariables,
        QString *error) const
{
    // Extract file path
    if (!otherParameters.contains(QStringLiteral("file_path")))
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("The 'file_path' parameter is missing");
        }
        return {};
    }

    const QString filePath = otherParameters.value(QStringLiteral("file_path")).toString();

    if (filePath.isEmpty())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("The 'file_path' parameter is not valid");
        }
        return {};
    }

    // Extract source node
    auto sourceNodePath = ConfigNodePath::ROOT_PATH;

    if (otherParameters.contains(QStringLiteral("source_node")))
    {
        sourceNodePath.setPath(otherParameters.value(QStringLiteral("source_node")).toString());
    }

    // Read the configuration file
    return read(filePath,
                workingDir,
                sourceNodePath,
                destinationNodePath,
                externalConfigs,
                environmentVariables, error);
}

// -------------------------------------------------------------------------------------------------

bool ConfigReader::readEnvironmentVariablesMember(const QJsonObject &rootObject,
                                                  EnvironmentVariables *environmentVariables,
                                                  QString *error) const
{
    Q_ASSERT(environmentVariables);

    // Check if the root object has any environment variables
    const auto envVarsValue = rootObject.value(QStringLiteral("environment_variables"));

    if (envVarsValue.isNull() || envVarsValue.isUndefined())
    {
        // No additional environment variables
        return true;
    }

    if (!envVarsValue.isObject())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("The 'environment_variables' member in the root JSON Object is "
                                    "not a JSON Object!");
        }
        return false;
    }

    // Read individual environment variables
    auto envVarsObject = envVarsValue.toObject();

    for (auto it = envVarsObject.begin(); it != envVarsObject.end(); it++)
    {
        // Extract the name
        static const QRegularExpression regex("^\\w+$");
        const QString name = it.key();

        if (!regex.match(name).hasMatch())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Invalid environment variable name: ") % name;
            }
            return false;
        }

        // Extract the value
        const QJsonValue jsonValue = it.value();
        QString value;

        switch (jsonValue.type())
        {
            case QJsonValue::Bool:
            {
                value = jsonValue.toBool() ? QStringLiteral("1") : QStringLiteral("0");
                break;
            }

            case QJsonValue::Double:
            {
                value = QString::number(jsonValue.toDouble());
                break;
            }

            case QJsonValue::String:
            {
                value = jsonValue.toString();
                break;
            }

            default:
            {
                if (error != nullptr)
                {
                    *error = QString("Environment variable [%1] does not have a value that can be "
                                     "converted to a string!").arg(it.key());
                }
                return false;
            }
        }

        if (!environmentVariables->contains(name))
        {
            environmentVariables->setValue(name, value);
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReader::readIncludesMember(
        const QJsonObject &rootObject,
        const QDir &workingDir,
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        EnvironmentVariables *environmentVariables,
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
            *error = QStringLiteral("The 'includes' member in the root JSON Object is not a JSON "
                                    "array!");
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
        auto config = ConfigReaderFactory::instance()->readConfig(
                          type,
                          workingDir,
                          destinationNodePath,
                          includeObject.toVariantMap(),
                          extendedExternalConfigs,
                          environmentVariables,
                          error);

        if (!config)
        {
            if (error != nullptr)
            {
                *error = QString("Failed to read config for include:"
                                 "\n    index: %1"
                                 "\n    error: [%2]").arg(i).arg(*error);
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
        const EnvironmentVariables &environmentVariables,
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
            *error = QStringLiteral("The 'config' member in the root JSON Object is not a JSON "
                                    "Object!");
        }
        return {};
    }

    // Read 'config' object
    auto config = readObjectNode(configValue.toObject(),
                                 ConfigNodePath::ROOT_PATH,
                                 environmentVariables,
                                 error);

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
            *error = QString("Failed to resolve references. Error: [%1]").arg(*error);
        }
        return {};
    }

    return config;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigValueNode> ConfigReader::readValueNode(const QJsonValue &jsonValue,
                                                             const ConfigNodePath &currentNodePath)
{
    Q_UNUSED(currentNodePath)

    return std::make_unique<ConfigValueNode>(jsonValue.toVariant());
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReader::readObjectNode(
        const QJsonObject &jsonObject,
        const ConfigNodePath &currentNodePath,
        const EnvironmentVariables &environmentVariables,
        QString *error)
{
    auto objectNode = std::make_unique<ConfigObjectNode>();

    for (auto it = jsonObject.begin(); it != jsonObject.end(); it++)
    {
        QString memberName = it.key();

        // Check for "decorators" in the member name (reference type or Value node)
        QChar decorator;

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
                // Explicit Value node (even if it is a JSON Object type)
                memberNode = readValueNode(it.value(), memberNodePath);
                Q_ASSERT(memberNode);
                break;
            }

            case '$':
            {
                // Explicit Value node (even if it is a JSON Object type) where references to
                // environment variables in the value are resolved
                const QJsonValue resolvedValue =
                        resolveJsonValue(it.value(), environmentVariables, error);

                if (resolvedValue.isUndefined())
                {
                    if (error != nullptr)
                    {
                        *error = QString("Failed to resolve a Value node with references to "
                                         "environment variables:"
                                         "\n    member node path: %1"
                                         "\n    error: [%2]")
                                 .arg(memberNodePath.path(), *error);
                    }
                    return {};
                }

                memberNode = readValueNode(resolvedValue, memberNodePath);
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
                                                       environmentVariables,
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
                        *error = QStringLiteral("Unsupported reference type at path: ") %
                                 memberNodePath.path();
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
                    memberNode = readObjectNode(it.value().toObject(),
                                                memberNodePath,
                                                environmentVariables,
                                                error);

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
        const EnvironmentVariables &environmentVariables,
        QString *error)
{
    // Extract bases
    const auto baseValue = jsonObject.value(QStringLiteral("base"));

    if (baseValue.isUndefined())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("A derived object doesn't have the 'base' member at path: ") %
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
                    *error = QStringLiteral("Unsupported JSON type for an item in the 'base' "
                                            "member at path: ") % currentNodePath.path();
                }
                return {};
            }

            bases.append(ConfigNodePath(item.toString()));
        }

        if (bases.isEmpty())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("The 'base' member is empty at path: ") %
                         currentNodePath.path();
            }
            return {};
        }
    }
    else
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Unsupported JSON type for an item in the 'base' member at "
                                    "path: ") % currentNodePath.path();
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
            config = readObjectNode(configValue.toObject(),
                                    currentNodePath,
                                    environmentVariables,
                                    error);

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
                *error = QStringLiteral("Unsupported JSON type for the 'config' member at path: ") %
                         currentNodePath.path();
            }
            return {};
        }
    }

    // Create derived object node
    return std::make_unique<ConfigDerivedObjectNode>(bases, *config);
}

// -------------------------------------------------------------------------------------------------

QJsonValue ConfigReader::resolveJsonValue(const QJsonValue &jsonValue,
                                          const EnvironmentVariables &environmentVariables,
                                          QString *error)
{
    QJsonValue resolvedJsonValue;

    switch (jsonValue.type())
    {
        case QJsonValue::Array:
        {
            resolvedJsonValue = resolveJsonArray(jsonValue.toArray(), environmentVariables, error);
            break;
        }

        case QJsonValue::Object:
        {
            resolvedJsonValue = resolveJsonObject(jsonValue.toObject(),
                                                  environmentVariables,
                                                  error);
            break;
        }

        case QJsonValue::String:
        {
            QString value = jsonValue.toString();

            if (!value.isEmpty())
            {
                // Value is not an empty string, expand it
                value = environmentVariables.expandText(value);

                if (value.isNull())
                {
                    if (error != nullptr)
                    {
                        *error = QStringLiteral("Failed to resolve String value: ") %
                                 jsonValue.toString();
                    }
                    return QJsonValue(QJsonValue::Undefined);
                }
            }

            resolvedJsonValue = QJsonValue(value);
            break;
        }

        default:
        {
            // For all other types, just copy the value
            resolvedJsonValue = jsonValue;
            break;
        }
    }

    return resolvedJsonValue;
}

// -------------------------------------------------------------------------------------------------

QJsonValue ConfigReader::resolveJsonArray(const QJsonArray &jsonArray,
                                          const EnvironmentVariables &environmentVariables,
                                          QString *error)
{
    QJsonArray array;

    for (int i = 0; i < jsonArray.size(); i++)
    {
        QJsonValue item = jsonArray.at(i);

        if (!item.isUndefined())
        {
            item = resolveJsonValue(item, environmentVariables, error);

            if (item.isUndefined())
            {
                if (error != nullptr)
                {
                    *error = QString("Failed to resolve Array item:"
                                     "\n    index: %1"
                                     "\n    error: [%2]").arg(i).arg(*error);
                }
                return QJsonValue(QJsonValue::Undefined);
            }
        }

        // Insert the item into the output Array
        array.append(item);
    }

    return array;
}

// -------------------------------------------------------------------------------------------------

QJsonValue ConfigReader::resolveJsonObject(const QJsonObject &jsonObject,
                                           const EnvironmentVariables &environmentVariables,
                                           QString *error)
{
    QJsonObject object;

    for (auto it = jsonObject.begin(); it != jsonObject.end(); it++)
    {
        // Resolve key
        QString key = it.key();

        if (!key.isEmpty())
        {
            key = environmentVariables.expandText(key);

            if (key.isNull())
            {
                if (error != nullptr)
                {
                    *error = QStringLiteral("Failed to resolve Object key: ") % it.key();
                }
                return QJsonValue(QJsonValue::Undefined);
            }
        }

        // Resolve value
        QJsonValue value = it.value();

        if (!value.isUndefined())
        {
            value = resolveJsonValue(value, environmentVariables, error);

            if (value.isUndefined())
            {
                if (error != nullptr)
                {
                    *error = QString("Failed to resolve Object value:"
                                     "\n    key: %1"
                                     "\n    error: [%2]").arg(it.key(), *error);
                }
                return QJsonValue(QJsonValue::Undefined);
            }
        }

        // Insert the key-value pair into the output Object
        object.insert(key, value);
    }

    return object;
}

// -------------------------------------------------------------------------------------------------

bool ConfigReader::hasDecorator(const QString &memberName)
{
    static QRegularExpression regex("^[&#$]");

    return regex.match(memberName).hasMatch();
}

} // namespace CppConfigFramework
