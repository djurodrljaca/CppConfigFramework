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

#ifndef CPPCONFIGFRAMEWORK_CONFIGREADER_HPP
#define CPPCONFIGFRAMEWORK_CONFIGREADER_HPP

// C++ Config Framework includes
#include <CppConfigFramework/ConfigReaderBase.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

//! This class reads the configuration
class CPPCONFIGFRAMEWORK_EXPORT ConfigReader : public ConfigReaderBase
{
public:
    //! Constructor
    ConfigReader() = default;

    //! Copy constructor
    ConfigReader(const ConfigReader &) = default;

    //! Move constructor
    ConfigReader(ConfigReader &&) noexcept = default;

    //! Destructor
    ~ConfigReader() = default;

    //! Copy assignment operator
    ConfigReader &operator=(const ConfigReader &) = default;

    //! Move assignment operator
    ConfigReader &operator=(ConfigReader &&) noexcept = default;

    /*!
     * Read the specified config file
     *
     * \param   filePath            Path to the configuration file
     * \param   workingDir          Path to the working directory
     * \param   sourceNodePath      Node path to the node that needs to be extracted from this
     *                              configuration file (must be absolute node path)
     * \param   destinationNodePath Node path to the destination node where the result needs to be
     *                              stored (must be absolute node path)
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
    std::unique_ptr<ConfigObjectNode> read(
            const QString &filePath,
            const QDir &workingDir,
            const ConfigNodePath &sourceNodePath,
            const ConfigNodePath &destinationNodePath,
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            EnvironmentVariables *environmentVariables) const;

    //! \copydoc    ConfigReaderBase::read()
    std::unique_ptr<ConfigObjectNode> read(
            const QDir &workingDir,
            const ConfigNodePath &destinationNodePath,
            const QVariantMap &otherParameters,
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            EnvironmentVariables *environmentVariables) const override;

private:
    /*!
     * Reads the 'environment_variables' member of the configuration file
     *
     * \param   rootObject      Root JSON Object
     *
     * \param[in,out]   environmentVariables    Environment variables
     *
     * \retval  true    Success
     * \retval  false   Failure
     *
     * \note    Read environment variables are set in the environmentVariables parameter only if the
     *          variables are not already set!
     */
    bool readEnvironmentVariablesMember(
            const QJsonObject &rootObject,
            EnvironmentVariables *environmentVariables) const;

    /*!
     * Reads the 'includes' member of the configuration file
     *
     * \param   rootObject      Root JSON Object
     * \param   workingDir      Path to the working directory
     * \param   externalConfigs Configuration nodes provided by an external source
     *
     * \param[in,out]   environmentVariables    Environment variables
     *
     * \return  Configuration node instance or null in case of failure
     */
    std::unique_ptr<ConfigObjectNode> readIncludesMember(
            const QJsonObject &rootObject,
            const QDir &workingDir,
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            EnvironmentVariables *environmentVariables) const;

    /*!
     * Reads the 'config' member of the configuration file
     *
     * \param   rootObject              Root JSON Object
     * \param   externalConfigs         Configuration nodes provided by an external source
     * \param   includesConfig          Configuration node loaded from includes
     * \param   environmentVariables    Environment variables
     *
     * \return  Configuration node instance or null in case of failure
     */
    std::unique_ptr<ConfigObjectNode> readConfigMember(
            const QJsonObject &rootObject,
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            const ConfigObjectNode &includesConfig,
            const EnvironmentVariables &environmentVariables) const;

    /*!
     * Reads a Value node from the JSON Value
     *
     * \param   jsonValue       JSON Value
     * \param   currentNodePath Current node path
     *
     * \return  Configuration node instance or null in case of failure
     */
    static std::unique_ptr<ConfigValueNode> readValueNode(const QJsonValue &jsonValue,
                                                          const ConfigNodePath &currentNodePath);

    /*!
     * Reads an Object node from the JSON Object
     *
     * \param   jsonObject              JSON Object
     * \param   currentNodePath         Current node path
     * \param   environmentVariables    Environment variables
     *
     * \return  Configuration node instance or null in case of failure
     */
    static std::unique_ptr<ConfigObjectNode> readObjectNode(
            const QJsonObject &jsonObject,
            const ConfigNodePath &currentNodePath,
            const EnvironmentVariables &environmentVariables);

    /*!
     * Reads a NodeReference node from the JSON String
     *
     * \param   reference       JSON String
     * \param   currentNodePath Current node path
     *
     * \return  Configuration node instance or null in case of failure
     */
    static std::unique_ptr<ConfigNodeReference> readNodeReferenceNode(
            const QString &reference,
            const ConfigNodePath &currentNodePath);

    /*!
     * Reads a DerivedObject node from the JSON Object
     *
     * \param   jsonObject              JSON Object
     * \param   currentNodePath         Current node path
     * \param   environmentVariables    Environment variables
     *
     * \return  Configuration node instance or null in case of failure
     */
    static std::unique_ptr<ConfigDerivedObjectNode> readDerivedObjectNode(
            const QJsonObject &jsonObject,
            const ConfigNodePath &currentNodePath,
            const EnvironmentVariables &environmentVariables);

    /*!
     * Resolves references to environment variables in a JSON Value
     *
     * \param   jsonValue               JSON Value
     * \param   environmentVariables    Environment variables
     *
     * \return  Resolved JSON Value or an undefined value in case of failure
     */
    static QJsonValue resolveJsonValue(const QJsonValue &jsonValue,
                                       const EnvironmentVariables &environmentVariables);

    /*!
     * Resolves references to environment variables in a JSON Array
     *
     * \param   jsonArray               JSON Array
     * \param   environmentVariables    Environment variables
     *
     * \return  Resolved JSON Array or an undefined value in case of failure
     */
    static QJsonValue resolveJsonArray(const QJsonArray &jsonArray,
                                       const EnvironmentVariables &environmentVariables);

    /*!
     * Resolves references to environment variables in a JSON Object
     *
     * \param   jsonObject               JSON Object
     * \param   environmentVariables    Environment variables
     *
     * \return  Resolved JSON Object or an undefined value in case of failure
     */
    static QJsonValue resolveJsonObject(const QJsonObject &jsonObject,
                                        const EnvironmentVariables &environmentVariables);

    /*!
     * Checks if a member name starts with one of the decorator characters
     *
     * \param   memberName  Member name
     *
     * \retval  true    Member name starts with a decorator character
     * \retval  false   Member name does not start with a decorator character
     *
     * Decorator characters:
     * - '#': Represents a Value node
     * - '&': Represents one of the reference types (NodeReference, DerivedObject)
     */
    static bool hasDecorator(const QString &memberName);

    /*!
     * Sets the current directory environment variable (CPPCONFIGFRAMEWORK_CURRENT_DIR)
     *
     * \param   currentDir  Current directory
     *
     * \param[in,out]   environmentVariables    Environment variables
     */
    static void setCurrentDirectory(const QDir &currentDir,
                                    EnvironmentVariables *environmentVariables);
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGREADER_HPP
