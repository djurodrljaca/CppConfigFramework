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
 * Contains a private implementation for ConfigNode class
 */

#ifndef CPPCONFIGFRAMEWORK_CONFIGNODEIMPL_HPP
#define CPPCONFIGFRAMEWORK_CONFIGNODEIMPL_HPP

// C++ Config Framework includes
#include <CppConfigFramework/ConfigNode.hpp>
#include "ConfigNodeData.hpp"

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

/*!
 * Private implementation for ConfigNode class
 */
class ConfigNode::Impl
{
public:
    //! Constructor
    Impl() = delete;

    //! Constructor
    Impl(const Type type, ConfigNode *parent, ConfigNode *owner);

    //! Copy constructor
    Impl(const Impl &other) = delete;

    //! Move constructor
    Impl(Impl &&other) = delete;

    //! Destructor
    ~Impl();

    //! Copy assignment operator
    Impl &operator=(const Impl &other) = delete;

    //! Move assignment operator
    Impl &operator=(Impl &&other) = delete;

    //! Changes the owner of this implementation
    void setOwner(ConfigNode *owner);

    //! \copydoc    ConfigNode::clone()
    ConfigNode clone() const;

    //! \copydoc    ConfigNode::type()
    Type type() const;

    //! \copydoc    ConfigNode::isNull()
    bool isNull() const;

    //! \copydoc    ConfigNode::isValue()
    bool isValue() const;

    //! \copydoc    ConfigNode::isArray()
    bool isArray() const;

    //! \copydoc    ConfigNode::isObject()
    bool isObject() const;

    //! \copydoc    ConfigNode::isNodeReference()
    bool isNodeReference() const;

    //! \copydoc    ConfigNode::isRoot()
    bool isRoot() const;

    //! \copydoc    ConfigNode::parent()
    const ConfigNode *parent() const;

    //! \copydoc    ConfigNode::parent()
    ConfigNode *parent();

    //! \copydoc    ConfigNode::setParent()
    void setParent(ConfigNode *parent);

    //! \copydoc    ConfigNode::rootNode()
    const ConfigNode *rootNode() const;

    //! \copydoc    ConfigNode::rootNode()
    ConfigNode *rootNode();

    //! \copydoc    ConfigNode::absoluteNodePath()
    QString absoluteNodePath() const;

    //! \copydoc    ConfigNode::count()
    int count() const;

    //! \copydoc    ConfigNode::containsMember()
    bool containsMember(const QString &name) const;

    //! \copydoc    ConfigNode::memberNames()
    QStringList memberNames() const;

    //! \copydoc    ConfigNode::value()
    QVariant value() const;

    //! \copydoc    ConfigNode::element()
    const ConfigNode *element(const int index) const;

    //! \copydoc    ConfigNode::element()
    ConfigNode *element(const int index);

    //! \copydoc    ConfigNode::member()
    const ConfigNode *member(const QString &name) const;

    //! \copydoc    ConfigNode::member()
    ConfigNode *member(const QString &name);

    //! \copydoc    ConfigNode::nodeAtPath()
    const ConfigNode *nodeAtPath(const QString &nodePath) const;

    //! \copydoc    ConfigNode::nodeAtPath()
    ConfigNode *nodeAtPath(const QString &nodePath);

    //! \copydoc    ConfigNode::nodeReference()
    QString nodeReference() const;

    //! \copydoc    ConfigNode::setValue()
    void setValue(const QVariant &value);

    //! \copydoc    ConfigNode::setElement()
    void setElement(const int index, ConfigNode &&value);

    //! \copydoc    ConfigNode::appendElement()
    void appendElement(ConfigNode &&value);

    //! \copydoc    ConfigNode::setMember()
    void setMember(const QString &name, ConfigNode &&value);

    //! \copydoc    ConfigNode::applyObject()
    bool applyObject(const ConfigNode &otherNode);

    //! \copydoc    ConfigNode::setNodeReference()
    void setNodeReference(const QString &nodePath);

    //! \copydoc    ConfigNode::removeElement()
    void removeElement(const int index);

    //! \copydoc    ConfigNode::removeMember()
    void removeMember(const QString &name);

    //! \copydoc    ConfigNode::removeAll()
    void removeAll();

private:
    //! Gets the index of the specifed node
    int findNodeIndex(const Impl &value) const;

    //! Gets the name of the specifed node
    QString findNodeName(const Impl &value) const;

    //! Container for node data
    std::unique_ptr<ConfigNodeData> m_data;

    //! Pointer to the parent node's PIMPL
    Impl *m_parent;

    //! Pointer to the public part of the class
    ConfigNode *m_owner;
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGNODEIMPL_HPP
