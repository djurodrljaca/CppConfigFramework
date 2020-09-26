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
#include <CppConfigFramework/LoggingCategories.hpp>

// Qt includes
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QRegularExpression>

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
        EnvironmentVariables *environmentVariables) const
{
    // Make sure that file path is not empty
    if (filePath.isEmpty())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader) << "File path is empty!";
        return {};
    }

    // Validate source node path
    if ((!sourceNodePath.isAbsolute()) ||
        (!sourceNodePath.isValid()))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Invalid source node path:" << sourceNodePath.path();
        return {};
    }

    // Validate destination node
    if ((!destinationNodePath.isAbsolute()) ||
        (!destinationNodePath.isValid()))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Invalid destination node path:" << destinationNodePath.path();
        return {};
    }

    // Check if environment variables were provided
    if (environmentVariables == nullptr)
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Environment variables are not provided!";
        return {};
    }

    // Expand references to environment variables in the file path
    const QString expandedFilePath = environmentVariables->expandText(filePath);

    if (expandedFilePath.isEmpty())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Failed to expand file path:" << filePath;
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
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << "Invalid external configs for file at path:" << absoluteFilePath;
            return {};
        }
    }

    // Open file
    if (!QFile::exists(absoluteFilePath))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "File at path was not found:" << absoluteFilePath;
        return {};
    }

    QFile file(absoluteFilePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Failed to open file at path:" << absoluteFilePath;
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

        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << QString("Failed to parse the file contents:"
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
        return {};
    }

    if (!doc.isObject())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Config file does not contain a JSON object:" << absoluteFilePath;
        return {};
    }

    const auto rootObject = doc.object();

    // Read 'environment_variables' member
    if (!readEnvironmentVariablesMember(rootObject, environmentVariables))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Failed to read the 'environment_variables' member:"
                   "\n    file path:" << absoluteFilePath;
        return {};
    }

    // Read 'includes' member
    auto completeConfig = readIncludesMember(rootObject,
                                             QFileInfo(absoluteFilePath).absoluteDir(),
                                             externalConfigs,
                                             environmentVariables);

    if (!completeConfig)
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Failed to read the 'includes' member:"
                   "\n    file path:" << absoluteFilePath;
        return {};
    }

    // Update current directory environment variable
    setCurrentDirectory(QFileInfo(absoluteFilePath).absoluteDir(), environmentVariables);

    // Read 'config' member
    auto configMember = readConfigMember(rootObject,
                                         externalConfigs,
                                         *completeConfig,
                                         *environmentVariables);

    if (!configMember)
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Failed to read the 'config' member:"
                   "\n    file path:" << absoluteFilePath;
        return {};
    }

    // Apply the overloads from 'config' member to the read configuration
    completeConfig->apply(*configMember);

    // Resolve references
    if (!resolveReferences(externalConfigs, completeConfig.get()))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Failed to resolve references:"
                   "\n    file path:" << absoluteFilePath;
        return {};
    }

    // Transform the configuration node based on source and destination node paths
    auto transformedConfig = transformConfig(std::move(completeConfig),
                                             sourceNodePath,
                                             destinationNodePath);

    if (!transformedConfig)
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Failed to transform the config:"
                   "\n    file path:" << absoluteFilePath;
        return {};
    }

    return transformedConfig;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReader::read(
        const QDir &workingDir,
        const ConfigNodePath &destinationNodePath,
        const QJsonObject &otherParameters,
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        EnvironmentVariables *environmentVariables) const
{
    // Extract file path
    QString filePath;

    if (!CedarFramework::deserializeNode(otherParameters, QStringLiteral("file_path"), &filePath))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "The 'file_path' parameter is missing or invalid";
        return {};
    }

    if (filePath.isEmpty())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "The 'file_path' parameter is must not be empty";
        return {};
    }

    // Extract source node
    ConfigNodePath sourceNodePath = ConfigNodePath::ROOT_PATH;

    if (!CedarFramework::deserializeOptionalNode(otherParameters,
                                                 QStringLiteral("source_node"),
                                                 &sourceNodePath))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "The 'source_node' parameter is invalid";
        return {};
    }

    // Read the configuration file
    return read(filePath,
                workingDir,
                sourceNodePath,
                destinationNodePath,
                externalConfigs,
                environmentVariables);
}

// -------------------------------------------------------------------------------------------------

bool ConfigReader::readEnvironmentVariablesMember(const QJsonObject &rootObject,
                                                  EnvironmentVariables *environmentVariables) const
{
    Q_ASSERT(environmentVariables);

    // Read individual environment variables
    QMap<QString, QString> newEnvironmentVariables;

    if (!CedarFramework::deserializeOptionalNode(rootObject,
                                                 QStringLiteral("environment_variables"),
                                                 &newEnvironmentVariables))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "The 'environment_variables' member in the root JSON Object is invalid";
        return false;
    }

    for (auto it = newEnvironmentVariables.begin(); it != newEnvironmentVariables.end(); it++)
    {
        // Extract the name
        static const QRegularExpression regex("^\\w+$");
        const QString name = it.key();

        if (!regex.match(name).hasMatch())
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << "Invalid environment variable name:" << name;
            return false;
        }

        // Add the environment variable value only if it is not set yet
        if (!environmentVariables->contains(name))
        {
            environmentVariables->setValue(name, it.value());
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReader::readIncludesMember(
        const QJsonObject &rootObject,
        const QDir &workingDir,
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        EnvironmentVariables *environmentVariables) const
{
    // Read individual environment variables
    QVector<QJsonObject> includes;

    if (!CedarFramework::deserializeOptionalNode(rootObject,
                                                 QStringLiteral("includes"),
                                                 &includes))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "The 'includes' member in the root JSON Object is invalid";
        return {};
    }

    auto includesConfig = std::make_unique<ConfigObjectNode>();

    for (int i = 0; i < includes.size(); i++)
    {
        const auto includeObject = includes.at(i);

        // Extract configuration file type
        QString type = QStringLiteral("CppConfigFramework");

        if (!CedarFramework::deserializeOptionalNode(includeObject,
                                                     QStringLiteral("type"),
                                                     &type))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << QString("The 'type' member for include at index [%1] is invalid").arg(i);
            return {};
        }

        if (type.isEmpty())
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << QString("The 'type' member for include at index [%1] is empty").arg(i);
            return {};
        }

        // Extract destination node
        auto destinationNodePath = ConfigNodePath::ROOT_PATH;

        if (!CedarFramework::deserializeOptionalNode(includeObject,
                                                     QStringLiteral("destination_node"),
                                                     &destinationNodePath))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << QString("The 'destination_node' member for include at index [%1] is invalid")
                       .arg(i);
            return {};
        }

        if (destinationNodePath.isRelative() || (!destinationNodePath.isValid()))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << QString("The 'destination_node' member [%1] for include at index [%2] "
                               "is invalid")
                       .arg(destinationNodePath.path())
                       .arg(i);
            return {};
        }

        // Extend the external configs with the includesConfig value if necessary
        auto extendedExternalConfigs = externalConfigs;

        if (includesConfig->count() > 0)
        {
            extendedExternalConfigs.push_back(includesConfig.get());
        }

        // Update current directory environment variable
        setCurrentDirectory(workingDir, environmentVariables);

        // Read config file
        // TODO: limit the includes depth to prevent an endless include loop?
        auto config = ConfigReaderFactory::instance()->readConfig(
                          type,
                          workingDir,
                          destinationNodePath,
                          includeObject,
                          extendedExternalConfigs,
                          environmentVariables);

        if (!config)
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << "Failed to read config for include at index:" << i;
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
        const EnvironmentVariables &environmentVariables) const
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
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "The 'config' member in the root JSON Object is not a JSON Object!";
        return {};
    }

    // Read 'config' object
    auto config = readObjectNode(configValue.toObject(),
                                 ConfigNodePath::ROOT_PATH,
                                 environmentVariables);

    if (!config)
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Failed to read the 'config' member in the root JSON Object!";
        return {};
    }

    // Extend the external configs with the includesConfig value if necessary
    auto extendedExternalConfigs = externalConfigs;

    if (includesConfig.count() > 0)
    {
        extendedExternalConfigs.push_back(&includesConfig);
    }

    // Resolve references
    if (!resolveReferences(extendedExternalConfigs, config.get()))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Failed to resolve references!";
        return {};
    }

    return config;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigValueNode> ConfigReader::readValueNode(const QJsonValue &jsonValue,
                                                             const ConfigNodePath &currentNodePath)
{
    Q_UNUSED(currentNodePath)

    return std::make_unique<ConfigValueNode>(jsonValue);
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReader::readObjectNode(
        const QJsonObject &jsonObject,
        const ConfigNodePath &currentNodePath,
        const EnvironmentVariables &environmentVariables)
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
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << QString("Invalid member name [%1] in path [%2]")
                       .arg(memberName, currentNodePath.path());
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
                const QJsonValue resolvedValue = resolveJsonValue(it.value(), environmentVariables);

                if (resolvedValue.isUndefined())
                {
                    qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                            << "Failed to resolve a Value node with references to "
                               "environment variables:"
                               "\n    member node path:" << memberNodePath.path();
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
                    memberNode = readNodeReferenceNode(it.value().toString(), memberNodePath);

                    if (!memberNode)
                    {
                        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                                << "Failed to read the a NodeReference node member:"
                                   "\n    member node path:" << memberNodePath.path();
                        return {};
                    }
                }
                else if (it.value().isObject())
                {
                    memberNode = readDerivedObjectNode(it.value().toObject(),
                                                       memberNodePath,
                                                       environmentVariables);

                    if (!memberNode)
                    {
                        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                                << "Failed to read the a NodeReference node member:"
                                   "\n    member node path:" << memberNodePath.path();
                        return {};
                    }
                }
                else
                {
                    qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                            << "Unsupported reference type at path:" << memberNodePath.path();
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
                                                environmentVariables);

                    if (!memberNode)
                    {
                        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                                << "Failed to read the an ordinary Object node member:"
                                   "\n    member node path:" << memberNodePath.path();
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
        const QString &reference, const ConfigNodePath &currentNodePath)
{
    const ConfigNodePath referencePath(reference);

    if (!referencePath.toAbsolute(currentNodePath).isValid())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << QString("Invalid node reference [%1] with current path [%2]")
                   .arg(reference, currentNodePath.path());
        return {};
    }

    return std::make_unique<ConfigNodeReference>(referencePath);
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigDerivedObjectNode> ConfigReader::readDerivedObjectNode(
        const QJsonObject &jsonObject,
        const ConfigNodePath &currentNodePath,
        const EnvironmentVariables &environmentVariables)
{
    // Extract bases
    const auto baseValue = jsonObject.value(QStringLiteral("base"));

    if (baseValue.isUndefined())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "A derived object doesn't have the 'base' member at path:"
                << currentNodePath.path();
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
                qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                        << "Unsupported JSON type for an item in the 'base' member at path:"
                        << currentNodePath.path();
                return {};
            }

            bases.append(ConfigNodePath(item.toString()));
        }

        if (bases.isEmpty())
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << "The 'base' member is empty at path:" << currentNodePath.path();
            return {};
        }
    }
    else
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Unsupported JSON type for an item in the 'base' member at path:"
                << currentNodePath.path();
        return {};
    }

    for (const auto &item : bases)
    {
        if (!item.toAbsolute(currentNodePath).isValid())
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << QString("Invalid node path in base item at path:"
                               "\n    base item's node path: %1"
                               "\n    node path: %2")
                       .arg(item.path(), currentNodePath.path());
            return {};
        }
    }

    // Extract config
    auto config = std::make_unique<ConfigObjectNode>();
    const auto configValue = jsonObject.value(QStringLiteral("config"));

    switch (configValue.type())
    {
        case QJsonValue::Object:
        {
            // Read overrides for the object derived from bases
            config = readObjectNode(configValue.toObject(), currentNodePath, environmentVariables);

            if (!config)
            {
                qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                        << "Failed to read the overrides for the object derived from bases at path:"
                           "\n    node path:" << currentNodePath.path();
                return {};
            }
            break;
        }

        case QJsonValue::Null:
        case QJsonValue::Undefined:
        {
            // No overrides for the object derived from bases
            break;
        }

        default:
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << "Unsupported JSON type for the 'config' member at path:"
                    << currentNodePath.path();
            return {};
        }
    }

    // Create derived object node
    return std::make_unique<ConfigDerivedObjectNode>(bases, *config);
}

// -------------------------------------------------------------------------------------------------

QJsonValue ConfigReader::resolveJsonValue(const QJsonValue &jsonValue,
                                          const EnvironmentVariables &environmentVariables)
{
    QJsonValue resolvedJsonValue;

    switch (jsonValue.type())
    {
        case QJsonValue::Array:
        {
            resolvedJsonValue = resolveJsonArray(jsonValue.toArray(), environmentVariables);
            break;
        }

        case QJsonValue::Object:
        {
            resolvedJsonValue = resolveJsonObject(jsonValue.toObject(), environmentVariables);
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
                    qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                            << "Failed to resolve String value:" << jsonValue.toString();
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
                                          const EnvironmentVariables &environmentVariables)
{
    QJsonArray array;

    for (int i = 0; i < jsonArray.size(); i++)
    {
        QJsonValue item = jsonArray.at(i);

        if (!item.isUndefined())
        {
            item = resolveJsonValue(item, environmentVariables);

            if (item.isUndefined())
            {
                qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                        << "Failed to resolve Array item at index:" << i;
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
                                           const EnvironmentVariables &environmentVariables)
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
                qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                        << "Failed to resolve Object key:" << it.key();
                return QJsonValue(QJsonValue::Undefined);
            }
        }

        // Resolve value
        QJsonValue value = it.value();

        if (!value.isUndefined())
        {
            value = resolveJsonValue(value, environmentVariables);

            if (value.isUndefined())
            {
                qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                        << "Failed to resolve Object value with key:" << it.key();
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

// -------------------------------------------------------------------------------------------------

void ConfigReader::setCurrentDirectory(const QDir &currentDir,
                                       EnvironmentVariables *environmentVariables)
{
    environmentVariables->setValue(QStringLiteral("CPPCONFIGFRAMEWORK_CURRENT_DIR"),
                                   currentDir.absolutePath());
}

} // namespace CppConfigFramework
