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

// Own header
#include "ConfigNodeImpl.hpp"

// C++ Config Framework includes
#include "DebugHelper.hpp"

// Qt includes
#include <QtCore/QStringBuilder>
#include <QtCore/QtDebug>

// System includes

// Forward declarations

// Macros
#define DEBUG_METHOD(METHOD)    CPPCONFIGFRAMEWORK_DEBUG_METHOD("ConfigNode::Impl::" METHOD)

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

ConfigNode::Impl::Impl(const Type type, ConfigNode *parent, ConfigNode *owner)
    : m_data(ConfigNodeData::create(type)),
      m_parent((parent == nullptr) ? nullptr : parent->impl()),
      m_owner(owner)
{
    if (parent != nullptr)
    {
        Q_ASSERT_X((parent->isArray() ||
                    parent->isObject() ||
                    parent->isDerivedArray() ||
                    parent->isDerivedObject()),
                   "ConfigNode::Impl::Impl",
                   "Parent must be either an Array, Object, DerivedArray, or DerivedObject!");
    }

    Q_ASSERT_X((owner != nullptr),
               "ConfigNode::Impl::Impl",
               "Owner class for the private implementation class must to be set!");
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Impl::~Impl()
{
    // This object doesn't own the parent or the public part of the class
    m_parent = nullptr;
    m_owner = nullptr;
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::setOwner(ConfigNode *owner)
{
    Q_ASSERT(owner);
    m_owner = owner;
}

// -------------------------------------------------------------------------------------------------

ConfigNode ConfigNode::Impl::clone() const
{
    // Create a new node with empty data and no parent
    ConfigNode clonedNode(m_data->type(), nullptr);

    // Copy the node data
    switch (m_data->type())
    {
        case Type::Null:
        {
            // No data
            break;
        }

        case Type::Value:
        {
            clonedNode.setValue(value());
            break;
        }

        case Type::Array:
        {
            for (int i = 0; i < count(); i++)
            {
                clonedNode.appendElement(element(i)->clone());
            }
            break;
        }

        case Type::Object:
        {
            for (const QString &name : memberNames())
            {
                clonedNode.setMember(name, member(name)->clone());
            }
            break;
        }

        case Type::NodeReference:
        {
            clonedNode.setNodeReference(nodeReference());
            break;
        }

        case Type::DerivedArray:
        {
            clonedNode.derivedArray()->clear();

            for (const auto &element : *derivedArray())
            {
                clonedNode.derivedArray()->push_back(element.clone());
            }
            break;
        }

        case Type::DerivedObject:
        {
            clonedNode.derivedObject()->setBases(derivedObject()->bases());
            clonedNode.derivedObject()->config() = derivedObject()->config().clone();
            break;
        }
    }

    return clonedNode;
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigNode::Impl::type() const
{
    return m_data->type();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::isNull() const
{
    return (m_data->type() == ConfigNode::Type::Null);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::isValue() const
{
    return (m_data->type() == ConfigNode::Type::Value);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::isArray() const
{
    return (m_data->type() == ConfigNode::Type::Array);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::isObject() const
{
    return (m_data->type() == ConfigNode::Type::Object);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::isNodeReference() const
{
    return (m_data->type() == ConfigNode::Type::NodeReference);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::isDerivedArray() const
{
    return (m_data->type() == ConfigNode::Type::DerivedArray);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::isDerivedObject() const
{
    return (m_data->type() == ConfigNode::Type::DerivedObject);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::isRoot() const
{
    return (m_parent == nullptr);
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigNode::Impl::parent() const
{
    return (m_parent == nullptr) ? nullptr : m_parent->m_owner;
}

// -------------------------------------------------------------------------------------------------

ConfigNode *ConfigNode::Impl::parent()
{
    return (m_parent == nullptr) ? nullptr : m_parent->m_owner;
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::setParent(ConfigNode *parent)
{
    if (parent == nullptr)
    {
        m_parent = nullptr;
    }
    else
    {
        Q_ASSERT_X((parent->isArray() ||
                    parent->isObject() ||
                    parent->isDerivedArray() ||
                    parent->isDerivedObject()),
                   "ConfigNode::Impl::setParent",
                   "Parent must be either an Array, Object, DerivedArray, or DerivedObject!");
        m_parent = parent->impl();
    }

}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigNode::Impl::rootNode() const
{
    if (isRoot())
    {
        return m_owner;
    }

    const ConfigNode *node = parent();

    while (!node->isRoot())
    {
        node = node->parent();
    }

    return node;
}

// -------------------------------------------------------------------------------------------------

ConfigNode *ConfigNode::Impl::rootNode()
{
    auto constThis = static_cast<const Impl*>(this);
    return const_cast<ConfigNode *>(constThis->rootNode());
}

// -------------------------------------------------------------------------------------------------

QString ConfigNode::Impl::absoluteNodePath() const
{
    if (isRoot())
    {
        return QStringLiteral("/");
    }

    if (parent()->isArray())
    {
        if (parent()->isRoot())
        {
            return '/' % QString::number(parent()->impl()->findNodeIndex(*this));
        }

        return parent()->absoluteNodePath() % '/' %
                QString::number(parent()->impl()->findNodeIndex(*this));
    }

    if (parent()->isObject())
    {
        if (parent()->isRoot())
        {
            return '/' % parent()->impl()->findNodeName(*this);
        }

        return parent()->absoluteNodePath() % '/' % parent()->impl()->findNodeName(*this);
    }

    qDebug() << DEBUG_METHOD("absoluteNodePath")
             << "Error: parent is not a container type (Array or Object)";
    return {};
}

// -------------------------------------------------------------------------------------------------

int ConfigNode::Impl::count() const
{
    if (isArray())
    {
        return static_cast<int>(m_data->array()->size());
    }

    if (isObject())
    {
        return static_cast<int>(m_data->object()->size());
    }

    return 0;
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::containsMember(const QString &name) const
{
    if (!isObject())
    {
        qDebug() << DEBUG_METHOD("containsMember") << "Error: config node is not of an Object type";
        return false;
    }

    return (m_data->object()->find(name) != m_data->object()->end());
}

// -------------------------------------------------------------------------------------------------

QStringList ConfigNode::Impl::memberNames() const
{
    if (!isObject())
    {
        qDebug() << DEBUG_METHOD("memberNames") << "Error: config node is not of an Object type";
        return {};
    }

    QStringList nameList;

    for (auto &it : *m_data->object())
    {
        nameList.append(it.first);
    }

    return nameList;
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigNode::Impl::value() const
{
    if (!isValue())
    {
        qDebug() << DEBUG_METHOD("value") << "Error: config node is not of a Value type";
        return {};
    }

    return *m_data->value();
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigNode::Impl::element(const int index) const
{
    if (!isArray())
    {
        qDebug() << DEBUG_METHOD("element") << "Error: config node is not of an Array type";
        return nullptr;
    }

    if ((index < 0) || (index >= count()))
    {
        qDebug() << DEBUG_METHOD("element")
                 << "Error: index out of range: index =" << index << "; count =" << count();
        return nullptr;
    }

    auto it = m_data->array()->begin();
    std::advance(it, index);
    return &(*it);
}

// -------------------------------------------------------------------------------------------------

ConfigNode *ConfigNode::Impl::element(const int index)
{
    auto constThis = static_cast<const Impl*>(this);
    return const_cast<ConfigNode *>(constThis->element(index));
}

// -------------------------------------------------------------------------------------------------

std::vector<const ConfigNode *> ConfigNode::Impl::elements() const
{
    if (!isArray())
    {
        qDebug() << DEBUG_METHOD("elements") << "Error: config node is not of an Array type";
        return {};
    }

    std::vector<const ConfigNode *> items;
    items.reserve(count());

    for (const ConfigNode &node : *m_data->array())
    {
        items.push_back(&node);
    }

    return items;
}

// -------------------------------------------------------------------------------------------------

std::vector<ConfigNode *> ConfigNode::Impl::elements()
{
    if (!isArray())
    {
        qDebug() << DEBUG_METHOD("elements") << "Error: config node is not of an Array type";
        return {};
    }

    std::vector<ConfigNode *> items;
    items.reserve(count());

    for (ConfigNode &node : *m_data->array())
    {
        items.push_back(&node);
    }

    return items;
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigNode::Impl::member(const QString &name) const
{
    if (!isObject())
    {
        qDebug() << DEBUG_METHOD("member") << "Error: config node is not of an Object type";
        return nullptr;
    }

    auto it = m_data->object()->find(name);

    if (it == m_data->object()->end())
    {
        qDebug() << DEBUG_METHOD("member") << "Error: node was not found:" << name;
        return nullptr;
    }

    return &it->second;
}

// -------------------------------------------------------------------------------------------------

ConfigNode *ConfigNode::Impl::member(const QString &name)
{
    auto constThis = static_cast<const Impl*>(this);
    return const_cast<ConfigNode *>(constThis->member(name));
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigNode::Impl::nodeAtPath(const QString &nodePath) const
{
    // Validate node path
    if (!validateNodePath(nodePath, absoluteNodePath()))
    {
        qDebug() << DEBUG_METHOD("nodeAtPath") << "Error: invalid path:" << nodePath;
        return nullptr;
    }

    // Try to find the node at the specified path
    if (nodePath == QStringLiteral("/"))
    {
        return rootNode();
    }

    const ConfigNode *currentNode = nullptr;
    QStringList nodeNames;

    if (isAbsoluteNodePath(nodePath))
    {
        currentNode = rootNode();
        nodeNames = nodePath.mid(1).split(QChar('/'));
    }
    else
    {
        currentNode = m_owner;
        nodeNames = nodePath.split(QChar('/'));
    }

    for (const QString &nodeName : nodeNames)
    {
        // Check if parent node is referenced
        if (nodeName == QStringLiteral(".."))
        {
            if (currentNode->isRoot())
            {
                qDebug() << DEBUG_METHOD("nodeAtPath")
                         << "Error: parent of the root node was request:" << nodePath;
                return nullptr;
            }

            currentNode = currentNode->parent();
            continue;
        }

        // Get the specified member node if this is an Object node
        if (currentNode->isObject())
        {
            if (!currentNode->containsMember(nodeName))
            {
                qDebug() << DEBUG_METHOD("nodeAtPath")
                         << QString("Error: node [%1] from node path [%2] was not found in [%3]")
                            .arg(nodeName, nodePath, absoluteNodePath());
                return nullptr;
            }

            currentNode = currentNode->member(nodeName);
            continue;
        }

        // Get the specified element node if this is an Array node
        if (currentNode->isArray())
        {
            bool ok = false;
            const int index = nodeName.toInt(&ok);

            if (!ok)
            {
                qDebug() << DEBUG_METHOD("nodeAtPath")
                         << QString("Error: node [%1] from node path [%2] was not found in [%3]: "
                                    "invalid node index")
                            .arg(nodeName, nodePath, absoluteNodePath());
                return nullptr;
            }

            if ((index < 0) || (index >= currentNode->count()))
            {
                qDebug() << DEBUG_METHOD("nodeAtPath")
                         << QString("Error: node [%1] from node path [%2] was not found in [%3]: "
                                    "node index our of range")
                            .arg(nodeName, nodePath, absoluteNodePath());
                return nullptr;
            }

            currentNode = currentNode->element(index);
            continue;
        }

        qDebug() << DEBUG_METHOD("nodeAtPath")
                 << QString("Error: cannot get node [%1] from node path [%2] with type [%3]")
                    .arg(nodeName, nodePath, typeToString(currentNode->type()));
        return nullptr;
    }

    return currentNode;
}

// -------------------------------------------------------------------------------------------------

ConfigNode *ConfigNode::Impl::nodeAtPath(const QString &nodePath)
{
    auto constThis = static_cast<const Impl*>(this);
    return const_cast<ConfigNode *>(constThis->nodeAtPath(nodePath));
}

// -------------------------------------------------------------------------------------------------

QString ConfigNode::Impl::nodeReference() const
{
    return *m_data->nodeReference();
}

// -------------------------------------------------------------------------------------------------

const std::list<ConfigNode> *ConfigNode::Impl::derivedArray() const
{
    return m_data->derivedArray();
}

// -------------------------------------------------------------------------------------------------

std::list<ConfigNode> *ConfigNode::Impl::derivedArray()
{
    return m_data->derivedArray();
}

// -------------------------------------------------------------------------------------------------

const DerivedObjectData *ConfigNode::Impl::derivedObject() const
{
    return m_data->derivedObject();
}

// -------------------------------------------------------------------------------------------------

DerivedObjectData *ConfigNode::Impl::derivedObject()
{
    return m_data->derivedObject();
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::setValue(const QVariant &value)
{
    if (!isValue())
    {
        qDebug() << DEBUG_METHOD("setValue") << "Config node is not of a Value type";
        return;
    }

    *m_data->value() = value;
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::setElement(const int index, ConfigNode &&value)
{
    if (!isArray())
    {
        qDebug() << DEBUG_METHOD("setElement") << "Config node is not of an Array type";
        return;
    }

    if ((index < 0) || (index >= count()))
    {
        qDebug() << DEBUG_METHOD("setElement")
                 << "Error: index out of range: index =" << index << "; count =" << count();
        return;
    }

    auto it = m_data->array()->begin();
    std::advance(it, index);
    *it = std::move(value);
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::appendElement(ConfigNode &&value)
{
    if (!isArray())
    {
        qDebug() << DEBUG_METHOD("appendElement") << "Config node is not of an Array type";
        return;
    }

    value.setParent(m_owner);
    m_data->array()->push_back(std::move(value));
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::setMember(const QString &name, ConfigNode &&value)
{
    if (!isObject())
    {
        qDebug() << DEBUG_METHOD("setMember") << "Error: config node is not of an Object type";
        return;
    }

    if (name.isEmpty())
    {
        qDebug() << DEBUG_METHOD("setMember") << "Error: name must not be empty!";
        return;
    }

    value.setParent(m_owner);
    auto it = m_data->object()->find(name);

    if (it == m_data->object()->end())
    {
        // Insert a new item
        m_data->object()->emplace(name, std::move(value));
    }
    else
    {
        // Replace the exising item
        it->second = std::move(value);
    }
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::applyObject(const ConfigNode &otherNode)
{
    if (!isObject())
    {
        qDebug() << DEBUG_METHOD("applyObject")
                 << "Error: this config node is not of an Object type";
        return false;
    }

    if (!otherNode.isObject())
    {
        qDebug() << DEBUG_METHOD("applyObject")
                 << "Error: other config node is not of an Object type";
        return false;
    }

    // Merge nodes
    for (const QString &name : otherNode.memberNames())
    {
        // Check if a member with the same name already exists
        const ConfigNode *itemFromOtherNode = otherNode.member(name);

        if (!containsMember(name))
        {
            // A member with the same name doesn't exist, copy the item and add it to this node as
            // a new member (even if Null)
            setMember(name, itemFromOtherNode->clone());
            continue;
        }

        if (itemFromOtherNode->isNull())
        {
            // Nothing to merge, just skip the node
            continue;
        }

        // Apply other node's item to this node
        ConfigNode *itemFromThisNode = member(name);

        if (itemFromThisNode->isObject() && itemFromOtherNode->isObject())
        {
            // Merge object items
            if (!itemFromThisNode->applyObject(*itemFromOtherNode))
            {
                return false;
            }
        }
        else
        {
            // Overwrite the this node's value for non-Object types
            *itemFromThisNode = itemFromOtherNode->clone();
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::setNodeReference(const QString &nodePath)
{
    if (!isNodeReference())
    {
        qDebug() << DEBUG_METHOD("setNodeReference")
                 << "Config node is not of a NodeReference type";
        return;
    }

    *m_data->nodeReference() = nodePath;
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::removeElement(const int index)
{
    if (!isArray())
    {
        qDebug() << DEBUG_METHOD("removeElement") << "Config node is not of an Array type";
        return;
    }

    if ((index < 0) || (index >= count()))
    {
        qDebug() << DEBUG_METHOD("removeElement")
                 << "Error: index out of range: index =" << index << "; count =" << count();
        return;
    }

    auto it = m_data->array()->begin();
    std::advance(it, index);
    m_data->array()->erase(it);
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::removeMember(const QString &name)
{
    if (!isObject())
    {
        qDebug() << DEBUG_METHOD("removeMember") << "Error: config node is not of an Object type";
        return;
    }

    auto it = m_data->object()->find(name);

    if (it == m_data->object()->end())
    {
        qDebug() << DEBUG_METHOD("removeMember") << "Error: config node was not found:" << name;
        return;
    }

    m_data->object()->erase(it);
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::removeAll()
{
    if (isArray())
    {
        m_data->array()->clear();
    }
    else if (isObject())
    {
        m_data->object()->clear();
    }
    else
    {
        qDebug() << DEBUG_METHOD("removeAll") << "Config node is not of an Array or Object type";
    }
}

// -------------------------------------------------------------------------------------------------

int ConfigNode::Impl::findNodeIndex(const Impl &value) const
{
    int index = 0;

    for (auto it = m_data->array()->begin(); it != m_data->array()->end(); it++, index++)
    {
        if (it->impl() == &value)
        {
            return index;
        }
    }

    return -1;
}

// -------------------------------------------------------------------------------------------------

QString ConfigNode::Impl::findNodeName(const Impl &value) const
{
    for (auto &it : *m_data->object())
    {
        const Impl &item = *it.second.impl();

        if (&item == &value)
        {
            return it.first;
        }
    }

    return {};
}

} // namespace CppConfigFramework
