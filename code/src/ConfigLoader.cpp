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
 * Contains a base class for the loading configuration nodes to actual configuration parameters
 */

// Own header
#include <CppConfigFramework/ConfigLoader.hpp>

// C++ Config Framework includes

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

bool ConfigLoader::loadConfigAtPath(const ConfigNodePath &path,
                                    const ConfigObjectNode &config,
                                    QString *error)
{
    // Validate node path
    if (!path.isValid())
    {
        const QString errorString = QString("Configuration node path [%1] is not valid!")
                                    .arg(path.path());
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    // Try to find the node
    const auto *node = config.nodeAtPath(path);

    if (node == nullptr)
    {
        const QString errorString = QString("Configuration node [%1] was not found!")
                                    .arg(path.toAbsolute(config.nodePath()).path());
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    // Load the configuration structure from the configuration node
    return loadConfigFromNode(*node, error);
}

// -------------------------------------------------------------------------------------------------

bool ConfigLoader::loadConfigAtPath(const QString &path,
                                    const ConfigObjectNode &config,
                                    QString *error)
{
    return loadConfigAtPath(ConfigNodePath(path), config, error);
}

// -------------------------------------------------------------------------------------------------

bool ConfigLoader::loadOptionalConfigAtPath(const ConfigNodePath &path,
                                            const ConfigObjectNode &config,
                                            bool *loaded,
                                            QString *error)
{
    // Validate node path
    if (!path.isValid())
    {
        const QString errorString = QString("Configuration node path [%1] is not valid!")
                                    .arg(path.path());
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }

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

    // Load the configuration structure from the configuration node
    const bool result = loadConfigFromNode(*node, error);

    if (loaded != nullptr)
    {
        *loaded = result;
    }
    return result;
}

// -------------------------------------------------------------------------------------------------

bool ConfigLoader::loadOptionalConfigAtPath(const QString &path,
                                            const ConfigObjectNode &config,
                                            bool *loaded,
                                            QString *error)
{
    return loadOptionalConfigAtPath(ConfigNodePath(path), config, loaded, error);
}

// -------------------------------------------------------------------------------------------------

bool ConfigLoader::loadConfig(const QString &parameterName,
                              const ConfigObjectNode &config,
                              QString *error)
{
    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Parameter name [%1] is not valid!").arg(parameterName);
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    return loadConfigAtPath(ConfigNodePath(parameterName), config, error);
}

// -------------------------------------------------------------------------------------------------

bool ConfigLoader::loadOptionalConfig(const QString &parameterName,
                                      const ConfigObjectNode &config,
                                      bool *loaded,
                                      QString *error)
{
    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Parameter name [%1] is not valid!").arg(parameterName);
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    return loadOptionalConfigAtPath(ConfigNodePath(parameterName), config, loaded, error);
}

// -------------------------------------------------------------------------------------------------

bool ConfigLoader::loadConfigFromNode(const ConfigNode &node, QString *error)
{
    if (!node.isObject())
    {
        const QString errorString = QString("Configuration node [%1] is not an Object node!")
                                    .arg(node.nodePath().path());
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    QString loadingError;

    if (!loadConfigParameters(node.toObject(), &loadingError))
    {
        const QString errorString = QString("Failed to load the configuration parameters [%1]! "
                                            "Error: [%2]").arg(node.nodePath().path(),
                                                               loadingError);
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    // Validate the configuration structure
    const QString validationError = validateConfig();

    if (!validationError.isEmpty())
    {
        const QString errorString = QString("Configuration [%1] is not valid! Error: [%2]")
                                    .arg(node.nodePath().path(), validationError);
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

QString ConfigLoader::validateConfig() const
{
    return QString();
}

// -------------------------------------------------------------------------------------------------

void ConfigLoader::handleError(const QString &error)
{
    Q_UNUSED(error);
}

} // namespace CppConfigFramework
