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
 * Contains the registry for classes that can read different type of configuration files
 */

// Own header
#include <CppConfigFramework/ConfigReaderRegistry.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigObjectNode.hpp>
#include <CppConfigFramework/ConfigReader.hpp>
#include <CppConfigFramework/LoggingCategories.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

ConfigReaderRegistry *ConfigReaderRegistry::instance()
{
    static ConfigReaderRegistry factory;

    return &factory;
}

// -------------------------------------------------------------------------------------------------

bool ConfigReaderRegistry::registerConfigReader(const QString &type,
                                                std::unique_ptr<ConfigReaderBase> configReader)
{
    if (type.isEmpty() || (!configReader))
    {
        return false;
    }

    m_configReaders[type] = std::move(configReader);
    return true;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReaderRegistry::readConfig(
        const QString &type,
        const QDir &workingDir,
        const ConfigNodePath &destinationNodePath,
        const QJsonObject &otherParameters,
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        EnvironmentVariables *environmentVariables) const
{
    // Get the specified type of config reader
    auto it = m_configReaders.find(type);

    if (it == m_configReaders.end())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << "Unsupported configuration type:" << type;
        return {};
    }

    auto *configReader = it->second.get();

    // Read the config
    return configReader->read(workingDir,
                              destinationNodePath,
                              otherParameters,
                              externalConfigs,
                              environmentVariables);
}

// -------------------------------------------------------------------------------------------------

ConfigReaderRegistry::ConfigReaderRegistry()
{
    registerConfigReader(QStringLiteral("CppConfigFramework"), std::make_unique<ConfigReader>());
}

} // namespace CppConfigFramework
