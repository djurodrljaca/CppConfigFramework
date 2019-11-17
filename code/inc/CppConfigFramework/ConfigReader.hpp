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
#include <CppConfigFramework/ConfigNode.hpp>

// Qt includes
#include <QtCore/QString>
#include <QtCore/QDir>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

/*!
 * This class reads the configuration
 */
class CPPCONFIGFRAMEWORK_EXPORT ConfigReader
{
public:
    //! Constructor
    ConfigReader();

    /*!
     * Copy constructor
     *
     * \param   other   Instance to copy
     */
    ConfigReader(const ConfigReader &other) = default;

    /*!
     * Move constructor
     *
     * \param   other   Instance to move
     */
    ConfigReader(ConfigReader &&other) noexcept = default;

    //! Destructor
    ~ConfigReader() = default;

    /*!
     * Copy assignment operator
     *
     * \param   other   Instance to copy assign
     *
     * \return  Reference to this instance after the assignment is made
     */
    ConfigReader &operator=(const ConfigReader &other) = default;

    /*!
     * Move assignment operator
     *
     * \param   other   Instance to move assign
     *
     * \return  Reference to this instance after the assignment is made
     */
    ConfigReader &operator=(ConfigReader &&other) noexcept = default;;

    /*!
     * Gets the max number of cycles for reference resolution procedure
     *
     * \return  Max number of cycles
     */
    uint32_t referenceResolutionMaxCycles() const;

    /*!
     * Sets the max number of cycles for reference resolution procedure
     *
     * \param   referenceResolutionMaxCycles    New max number of cycles
     */
    void setReferenceResolutionMaxCycles(const uint32_t referenceResolutionMaxCycles);

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
     * \param[out]  error   Optional output for the error string
     *
     * \return  Configuration node instance or in case of failure a null pointer
     *
     * The externalConfigs items are used to provide an additional source for reference resolution.
     * This is mostly useful for includes so that they can declare references to externaly defined
     * nodes in its own config file or its includes.
     */
    std::unique_ptr<ConfigObjectNode> read(
            const QString &filePath,
            const QDir &workingDir = QDir::current(),
            const ConfigNodePath &sourceNodePath = ConfigNodePath::ROOT_PATH,
            const ConfigNodePath &destinationNodePath = ConfigNodePath::ROOT_PATH,
            const std::vector<const ConfigObjectNode *> &externalConfigs = {},
            QString *error = nullptr) const;

private:
    //! Enumerates the possible results of reference resolution procedure
    enum class ReferenceResolutionResult
    {
        Resolved,           //!< There were either no references or they were all resolved
        PartiallyResolved,  //!< At least one of the references was resolved, but not all of them
        Unchanged,          //!< None of the references were resolved
        Error               //!< An error occured
    };

private:
    /*!
     * Reads the 'includes' member of the configuration file
     *
     * \param   rootObject      Root JSON Object
     * \param   workingDir      Path to the working directory
     * \param   externalConfigs Configuration nodes provided by an external source
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Configuration node instance or null in case of failure
     */
    std::unique_ptr<ConfigObjectNode> readIncludesMember(
            const QJsonObject &rootObject,
            const QDir &workingDir,
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            QString *error) const;

    /*!
     * Reads the 'config' member of the configuration file
     *
     * \param   rootObject      Root JSON Object
     * \param   externalConfigs Configuration nodes provided by an external source
     * \param   includesConfig  Configuration node loaded from includes
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Configuration node instance or null in case of failure
     */
    std::unique_ptr<ConfigObjectNode> readConfigMember(const QJsonObject &rootObject,
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            const ConfigObjectNode &includesConfig,
            QString *error) const;

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
     * \param   jsonObject      JSON Object
     * \param   currentNodePath Current node path
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Configuration node instance or null in case of failure
     */
    static std::unique_ptr<ConfigObjectNode> readObjectNode(const QJsonObject &jsonObject,
                                                            const ConfigNodePath &currentNodePath,
                                                            QString *error);

    /*!
     * Reads a NodeReference node from the JSON String
     *
     * \param   reference       JSON String
     * \param   currentNodePath Current node path
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Configuration node instance or null in case of failure
     */
    static std::unique_ptr<ConfigNodeReference> readNodeReferenceNode(
            const QString &reference,
            const ConfigNodePath &currentNodePath,
            QString *error);

    /*!
     * Reads a DeriverObject node from the JSON Object
     *
     * \param   jsonObject      JSON Object
     * \param   currentNodePath Current node path
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Configuration node instance or null in case of failure
     */
    static std::unique_ptr<ConfigDerivedObjectNode> readDerivedObjectNode(
            const QJsonObject &jsonObject,
            const ConfigNodePath &currentNodePath,
            QString *error);

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
     * Checks if the node is fully resolved (has no unresolved references)
     *
     * \param   node    Configuration node
     *
     * \retval  true    Configuration node is fully resolved
     * \retval  false   Configuration node is not fully resolved
     */
    static bool isFullyResolved(const ConfigNode &node);

    /*!
     * Extracts all unresolved references from the configuration node
     *
     * \param   node    Configuration node
     *
     * \return  List of unresolved references
     */
    static QStringList unresolvedReferences(const ConfigObjectNode &node);

    /*!
     * Tries to resolve all references in the specified configuration node
     *
     * \param   externalConfigs     Configuration nodes provided by an external source
     *
     * \param[in,out]   config  Configuration node
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Reference resolution result
     */
    bool resolveReferences(
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            ConfigObjectNode *config,
            QString *error) const;

    /*!
     * Tries to resolve all references in the specified Object node
     *
     * \param   externalConfigs     Configuration nodes provided by an external source
     *
     * \param[in,out]   node    Configuration node
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Reference resolution result
     */
    static ReferenceResolutionResult resolveObjectReferences(
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            ConfigObjectNode *node,
            QString *error);

    /*!
     * Updates the reference resolution result
     *
     * \param   currentResult   Current result
     * \param   newResult       New result
     *
     * \return  Updated result
     */
    static ReferenceResolutionResult updateObjectResolutionResult(
            const ReferenceResolutionResult currentResult,
            const ReferenceResolutionResult newResult);

    /*!
     * Tries to resolve the reference in the specified NodeReference node
     *
     * \param   externalConfigs     Configuration nodes provided by an external source
     *
     * \param[in,out]   node    Configuration node
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Reference resolution result
     */
    static ReferenceResolutionResult resolveNodeReference(
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            ConfigNodeReference *node,
            QString *error);

    /*!
     * Tries to resolve all references in the specified DerivedObject node
     *
     * \param   externalConfigs     Configuration nodes provided by an external source
     *
     * \param[in,out]   node    Configuration node
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Reference resolution result
     */
    static ReferenceResolutionResult resolveDerivedObjectReferences(
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            ConfigDerivedObjectNode *node,
            QString *error);

    /*!
     * Try to find the referenced configuration node from the parent and as an alternative from the
     * configuration nodes provided by an external source
     *
     * \param   referenceNodePath   Reference to the configuration node
     * \param   parentNode          Parent configuration node
     * \param   externalConfigs     Configuration nodes provided by an external source
     *
     * \return  Referenced configuration node or null in case the node was not found
     */
    static const ConfigNode *findReferencedConfigNode(
            const ConfigNodePath &referenceNodePath,
            const ConfigObjectNode &parentNode,
            const std::vector<const ConfigObjectNode *> &externalConfigs);

    /*!
     * Transforms the configuration node by taking the node referenced by the source node path and
     * moving it to the destination node path
     *
     * \param   config              Configuration node to transform
     * \param   sourceNodePath      Node path to the node that needs to be extracted from this
     *                              configuration file (must be absolute node path)
     * \param   destinationNodePath Node path to the destination node where the result needs to be
     *                              stored (must be absolute node path)
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Configuration node instance or in case of failure a null pointer
     */
    static std::unique_ptr<ConfigObjectNode> transformConfig(
            std::unique_ptr<ConfigObjectNode> &&config,
            const ConfigNodePath &sourceNodePath,
            const ConfigNodePath &destinationNodePath,
            QString *error);

private:
    //! Holds the max number of cycles for reference resolution procedure (default: 100 cycles)
    uint32_t m_referenceResolutionMaxCycles;
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGREADER_HPP
