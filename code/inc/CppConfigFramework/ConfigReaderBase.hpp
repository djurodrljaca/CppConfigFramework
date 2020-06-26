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
 * Contains a base class for reading configurations
 */

#ifndef CPPCONFIGFRAMEWORK_CONFIGREADERBASE_HPP
#define CPPCONFIGFRAMEWORK_CONFIGREADERBASE_HPP

// C++ Config Framework includes
#include <CppConfigFramework/ConfigNode.hpp>
#include <CppConfigFramework/EnvironmentVariables.hpp>

// Qt includes
#include <QtCore/QString>
#include <QtCore/QDir>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

//! This is a base class for reading configurations
class CPPCONFIGFRAMEWORK_EXPORT ConfigReaderBase
{
public:
    //! Constructor
    ConfigReaderBase() = default;

    //! Copy constructor
    ConfigReaderBase(const ConfigReaderBase &) = default;

    //! Move constructor
    ConfigReaderBase(ConfigReaderBase &&) noexcept = default;

    //! Destructor
    virtual ~ConfigReaderBase() = default;

    //! Copy assignment operator
    ConfigReaderBase &operator=(const ConfigReaderBase &) = default;

    //! Move assignment operator
    ConfigReaderBase &operator=(ConfigReaderBase &&) noexcept = default;

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
     * Read the specified configuration
     *
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
    virtual std::unique_ptr<ConfigObjectNode> read(
            const QDir &workingDir,
            const ConfigNodePath &destinationNodePath,
            const QVariantMap &otherParameters,
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            EnvironmentVariables *environmentVariables) const = 0;

protected:
    //! Enumerates the possible results of reference resolution procedure
    enum class ReferenceResolutionResult
    {
        Resolved,           //!< There were either no references or they were all resolved
        PartiallyResolved,  //!< At least one of the references was resolved, but not all of them
        Unchanged,          //!< None of the references were resolved
        Error               //!< An error occurred
    };

protected:
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
     * \return  Reference resolution result
     */
    bool resolveReferences(const std::vector<const ConfigObjectNode *> &externalConfigs,
                           ConfigObjectNode *config) const;

    /*!
     * Tries to resolve all references in the specified Object node
     *
     * \param   externalConfigs     Configuration nodes provided by an external source
     *
     * \param[in,out]   node    Configuration node
     *
     * \return  Reference resolution result
     */
    static ReferenceResolutionResult resolveObjectReferences(
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            ConfigObjectNode *node);

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
     * \return  Reference resolution result
     */
    static ReferenceResolutionResult resolveNodeReference(
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            ConfigNodeReference *node);

    /*!
     * Tries to resolve all references in the specified DerivedObject node
     *
     * \param   externalConfigs     Configuration nodes provided by an external source
     *
     * \param[in,out]   node    Configuration node
     *
     * \return  Reference resolution result
     */
    static ReferenceResolutionResult resolveDerivedObjectReferences(
            const std::vector<const ConfigObjectNode *> &externalConfigs,
            ConfigDerivedObjectNode *node);

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
     * \return  Configuration node instance or in case of failure a null pointer
     */
    static std::unique_ptr<ConfigObjectNode> transformConfig(
            std::unique_ptr<ConfigObjectNode> &&config,
            const ConfigNodePath &sourceNodePath,
            const ConfigNodePath &destinationNodePath);

private:
    //! Holds the default value for max number of cycles for reference resolution procedure
    static constexpr uint32_t m_defaultReferenceResolutionMaxCycles = 100U;

    //! Holds the max number of cycles for reference resolution procedure
    uint32_t m_referenceResolutionMaxCycles = m_defaultReferenceResolutionMaxCycles;
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGREADERBASE_HPP
