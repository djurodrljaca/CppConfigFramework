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

#ifndef CPPCONFIGFRAMEWORK_CONFIGVALUENODE_HPP
#define CPPCONFIGFRAMEWORK_CONFIGVALUENODE_HPP

// C++ Config Framework includes
#include <CppConfigFramework/ConfigNode.hpp>

// Qt includes
#include <QtCore/QVariant>

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
    ConfigValueNode(const QVariant &value = QVariant(), ConfigObjectNode *parent = nullptr);

    //! Copy constructor is disabled
    ConfigValueNode(const ConfigValueNode &other) = delete;

    /*!
     * Move constructor
     *
     * \param   other   Instance to move
     */
    ConfigValueNode(ConfigValueNode &&other) noexcept = default;

    //! Destructor
    ~ConfigValueNode() override = default;

    //! Copy assignment operator is disabled
    ConfigValueNode &operator=(const ConfigValueNode &other) = delete;

    /*!
     * Move assignment operator
     *
     * \param   other   Instance to move assign
     *
     * \return  Reference to this instance after the assignment is made
     */
    ConfigValueNode &operator=(ConfigValueNode &&other) noexcept = default;

    //! \copydoc    ConfigNode::clone()
    std::unique_ptr<ConfigNode> clone() const override;

    //! \copydoc    ConfigNode::type()
    Type type() const override;

    /*!
     * Gets the value of the configuration node
     *
     * \return  Configuration node's value
     */
    QVariant value() const;

    /*!
     * Sets the value of the configuration node
     *
     * \param   value   New value
     */
    void setValue(const QVariant &value);

private:
    //! Configuration node's value
    QVariant m_value;
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGVALUENODE_HPP
