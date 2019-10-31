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
 * Contains a class for the Object configuration node
 */

#ifndef CPPCONFIGFRAMEWORK_CONFIGOBJECTNODE_HPP
#define CPPCONFIGFRAMEWORK_CONFIGOBJECTNODE_HPP

// C++ Config Framework includes
#include <CppConfigFramework/ConfigNode.hpp>
#include <CppConfigFramework/HashFunctions.hpp>

// Qt includes

// System includes
#include <unordered_map>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

//! This class holds the Object configuration node
class CPPCONFIGFRAMEWORK_EXPORT ConfigObjectNode : public ConfigNode
{
public:
    /*!
     * Constructor
     *
     * \param   value   Value for this configuration node
     * \param   parent  Parent for this configuration node
     */
    ConfigObjectNode(ConfigObjectNode *parent = nullptr);

    //! Copy constructor is disabled
    ConfigObjectNode(const ConfigObjectNode &other) = delete;

    /*!
     * Move constructor
     *
     * \param   other   Instance to move
     */
    ConfigObjectNode(ConfigObjectNode &&other) noexcept;

    //! Destructor
    ~ConfigObjectNode() override = default;

    //! Copy assignment operator is disabled
    ConfigObjectNode &operator=(const ConfigObjectNode &other) = delete;

    /*!
     * Move assignment operator
     *
     * \param   other   Instance to move assign
     *
     * \return  Reference to this instance after the assignment is made
     */
    ConfigObjectNode &operator=(ConfigObjectNode &&other) noexcept;

    //! \copydoc    ConfigNode::clone()
    std::unique_ptr<ConfigNode> clone() const override;

    //! \copydoc    ConfigNode::type()
    Type type() const override;

    /*!
     * Gets the number of members in this node
     *
     * \return  Number of members
     */
    int count() const;

    /*!
     * Checks if the node contains a member with the specified name
     *
     * \param   name    Name of the member node
     *
     * \retval  true    Member with the specified name was found
     * \retval  false   Member with the specified name was not found
     */
    bool contains(const QString &name) const;

    /*!
     * Gets the names of all member nodes in this node
     *
     * \return  Member names
     */
    QStringList names() const;

    /*!
     * Gets the name of the specified node
     *
     * \param   node    Configuration node to find
     *
     * \return  Name of the configuration node or an empty string if the member was not found
     */
    QString name(const ConfigNode &node) const;

    /*!
     * Gets the member with the specified name
     *
     * \param   name    Name of the member node
     *
     * \return  Configuration node or nullptr if the member was not found
     */
    const ConfigNode *member(const QString &name) const;

    //! \copydoc    ConfigObjectNode::member()
    ConfigNode *member(const QString &name);

    /*!
     * Inserts a new member node or replaces an existing member node with the same name
     *
     * \param   name    Name of the member node
     * \param   node    Member node value
     *
     * \retval  true    Success
     * \retval  false   Failure
     *
     * \note    When the node is stored its parent is updated to point to this node
     */
    bool setMember(const QString &name, std::unique_ptr<ConfigNode> &&node);

    //! \copydoc    ConfigObjectNode::setMember()
    bool setMember(const QString &name, const ConfigNode &node);

    /*!
     * Removes a member with the specified name
     *
     * \param   name    Name of the member node
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool remove(const QString &name);

    //! Removes all members
    void removeAll();

    /*!
     * Applies values from the specified node to the matching nodes in this node
     *
     * \param   other   Configuration node to apply
     *
     * All items from the other node that are not found in this node are added but items with the
     * same name are applied recursively with values from the new node overwriting the ones from
     * this node.
     */
    void apply(const ConfigObjectNode &other);

private:
    //! Configuration node members
    std::unordered_map<QString, std::unique_ptr<ConfigNode>> m_members;
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGOBJECTNODE_HPP
