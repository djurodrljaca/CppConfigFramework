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
 * Contains a class for the Value configuration node
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

//! This class holds the Value configuration node
class CPPCONFIGFRAMEWORK_EXPORT ConfigValueNode : public ConfigNode
{
public:
    /*!
     * Constructor
     *
     * \param   value   Value for this configuration node
     * \param   parent  Parent for this configuration node
     */
    ConfigValueNode(const QJsonValue &value = QJsonValue(), ConfigObjectNode *parent = nullptr);

    //! Copy constructor is disabled
    ConfigValueNode(const ConfigValueNode &) = delete;

    //! Move constructor
    ConfigValueNode(ConfigValueNode &&) noexcept = default;

    //! Destructor
    ~ConfigValueNode() override = default;

    //! Copy assignment operator is disabled
    ConfigValueNode &operator=(const ConfigValueNode &) = delete;

    //! Move assignment operator
    ConfigValueNode &operator=(ConfigValueNode &&) noexcept = default;

    //! \copydoc    ConfigNode::clone()
    std::unique_ptr<ConfigNode> clone() const override;

    //! \copydoc    ConfigNode::type()
    Type type() const override;

    /*!
     * Gets the value of the configuration node
     *
     * \return  Configuration node's value
     */
    QJsonValue value() const;

    /*!
     * Sets the value of the configuration node
     *
     * \param   value   New value
     */
    void setValue(const QJsonValue &value);

private:
    //! Configuration node's value
    QJsonValue m_value;
};

} // namespace CppConfigFramework

// -------------------------------------------------------------------------------------------------

/*!
 * Global "equal to" operator for CppConfigFramework::ConfigValueNode
 *
 * \param   left    Node
 * \param   right   Node
 *
 * \retval  true    Nodes are equal
 * \retval  false   Nodes are not equal
 */
CPPCONFIGFRAMEWORK_EXPORT bool operator==(const CppConfigFramework::ConfigValueNode &left,
                                          const CppConfigFramework::ConfigValueNode &right);

// -------------------------------------------------------------------------------------------------

/*!
 * Global "not equal to" operator for CppConfigFramework::ConfigValueNode
 *
 * \param   left    Node
 * \param   right   Node
 *
 * \retval  true    Nodes are equal
 * \retval  false   Nodes are not equal
 */
CPPCONFIGFRAMEWORK_EXPORT bool operator!=(const CppConfigFramework::ConfigValueNode &left,
                                          const CppConfigFramework::ConfigValueNode &right);

