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
 * Contains a class for the DerivedObject configuration node
 */

#pragma once

// C++ Config Framework includes
#include <CppConfigFramework/ConfigObjectNode.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

//! This class holds the DerivedObject configuration node
class CPPCONFIGFRAMEWORK_EXPORT ConfigDerivedObjectNode : public ConfigNode
{
public:
    /*!
     * Constructor
     *
     * \param   bases   Bases for this configuration node
     * \param   config  Overloads for this configuration node
     * \param   parent  Parent for this configuration node
     */
    ConfigDerivedObjectNode(const QList<ConfigNodePath> &bases = QList<ConfigNodePath>(),
                            const ConfigObjectNode &config = ConfigObjectNode(),
                            ConfigObjectNode *parent = nullptr);

    //! Copy constructor is disabled
    ConfigDerivedObjectNode(const ConfigDerivedObjectNode &) = delete;

    //! Move constructor
    ConfigDerivedObjectNode(ConfigDerivedObjectNode &&) noexcept = default;

    //! Destructor
    ~ConfigDerivedObjectNode() override = default;

    //! Copy assignment operator is disabled
    ConfigDerivedObjectNode &operator=(const ConfigDerivedObjectNode &) = delete;

    //! Move assignment operator
    ConfigDerivedObjectNode &operator=(ConfigDerivedObjectNode &&) noexcept = default;

    //! \copydoc    ConfigNode::clone()
    std::unique_ptr<ConfigNode> clone() const override;

    //! \copydoc    ConfigNode::type()
    Type type() const override;

    /*!
     * Gets the bases for deriving the Object configuration node
     *
     * \return  List of node paths
     */
    QList<ConfigNodePath> bases() const;

    /*!
     * Sets the bases for deriving the Object configuration node
     *
     * \param   bases   New bases
     */
    void setBases(const QList<ConfigNodePath> &bases);

    /*!
     * Gets the overloads for deriving the Object configuration node
     *
     * \return  Object node
     */
    const ConfigObjectNode &config() const;

    /*!
     * Sets the overloads for deriving the Object configuration node
     *
     * \param   config  New overloads
     */
    void setConfig(const ConfigObjectNode &config);

private:
    //! Bases for deriving the Object configuration node
    QList<ConfigNodePath> m_bases;

    //! Overloads (node applied to the base) for deriving the Object configuration node
    ConfigObjectNode m_config;
};

} // namespace CppConfigFramework

// -------------------------------------------------------------------------------------------------

/*!
 * Global "equal to" operator for CppConfigFramework::ConfigDerivedObjectNode
 *
 * \param   left    Node
 * \param   right   Node
 *
 * \retval  true    Nodes are equal
 * \retval  false   Nodes are not equal
 */
CPPCONFIGFRAMEWORK_EXPORT bool operator==(const CppConfigFramework::ConfigDerivedObjectNode &left,
                                          const CppConfigFramework::ConfigDerivedObjectNode &right);

// -------------------------------------------------------------------------------------------------

/*!
 * Global "not equal to" operator for CppConfigFramework::ConfigDerivedObjectNode
 *
 * \param   left    Node
 * \param   right   Node
 *
 * \retval  true    Nodes are not equal
 * \retval  false   Nodes are equal
 */
CPPCONFIGFRAMEWORK_EXPORT bool operator!=(const CppConfigFramework::ConfigDerivedObjectNode &left,
                                          const CppConfigFramework::ConfigDerivedObjectNode &right);

