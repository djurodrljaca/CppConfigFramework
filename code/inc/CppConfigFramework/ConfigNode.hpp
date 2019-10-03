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
 * Contains a class for storing a configuration node
 */

#ifndef CPPCONFIGFRAMEWORK_CONFIGNODE_HPP
#define CPPCONFIGFRAMEWORK_CONFIGNODE_HPP

// C++ Config Framework includes
#include <CppConfigFramework/CppConfigFrameworkExport.hpp>
#include <CppConfigFramework/Pimpl.hpp>

// Qt includes
#include <QtCore/QVariant>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

/*!
 * This class holds a configuration node
 */
class CPPCONFIGFRAMEWORK_EXPORT ConfigNode
{
public:
    //! Enumerates different types of nodes
    enum class Type
    {
        Null,   //!< A node with no value
        Value,  //!< A node with a primitive value (Boolean, integer, floating-point number, string)
        Array,  //!< An ordered list of nodes
        Object  /*!< An unordered collection of name–value pairs (names are strings and values are
                     configuration nodes) */
        // TODO: add node refrence ("&name")
        // TODO: add array inheritance ("@name")
        // TODO: add object inheritance ("#name")
    };

    //! Constructor
    ConfigNode(const Type type = Type::Null, ConfigNode *parent = nullptr);

    //! Copy constructor
    ConfigNode(const ConfigNode &other) = delete;

    //! Move constructor
    ConfigNode(ConfigNode &&other) noexcept;

    //! Destructor
    ~ConfigNode();

    //! Copy assignment operator
    ConfigNode &operator=(const ConfigNode &other) = delete;

    //! Move assignment operator
    ConfigNode &operator=(ConfigNode &&other) noexcept;

    //! Clones just the node data (and not the parent!)
    ConfigNode clone() const;

    //! Gets the node's type
    Type type() const;

    //! Converts the Type value to string
    static QString typeToString(const Type type);

    //! Checks if the node is of Null type
    bool isNull() const;

    //! Checks if the node is of Value type
    bool isValue() const;

    //! Checks if the node is of Array type
    bool isArray() const;

    //! Checks if the node is of Object type
    bool isObject() const;

    //! Checks if the node is the root node (has no parent)
    bool isRoot() const;

    //! Gets the parent node (nullptr if this is a root node)
    const ConfigNode *parent() const;

    //! \copydoc    ConfigNode::parent()
    ConfigNode *parent();

    //! Changes the node's parent
    void setParent(ConfigNode *parent);

    //! Gets the root node
    const ConfigNode *rootNode() const;

    //! \copydoc    ConfigNode::rootNode()
    ConfigNode *rootNode();

    //! Gets the absolute node path
    QString absoluteNodePath() const;

    //! Gets the number of elements in the Array node or number of members in the Object node
    int count() const;

    //! Checks if the node contains a member with the specified name
    bool containsMember(const QString &name) const;

    //! Gets the names of the member nodes in the Object node
    QStringList memberNames() const;

    //! Gets the value of the node
    QVariant value() const;

    //! Gets the element at the specified index of the Array node
    const ConfigNode *element(const int index) const;

    //! \copydoc    ConfigNode::element()
    ConfigNode *element(const int index);

    // TODO: add iteration API for array?

    //! Gets the member at the specified index of the Object node
    const ConfigNode *member(const QString &name) const;

    //! \copydoc    ConfigNode::member()
    ConfigNode *member(const QString &name);

    // TODO: add iteration API for object?

    //! Gets the member at the specified node path
    const ConfigNode *nodeAtPath(const QString &nodePath) const;

    //! \copydoc    ConfigNode::nodeAtPath()
    ConfigNode *nodeAtPath(const QString &nodePath);

    //! Sets the node's value
    void setValue(const QVariant &value);

    /*!
     * Sets the value of the element node at the specified index in the Array node
     *
     * \param   index   Index of the element node where the value needs to be stored
     * \param   value   Node value
     *
     * When the node value is stored its parent is updated to point to this node.
     */
    void setElement(const int index, ConfigNode &&value);

    /*!
     * Sets the value of the member node with the specified name in the Object node
     *
     * \param   name    Name of the member node
     * \param   value   Node value
     *
     * If a node with the same name already exists it is replaced with the new node value otherwise
     * it is added as a new member node,
     *
     * When the node value is stored its parent is updated to point to this node.
     */
    void setMember(const QString &name, ConfigNode &&value);

    //! Adds a value to the end of the Array node and updates its parent
    void appendElement(ConfigNode &&value);

    //! Removes a value with the specified index from the Array node
    void removeElement(const int index);

    //! Removes a value with the specified name from the Object node
    void removeMember(const QString &name);

    //! Removes all elements from an Array node or all members from an Object node
    void removeAll();

    /*!
     * Merges the specified Object node with this Object node
     *
     * \param   otherNode   Configuration node to apply
     *
     * \retval  true    Success
     * \retval  false   Failure
     *
     * All items from the other node that are not found in this node are added but items with the
     * same name are applied recursively with values from the new node overwriting the ones from
     * this node.
     *
     * \note    Items with the same name but a different underlying node type cannot be merged!
     */
    bool apply(const ConfigNode &otherNode);

    /*!
     * Validates the configuration node name
     *
     * \param   name    Node name to validate
     *
     * \return  true    Valid node name
     * \return  false   Invalid node name
     *
     * The node name must match the regular expression "^[a-zA-Z][a-zA-Z0-9_]*$" or it must be a
     * decimal integer.
     */
    static bool validateNodeName(const QString &name);

    //! Checks if the node path is an absolute path
    static bool isAbsoluteNodePath(const QString &nodePath);

    //! Checks if the node path is a relative path
    static bool isRelativeNodePath(const QString &nodePath);

    /*!
     * Validates the configuration node path
     *
     * \param   nodePath        Node path to validate
     * \param   currentNodePath Absolute node path to the current node
     *
     * \return  true    Node path is valid
     * \return  false   Node path is not valid
     *
     * \note    Current node path is needed only for relative paths.
     */
    static bool validateNodePath(const QString &nodePath,
                                 const QString &currentNodePath = QString());

    //! Appends a node to the node path
    static QString appendNodeToPath(const QString &nodePath, const QString &nodeName);

private:
    CPPCONFIGFRAMEWORK_DECLARE_PIMPL()
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGNODE_HPP
