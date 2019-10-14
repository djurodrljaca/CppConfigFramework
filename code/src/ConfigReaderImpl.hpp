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
 * Contains a private implementation for ConfigReader class
 */

#ifndef CPPCONFIGFRAMEWORK_CONFIGREADERIMPL_HPP
#define CPPCONFIGFRAMEWORK_CONFIGREADERIMPL_HPP

// C++ Config Framework includes
#include <CppConfigFramework/ConfigReader.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

/*!
 * Private implementation for ConfigReader class
 */
class ConfigReader::Impl
{
public:
    //! Constructor
    Impl();

    //! \copydoc    ConfigReader::referenceResolutionMaxCycles()
    uint32_t referenceResolutionMaxCycles() const;

    //! \copydoc    ConfigReader::setReferenceResolutionMaxCycles()
    void setReferenceResolutionMaxCycles(const uint32_t referenceResolutionMaxCycles);

    //! \copydoc    ConfigReader::read()
    std::unique_ptr<ConfigNode> read(const QString &filePath,
                                     const QString &sourceNode,
                                     const QString &destinationNode);

private:
    //! Enumerates the possible results of reference resolution procedure
    enum class ReferenceResolutionResult
    {
        Error,      //!< An error occured
        Unresolved, //!< At least one reference is still unresolved
        Resolved    //!< All references were resolved
    };

    /*!
     * Reads the 'includes' member of the configuration file
     *
     * \param   rootObject  Root JSON Object
     *
     * \return  Configuration node instance or in case of failure a null pointer
     */
    std::unique_ptr<ConfigNode> readIncludesMember(const QJsonObject &rootObject);

    /*!
     * Reads the 'config' member of the configuration file
     *
     * \param   rootObject  Root JSON Object
     *
     * \return  Configuration node instance or in case of failure a null pointer
     */
    std::unique_ptr<ConfigNode> readConfigMember(const QJsonObject &rootObject);

    /*!
     * Transforms the configuration node by taking the node referenced by the source node path and
     * moving it to the destination node path
     *
     * \param   config          Configuration node to transform
     * \param   sourceNode      Node path to the node that needs to be extracted from this
     *                          configuration file (must be absolute node path)
     * \param   destinationNode Node path to the destination node where the result needs to be
     *                          stored (must be absolute node path)
     *
     * \return  Configuration node instance or in case of failure a null pointer
     */
    std::unique_ptr<ConfigNode> transformConfig(ConfigNode &&config,
                                                const QString &sourceNode,
                                                const QString &destinationNode);

    /*!
     * Reads the JSON Value
     *
     * \param   jsonValue       JSON Value
     * \param   currentNodePath Current node path
     *
     * \return  Configuration node instance or in case of failure a null pointer
     */
    static std::unique_ptr<ConfigNode> readJsonValue(const QJsonValue &jsonValue,
                                                     const QString &currentNodePath);

    /*!
     * Reads the JSON Array
     *
     * \param   jsonArray       JSON Array
     * \param   currentNodePath Current node path
     *
     * \return  Configuration node instance or in case of failure a null pointer
     */
    static std::unique_ptr<ConfigNode> readJsonArray(const QJsonArray &jsonArray,
                                                     const QString &currentNodePath);

    /*!
     * Reads the JSON Object
     *
     * \param   jsonObject      JSON Object
     * \param   currentNodePath Current node path
     *
     * \return  Configuration node instance or in case of failure a null pointer
     */
    static std::unique_ptr<ConfigNode> readJsonObject(const QJsonObject &jsonObject,
                                                      const QString &currentNodePath);

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
     * - '&': Represents one of the reference types (NodeReference, DerivedArray, DerivedObject)
     */
    static bool hasDecorator(const QString &memberName);

    /*!
     * Reads a NodeReference node
     *
     * \param   nodeReference   Reference to another node
     * \param   currentNodePath Current node path
     *
     * \return  Configuration node instance or in case of failure a null pointer
     */
    static std::unique_ptr<ConfigNode> readReferenceNode(const QString &nodeReference,
                                                         const QString &currentNodePath);

    /*!
     * Reads a DerivedArray node
     *
     * \param   jsonArray       JSON Array
     * \param   currentNodePath Current node path
     *
     * \return  Configuration node instance or in case of failure a null pointer
     */
    static std::unique_ptr<ConfigNode> readDerivedArrayNode(const QJsonArray &jsonArray,
                                                            const QString &currentNodePath);

    /*!
     * Reads a DerivedObject node
     *
     * \param   jsonObject      JSON Object
     * \param   currentNodePath Current node path
     *
     * \return  Configuration node instance or in case of failure a null pointer
     */
    static std::unique_ptr<ConfigNode> readDerivedObjectNode(const QJsonObject &jsonObject,
                                                             const QString &currentNodePath);

    /*!
     * Tries to resolve all references in the specified node and all its sub-nodes
     *
     * \param   node    Config node
     *
     * \retval  ReferenceResolutionResult::Resolved     All references were resolved
     * \retval  ReferenceResolutionResult::Unresolved   All references were not resolved yet
     * \retval  ReferenceResolutionResult::Error        An error occured
     */
    static ReferenceResolutionResult resolveReferences(ConfigNode *node);

    /*!
     * Tries to resolve all references in the Array node
     *
     * \param   node    Config node
     *
     * \retval  ReferenceResolutionResult::Resolved     All references were resolved
     * \retval  ReferenceResolutionResult::Unresolved   All references were not resolved yet
     * \retval  ReferenceResolutionResult::Error        An error occured
     */
    static ReferenceResolutionResult resolveArrayReferences(ConfigNode *node);

    /*!
     * Tries to resolve all references in the Object node
     *
     * \param   node    Config node
     *
     * \retval  ReferenceResolutionResult::Resolved     All references were resolved
     * \retval  ReferenceResolutionResult::Unresolved   All references were not resolved yet
     * \retval  ReferenceResolutionResult::Error        An error occured
     */
    static ReferenceResolutionResult resolveObjectReferences(ConfigNode *node);

    /*!
     * Tries to resolve a NodeReference node
     *
     * \param   node    Config node
     *
     * \retval  ReferenceResolutionResult::Resolved     All references were resolved
     * \retval  ReferenceResolutionResult::Unresolved   All references were not yet resolved
     * \retval  ReferenceResolutionResult::Error        An error occured (no parent)
     */
    static ReferenceResolutionResult resolveNodeReference(ConfigNode *node);

    /*!
     * Tries to resolve all references in the DerivedArray node
     *
     * \param   node    Config node
     *
     * \retval  ReferenceResolutionResult::Resolved     All references were resolved
     * \retval  ReferenceResolutionResult::Unresolved   All references were not resolved yet
     * \retval  ReferenceResolutionResult::Error        An error occured
     */
    static ReferenceResolutionResult resolveDerivedArrayReferences(ConfigNode *node);

    /*!
     * Tries to resolve all references in the DerivedObject node
     *
     * \param   node    Config node
     *
     * \retval  ReferenceResolutionResult::Resolved     All references were resolved
     * \retval  ReferenceResolutionResult::Unresolved   All references were not resolved yet
     * \retval  ReferenceResolutionResult::Error        An error occured
     */
    static ReferenceResolutionResult resolveDerivedObjectReferences(ConfigNode *node);

    //! Checks if the node is fully resolved (has no references)
    static bool isFullyResolved(const ConfigNode &node);

    //! Holds the max number of cycles for reference resolution procedure
    uint32_t m_referenceResolutionMaxCycles;
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGREADERIMPL_HPP
