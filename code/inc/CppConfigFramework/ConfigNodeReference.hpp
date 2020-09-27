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
 * Contains a class for the NodeReference configuration node
 */

#pragma once

// C++ Config Framework includes
#include <CppConfigFramework/ConfigNode.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

//! This class holds the NodeReference configuration node
class CPPCONFIGFRAMEWORK_EXPORT ConfigNodeReference : public ConfigNode
{
public:
    /*!
     * Constructor
     *
     * \param   reference   Reference for another configuration node
     * \param   parent      Parent for this configuration node
     */
    ConfigNodeReference(const ConfigNodePath &reference = ConfigNodePath(),
                        ConfigObjectNode *parent = nullptr);

    //! Copy constructor is disabled
    ConfigNodeReference(const ConfigNodeReference &) = delete;

    //! Move constructor
    ConfigNodeReference(ConfigNodeReference &&) noexcept = default;

    //! Destructor
    ~ConfigNodeReference() override = default;

    //! Copy assignment operator is disabled
    ConfigNodeReference &operator=(const ConfigNodeReference &) = delete;

    //! Move assignment operator
    ConfigNodeReference &operator=(ConfigNodeReference &&) noexcept = default;

    //! \copydoc    ConfigNode::clone()
    std::unique_ptr<ConfigNode> clone() const override;

    //! \copydoc    ConfigNode::type()
    Type type() const override;

    //! \copydoc    ConfigNode::toJson()
    QJsonValue toJson() const override;

    /*!
     * Gets the reference to a configuration node
     *
     * \return  Configuration node path
     */
    ConfigNodePath reference() const;

    /*!
     * Sets the reference to a configuration node
     *
     * \param   reference   New reference
     */
    void setReference(const ConfigNodePath &reference);

private:
    //! Reference to a configuration node
    ConfigNodePath m_reference;
};

} // namespace CppConfigFramework

// -------------------------------------------------------------------------------------------------

/*!
 * Global "equal to" operator for CppConfigFramework::ConfigNodeReference
 *
 * \param   left    Node
 * \param   right   Node
 *
 * \retval  true    Nodes are equal
 * \retval  false   Nodes are not equal
 */
CPPCONFIGFRAMEWORK_EXPORT bool operator==(const CppConfigFramework::ConfigNodeReference &left,
                                          const CppConfigFramework::ConfigNodeReference &right);

// -------------------------------------------------------------------------------------------------

/*!
 * Global "not equal to" operator for CppConfigFramework::ConfigNodeReference
 *
 * \param   left    Node
 * \param   right   Node
 *
 * \retval  true    Nodes are equal
 * \retval  false   Nodes are not equal
 */
CPPCONFIGFRAMEWORK_EXPORT bool operator!=(const CppConfigFramework::ConfigNodeReference &left,
                                          const CppConfigFramework::ConfigNodeReference &right);

