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
 * Contains a class for the configuration node
 */

#pragma once

// C++ Config Framework includes
#include <CppConfigFramework/ConfigNodePath.hpp>

// Qt includes
#include <QtCore/QJsonValue>

// System includes

// Forward declarations
namespace CppConfigFramework
{

class ConfigDerivedObjectNode;
class ConfigNodeReference;
class ConfigObjectNode;
class ConfigValueNode;

}

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

//! This class holds the configuration node
class CPPCONFIGFRAMEWORK_EXPORT ConfigNode
{
public:
    //! Enumerates different types of nodes
    enum class Type
    {
        //! A node with a primitive value (Boolean, integer, floating-point number, string)
        Value,

        //! A collection of name–value pairs (names are strings and values are configuration nodes)
        Object,

        //! A reference to a node to be copied
        NodeReference,

        //! An object derived from other objects
        DerivedObject
    };

public:
    /*!
     * Constructor
     *
     * \param   parent  Parent for this configuration node
     */
    ConfigNode(ConfigObjectNode *parent);

    //! Copy constructor is disabled
    ConfigNode(const ConfigNode &) = delete;

    //! Move constructor
    ConfigNode(ConfigNode &&) noexcept = default;

    //! Destructor
    virtual ~ConfigNode() = default;

    //! Copy assignment operator is disabled
    ConfigNode &operator=(const ConfigNode &) = delete;

    //! Move assignment operator
    ConfigNode &operator=(ConfigNode &&) noexcept = default;

    /*!
     * Clones just the configuration node contents and not the parent
     *
     * \return  Cloned configuration node
     */
    virtual std::unique_ptr<ConfigNode> clone() const = 0;

    /*!
     * Gets the configuration node's type
     *
     * \return  Configuration node's type
     */
    virtual Type type() const = 0;

    /*!
     * Checks if the configuration node is of Value type
     *
     * \retval  true    This is a Value configuration node
     * \retval  false   This is not a Value configuration node
     */
    bool isValue() const;

    /*!
     * Checks if the configuration node is of Object type
     *
     * \retval  true    This is a Object configuration node
     * \retval  false   This is not a Object configuration node
     */
    bool isObject() const;

    /*!
     * Checks if the configuration node is of NodeReference type
     *
     * \retval  true    This is a NodeReference configuration node
     * \retval  false   This is not a NodeReference configuration node
     */
    bool isNodeReference() const;

    /*!
     * Checks if the configuration node is of DerivedObject type
     *
     * \retval  true    This is a DerivedObject configuration node
     * \retval  false   This is not a DerivedObject configuration node
     */
    bool isDerivedObject() const;

    /*!
     * Checks if the configuration node is the root node (has no parent)
     *
     * \retval  true    This is a DerivedObject configuration node
     * \retval  false   This is not a DerivedObject configuration node
     */
    bool isRoot() const;

    /*!
     * Gets the parent node (nullptr if this is a root node)
     *
     * \return  Parent of this configuration node
     */
    const ConfigObjectNode *parent() const;

    //! \copydoc    ConfigNode::parent()
    ConfigObjectNode *parent();

    /*!
     * Sets the parent of this configuration node
     *
     * \param   parent  New parent of this configuration node
     */
    void setParent(ConfigObjectNode *parent);

    /*!
     * Gets the root node
     *
     * \return  Root configuration node
     */
    const ConfigObjectNode *rootNode() const;

    //! \copydoc    ConfigNode::rootNode()
    ConfigObjectNode *rootNode();

    /*!
     * Converts this configuration node to the Value configuration node
     *
     * \return  Value configuration node
     *
     * \note    This method asserts if the configuration node is not of a Value type!
     */
    const ConfigValueNode &toValue() const;

    //! \copydoc    ConfigNode::toValue()
    ConfigValueNode &toValue();

    /*!
     * Converts this configuration node to the Object configuration node
     *
     * \return  Object configuration node
     *
     * \note    This method asserts if the configuration node is not of a Object type!
     */
    const ConfigObjectNode &toObject() const;

    //! \copydoc    ConfigNode::toObject()
    ConfigObjectNode &toObject();

    /*!
     * Converts this configuration node to the NodeReference configuration node
     *
     * \return  NodeReference configuration node
     *
     * \note    This method asserts if the configuration node is not of a NodeReference type!
     */
    const ConfigNodeReference &toNodeReference() const;

    //! \copydoc    ConfigNode::toNodeReference()
    ConfigNodeReference &toNodeReference();

    /*!
     * Converts this configuration node to the DerivedObject configuration node
     *
     * \return  DerivedObject configuration node
     *
     * \note    This method asserts if the configuration node is not of a DerivedObject type!
     */
    const ConfigDerivedObjectNode &toDerivedObject() const;

    //! \copydoc    ConfigNode::toDerivedObject()
    ConfigDerivedObjectNode &toDerivedObject();

    /*!
     * Gets the absolute node path of this configuration node
     *
     * \return  Node path
     */
    ConfigNodePath nodePath() const;

    /*!
     * Gets the node at the specified node path
     *
     * \param   nodePath    Node Path
     *
     * \return  Node at specified node path or null if node was not found
     */
    const ConfigNode *nodeAtPath(const ConfigNodePath &nodePath) const;

    //! \copydoc    ConfigNode::nodeAtPath()
    const ConfigNode *nodeAtPath(const QString &nodePath) const;

    //! \copydoc    ConfigNode::nodeAtPath()
    ConfigNode *nodeAtPath(const ConfigNodePath &nodePath);

    //! \copydoc    ConfigNode::nodeAtPath()
    ConfigNode *nodeAtPath(const QString &nodePath);

    /*!
     * Converts the Type value to string
     *
     * \param   type    Configuration node type
     *
     * \return  String representation of the type
     */
    static QString typeToString(const Type type);

private:
    //! Holds a reference to the parent of this node or null if this is a root node
    ConfigObjectNode *m_parent;
};

} // namespace CppConfigFramework
