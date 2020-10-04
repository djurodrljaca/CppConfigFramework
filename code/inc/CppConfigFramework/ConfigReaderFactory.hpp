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
 * Contains the factory for classes that can read different type of configuration files
 */

#pragma once

// C++ Config Framework includes
#include <CppConfigFramework/ConfigReaderBase.hpp>

// Qt includes

// System includes
#include <unordered_map>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

//! This is a base class for reading configurations
class CPPCONFIGFRAMEWORK_EXPORT ConfigReaderFactory
{
public:
    //! Destructor
    ~ConfigReaderFactory() = default;

    /*!
     * Gets the factory instance
     *
     * \return  Factory instance
     */
    static ConfigReaderFactory *instance();

    /*!
     * Registers a configuration reader type
     *
     * \param   type            Configuration reader type
     * \param   configReader    Configuration reader instance
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool registerConfigReader(const QString &type,
                              std::unique_ptr<ConfigReaderBase> configReader);

    /*!
     * Read the specified configuration
     *
     * \param   type                Configuration reader type
     * \param   workingDir          Path to the working directory
     * \param   destinationNodePath Node path to the destination node where the result needs to be
     *                              stored (must be absolute node path)
     * \param   otherParameters     Other parameters need to parse this configuration
     * \param   externalConfigs     Configuration nodes provided by an external source
     *
     * \param[in,out]   environmentVariables    Environment variables
     *
     * \return  Configuration node instance or in case of failure a null pointer
     *
     * The externalConfigs items are used to provide an additional source for reference resolution.
     * This is mostly useful for includes so that they can declare references to externally defined
     * nodes in its own config file or its includes.
     */
    std::unique_ptr<ConfigObjectNode> readConfig(
            const QString &type,
            const QDir &workingDir,
            const ConfigNodePath &destinationNodePath,
            const QJsonObject &otherParameters,
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            EnvironmentVariables *environmentVariables) const;

private:
    //! Constructor
    ConfigReaderFactory();

private:
    //! Holds the
    std::map<QString, std::unique_ptr<ConfigReaderBase>> m_configReaders;
};

} // namespace CppConfigFramework
