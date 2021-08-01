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
 * Contains a base class for loading configuration nodes to actual configuration parameters and
 * storing actual configuration parameters to configuration nodes
 */

// Own header
#include <CppConfigFramework/ConfigItem.hpp>

// C++ Config Framework includes

// Qt includes
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

bool ConfigItem::loadConfig(const ConfigObjectNode &config)
{
    if (!loadConfigParameters(config))
    {
        const QString errorString = QString("Failed to load the configuration parameters [%1]!")
                                    .arg(config.nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Validate the configuration structure
    const QString validationError = validateConfig();

    if (!validationError.isEmpty())
    {
        const QString errorString = QString("Configuration [%1] is not valid! Error: [%2]")
                                    .arg(config.nodePath().path(), validationError);
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigItem::loadConfig(const QString &parameterName, const ConfigObjectNode &config)
{
    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Parameter name [%1] is not valid!").arg(parameterName);
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    return loadConfigAtPath(ConfigNodePath(parameterName), config);
}

// -------------------------------------------------------------------------------------------------

bool ConfigItem::loadOptionalConfig(const QString &parameterName,
                                    const ConfigObjectNode &config,
                                    bool *loaded)
{
    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Parameter name [%1] is not valid!").arg(parameterName);
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    return loadOptionalConfigAtPath(ConfigNodePath(parameterName), config, loaded);
}

// -------------------------------------------------------------------------------------------------

bool ConfigItem::loadConfigAtPath(const ConfigNodePath &path, const ConfigObjectNode &config)
{
    // Validate node path
    if (!path.isValid())
    {
        const QString errorString = QString("Configuration node path [%1] is not valid!")
                                    .arg(path.path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Try to find the node
    const auto *node = config.nodeAtPath(path);

    if (node == nullptr)
    {
        const QString errorString = QString("Configuration node [%1] was not found!")
                                    .arg(path.toAbsolute(config.nodePath()).path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    if (!node->isObject())
    {
        const QString errorString = QString("Configuration node [%1] is not an Object node!")
                                    .arg(node->nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Load the configuration structure from the configuration node
    return loadConfig(node->toObject());
}

// -------------------------------------------------------------------------------------------------

bool ConfigItem::loadConfigAtPath(const QString &path, const ConfigObjectNode &config)
{
    return loadConfigAtPath(ConfigNodePath(path), config);
}

// -------------------------------------------------------------------------------------------------

bool ConfigItem::loadOptionalConfigAtPath(const ConfigNodePath &path,
                                          const ConfigObjectNode &config,
                                          bool *loaded)
{
    // Validate node path
    if (!path.isValid())
    {
        const QString errorString = QString("Configuration node path [%1] is not valid!")
                                    .arg(path.path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);

        if (loaded != nullptr)
        {
            *loaded = false;
        }
        return false;
    }

    // Try to find the node
    const auto *node = config.nodeAtPath(path);

    if (node == nullptr)
    {
        // Node was not found, skip it
        if (loaded != nullptr)
        {
            *loaded = false;
        }
        return true;
    }

    if (!node->isObject())
    {
        const QString errorString = QString("Configuration node [%1] is not an Object node!")
                                    .arg(node->nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Load the configuration structure from the configuration node
    const bool result = loadConfig(node->toObject());

    if (loaded != nullptr)
    {
        *loaded = result;
    }
    return result;
}

// -------------------------------------------------------------------------------------------------

bool ConfigItem::loadOptionalConfigAtPath(const QString &path,
                                          const ConfigObjectNode &config,
                                          bool *loaded)
{
    return loadOptionalConfigAtPath(ConfigNodePath(path), config, loaded);
}

// -------------------------------------------------------------------------------------------------

bool ConfigItem::storeConfig(ConfigObjectNode *config)
{
    if (config == nullptr)
    {
        const QString errorString("Configuration node is a null pointer!");
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    if (!storeConfigParameters(config))
    {
        const QString errorString = QString("Failed to store the configuration parameters [%1]!")
                                    .arg(config->nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigItem::storeConfig(const QString &parameterName, ConfigObjectNode *config)
{
    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Parameter name [%1] is not valid!").arg(parameterName);
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    return storeConfigAtPath(ConfigNodePath(parameterName), config);
}

// -------------------------------------------------------------------------------------------------

bool ConfigItem::storeConfigAtPath(const ConfigNodePath &path, ConfigObjectNode *config)
{
    if (config == nullptr)
    {
        const QString errorString("Configuration node is a null pointer!");
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Validate node path
    if (!path.isValid())
    {
        const QString errorString = QString("Configuration node path [%1] is not valid!")
                                    .arg(path.path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Get or create the object node at path (create missing nodes as object nodes)
    const auto nodeNameList = path.nodeNames();
    ConfigNode *node = path.isAbsolute() ? config->rootNode()
                                         : config;

    for (const QString &nodeName : nodeNameList)
    {
        if (nodeName == ConfigNodePath::PARENT_PATH_VALUE)
        {
            node = node->parent();
        }
        else
        {
            if (!node->isObject())
            {
                const QString errorString = QString("Cannot get the child node [%2] from a node at "
                                                    "path [%2] which is not an object!")
                                            .arg(nodeName, node->nodePath().path());
                qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
                handleError(errorString);
                return false;
            }

            ConfigObjectNode *parentNode = &node->toObject();

            if (!parentNode->contains(nodeName))
            {
                parentNode->setMember(nodeName, std::make_unique<ConfigObjectNode>());
            }

            node = parentNode->member(nodeName);
        }
    }

    if (!node->isObject())
    {
        const QString errorString = QString("Configuration node [%1] is not an Object node!")
                                    .arg(node->nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Stores the configuration structure to the configuration node
    return storeConfig(&node->toObject());
}

// -------------------------------------------------------------------------------------------------

bool ConfigItem::storeConfigAtPath(const QString &path, ConfigObjectNode *config)
{
    return storeConfigAtPath(ConfigNodePath(path), config);
}

// -------------------------------------------------------------------------------------------------

QString ConfigItem::jsonToString(const QJsonValue &value)
{
    switch (value.type())
    {
        case QJsonValue::Bool:
        {
            return value.toBool() ? "true"
                                  : "false";
        }

        case QJsonValue::Double:
        {
            return QString::number(value.toDouble(), 'g', 15);
        }

        case QJsonValue::String:
        {
            return value.toString();
        }

        case QJsonValue::Array:
        {
            return QString::fromUtf8(QJsonDocument(value.toArray()).toJson(QJsonDocument::Compact));
        }

        case QJsonValue::Object:
        {
            return QString::fromUtf8(QJsonDocument(value.toObject()).toJson(QJsonDocument::Compact));
        }

        case QJsonValue::Null:
        case QJsonValue::Undefined:
        default:
        {
            return QString();
        }
    }
}

// -------------------------------------------------------------------------------------------------

QString ConfigItem::validateConfig() const
{
    return {};
}

// -------------------------------------------------------------------------------------------------

void ConfigItem::handleError(const QString &error)
{
    Q_UNUSED(error);
}

} // namespace CppConfigFramework
