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

// Own header
#include <CppConfigFramework/ConfigNode.hpp>

// C++ Config Framework includes
#include "ConfigNodeData.hpp"
#include "DebugHelper.hpp"

// Qt includes
#include <QtCore/QRegularExpression>
#include <QtCore/QStringBuilder>
#include <QtCore/QtDebug>

// System includes
#include <unordered_map>
#include <functional>

// Forward declarations

// Macros
#define DEBUG_METHOD(METHOD)      CPPCONFIGFRAMEWORK_DEBUG_METHOD("ConfigNode::" METHOD)
#define DEBUG_METHOD_IMPL(METHOD) CPPCONFIGFRAMEWORK_DEBUG_METHOD("ConfigNode::Impl::" METHOD)

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

// -------------------------------------------------------------------------------------------------
// Implementation
// -------------------------------------------------------------------------------------------------

// TODO: move implementation to its own class!

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

    //! \copydoc    ConfigNode::setValue()
    void setValue(const QVariant &value);

    //! \copydoc    ConfigNode::setElement()
    void setElement(const int index, ConfigNode &&value);

    //! \copydoc    ConfigNode::setMember()
    void setMember(const QString &name, ConfigNode &&value);

    //! \copydoc    ConfigNode::appendElement()
    void appendElement(ConfigNode &&value);

    //! \copydoc    ConfigNode::removeElement()
    void removeElement(const int index);

    //! \copydoc    ConfigNode::removeMember()
    void removeMember(const QString &name);

    //! \copydoc    ConfigNode::removeAll()
    void removeAll();

    //! \copydoc    ConfigNode::apply()
    bool apply(const ConfigNode &otherNode);

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

// -------------------------------------------------------------------------------------------------

ConfigNode::Impl::Impl(const Type type, ConfigNode *parent, ConfigNode *owner)
    : m_data(ConfigNodeData::create(type)),
      m_parent((parent == nullptr) ? nullptr : parent->impl()),
      m_owner(owner)
{
    if (parent != nullptr)
    {
        Q_ASSERT_X((parent->isArray() || parent->isObject()),
                   "ConfigNode::Impl::Impl",
                   "Parent must be either an Array or Object!");
    }

    Q_ASSERT_X((owner != nullptr),
               "ConfigNode::Impl::Impl",
               "Owner class for an implementation class must to be set!");
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
    return m_data->isNull();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::isValue() const
{
    return m_data->isValue();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::isArray() const
{
    return m_data->isArray();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::isObject() const
{
    return m_data->isObject();
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
        Q_ASSERT_X((parent->isArray() || parent->isObject()),
                   "ConfigNode::Impl::setParent",
                   "Parent must be either an Array or Object!");
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
        else
        {
            return parent()->absoluteNodePath() % '/' %
                    QString::number(parent()->impl()->findNodeIndex(*this));
        }
    }
    else if (parent()->isObject())
    {
        if (parent()->isRoot())
        {
            return '/' % parent()->impl()->findNodeName(*this);
        }
        else
        {
            return parent()->absoluteNodePath() % '/' % parent()->impl()->findNodeName(*this);
        }
    }
    else
    {
        qDebug() << DEBUG_METHOD_IMPL("absoluteNodePath")
                 << "Error: parent is not a container type (Array or Object)";
        return {};
    }
}

// -------------------------------------------------------------------------------------------------

int ConfigNode::Impl::count() const
{
    if (isArray())
    {
        return static_cast<int>(m_data->array()->size());
    }
    else if (isObject())
    {
        return static_cast<int>(m_data->object()->size());
    }
    else
    {
        return 0;
    }
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::containsMember(const QString &name) const
{
    if (!isObject())
    {
        qDebug() << DEBUG_METHOD_IMPL("containsMember")
                 << "Error: config node is not of an Object type";
        return false;
    }

    return (m_data->object()->find(name) != m_data->object()->end());
}

// -------------------------------------------------------------------------------------------------

QStringList ConfigNode::Impl::memberNames() const
{
    if (!isObject())
    {
        qDebug() << DEBUG_METHOD_IMPL("memberNames")
                 << "Error: config node is not of an Object type";
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
        qDebug() << DEBUG_METHOD_IMPL("value") << "Error: config node is not of a Value type";
        return {};
    }

    return *m_data->value();
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigNode::Impl::element(const int index) const
{
    if (!isArray())
    {
        qDebug() << DEBUG_METHOD_IMPL("element") << "Error: config node is not of an Array type";
        return nullptr;
    }

    if ((index < 0) || (index >= count()))
    {
        qDebug() << DEBUG_METHOD_IMPL("element")
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

const ConfigNode *ConfigNode::Impl::member(const QString &name) const
{
    if (!isObject())
    {
        qDebug() << DEBUG_METHOD_IMPL("member") << "Error: config node is not of an Object type";
        return nullptr;
    }

    auto it = m_data->object()->find(name);

    if (it == m_data->object()->end())
    {
        qDebug() << DEBUG_METHOD_IMPL("member") << "Error: node was not found:" << name;
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
        qDebug() << DEBUG_METHOD_IMPL("nodeAtPath") << "Error: invalid path:" << nodePath;
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
                qDebug() << DEBUG_METHOD_IMPL("nodeAtPath")
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
                qDebug() << DEBUG_METHOD_IMPL("nodeAtPath")
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
                qDebug() << DEBUG_METHOD_IMPL("nodeAtPath")
                         << QString("Error: node [%1] from node path [%2] was not found in [%3]: "
                                    "invalid node index")
                            .arg(nodeName, nodePath, absoluteNodePath());
                return nullptr;
            }

            if ((index < 0) || (index >= currentNode->count()))
            {
                qDebug() << DEBUG_METHOD_IMPL("nodeAtPath")
                         << QString("Error: node [%1] from node path [%2] was not found in [%3]: "
                                    "node index our of range")
                            .arg(nodeName, nodePath, absoluteNodePath());
                return nullptr;
            }

            currentNode = currentNode->element(index);
            continue;
        }

        qDebug() << DEBUG_METHOD_IMPL("nodeAtPath")
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

void ConfigNode::Impl::setValue(const QVariant &value)
{
    if (!isValue())
    {
        qDebug() << DEBUG_METHOD_IMPL("setValue") << "Config node is not of a Value type";
        return;
    }

    *m_data->value() = value;
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::setElement(const int index, ConfigNode &&value)
{
    if (!isArray())
    {
        qDebug() << DEBUG_METHOD_IMPL("setElement") << "Config node is not of an Array type";
        return;
    }

    if ((index < 0) || (index >= count()))
    {
        qDebug() << DEBUG_METHOD_IMPL("setElement")
                 << "Error: index out of range: index =" << index << "; count =" << count();
        return;
    }

    auto it = m_data->array()->begin();
    std::advance(it, index);
    *it = std::move(value);
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::setMember(const QString &name, ConfigNode &&value)
{
    if (!isObject())
    {
        qDebug() << DEBUG_METHOD_IMPL("setMember") << "Error: config node is not of an Object type";
        return;
    }

    if (name.isEmpty())
    {
        qDebug() << DEBUG_METHOD_IMPL("setMember") << "Error: name must not be empty!";
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

void ConfigNode::Impl::appendElement(ConfigNode &&value)
{
    if (!isArray())
    {
        qDebug() << DEBUG_METHOD_IMPL("appendElement") << "Config node is not of an Array type";
        return;
    }

    value.setParent(m_owner);
    m_data->array()->push_back(std::move(value));
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::Impl::removeElement(const int index)
{
    if (!isArray())
    {
        qDebug() << DEBUG_METHOD_IMPL("removeElement") << "Config node is not of an Array type";
        return;
    }

    if ((index < 0) || (index >= count()))
    {
        qDebug() << DEBUG_METHOD_IMPL("removeElement")
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
        qDebug() << DEBUG_METHOD_IMPL("removeMember")
                 << "Error: config node is not of an Object type";
        return;
    }

    auto it = m_data->object()->find(name);

    if (it == m_data->object()->end())
    {
        qDebug() << DEBUG_METHOD_IMPL("removeMember")
                 << "Error: config node was not found:" << name;
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
        qDebug() << DEBUG_METHOD_IMPL("removeAll")
                 << "Config node is not of an Array or Object type";
    }
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::Impl::apply(const ConfigNode &otherNode)
{
    if (!isObject())
    {
        qDebug() << DEBUG_METHOD_IMPL("apply")
                 << "Error: this config node is not of an Object type";
        return false;
    }

    if (!otherNode.isObject())
    {
        qDebug() << DEBUG_METHOD_IMPL("apply")
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

        // Merge other node's item to this node
        ConfigNode *itemFromThisNode = member(name);

        if (itemFromThisNode->isNull())
        {
            // Anything can overwrite a Null node
            *itemFromThisNode = itemFromOtherNode->clone();
            continue;
        }

        // TODO: handle reference types?

        if (itemFromThisNode->type() != itemFromOtherNode->type())
        {
            qDebug() << DEBUG_METHOD_IMPL("apply")
                     << QString("Error: cannot apply node [%1] with type [%2] to "
                                "node [%3] with type [%4]!")
                        .arg(itemFromOtherNode->absoluteNodePath(),
                             typeToString(itemFromOtherNode->type()),
                             itemFromThisNode->absoluteNodePath(),
                             typeToString(itemFromThisNode->type()));
            return false;
        }

        if (itemFromThisNode->isObject())
        {
            // Merge object items
            if (!itemFromThisNode->apply(*itemFromOtherNode))
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

// -------------------------------------------------------------------------------------------------
// API
// -------------------------------------------------------------------------------------------------

ConfigNode::ConfigNode(const ConfigNode::Type type, ConfigNode *parent)
    : m_pimpl(std::make_unique<Impl>(type, parent, this))
{
}

// -------------------------------------------------------------------------------------------------

ConfigNode::ConfigNode(ConfigNode &&other) noexcept
    : m_pimpl(std::move(other.m_pimpl))
{
    // Take ownership of the moved implementation instance
    m_pimpl->setOwner(this);
}

// -------------------------------------------------------------------------------------------------

ConfigNode::~ConfigNode() = default;

// -------------------------------------------------------------------------------------------------

ConfigNode &ConfigNode::operator=(ConfigNode &&other) noexcept
{
    if (&other == this)
    {
        return *this;
    }

    m_pimpl = std::move(other.m_pimpl);
    m_pimpl->setOwner(this);
    return *this;
}

// -------------------------------------------------------------------------------------------------

ConfigNode ConfigNode::clone() const
{
    return impl()->clone();
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigNode::type() const
{
    return impl()->type();
}

// -------------------------------------------------------------------------------------------------

QString ConfigNode::typeToString(const ConfigNode::Type type)
{
    switch (type)
    {
        case Type::Null:
        {
            return QStringLiteral("Null");
        }

        case Type::Value:
        {
            return QStringLiteral("Value");
        }

        case Type::Array:
        {
            return QStringLiteral("Array");
        }

        case Type::Object:
        {
            return QStringLiteral("Object");
        }
    }

    return {};
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isNull() const
{
    return impl()->isNull();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isValue() const
{
    return impl()->isValue();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isArray() const
{
    return impl()->isArray();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isObject() const
{
    return impl()->isObject();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isRoot() const
{
    return impl()->isRoot();
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigNode::parent() const
{
    return impl()->parent();
}

// -------------------------------------------------------------------------------------------------

ConfigNode *ConfigNode::parent()
{
    return impl()->parent();
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::setParent(ConfigNode *parent)
{
    impl()->setParent(parent);
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigNode::rootNode() const
{
    return impl()->rootNode();
}

// -------------------------------------------------------------------------------------------------

ConfigNode *ConfigNode::rootNode()
{
    return impl()->rootNode();
}

// -------------------------------------------------------------------------------------------------

QString ConfigNode::absoluteNodePath() const
{
    return impl()->absoluteNodePath();
}

// -------------------------------------------------------------------------------------------------

int ConfigNode::count() const
{
    return impl()->count();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::containsMember(const QString &name) const
{
    return impl()->containsMember(name);
}

// -------------------------------------------------------------------------------------------------

QStringList ConfigNode::memberNames() const
{
    return impl()->memberNames();
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigNode::value() const
{
    return impl()->value();
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigNode::element(const int index) const
{
    return impl()->element(index);
}

// -------------------------------------------------------------------------------------------------

ConfigNode *ConfigNode::element(const int index)
{
    return impl()->element(index);
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigNode::member(const QString &name) const
{
    return impl()->member(name);
}

// -------------------------------------------------------------------------------------------------

ConfigNode *ConfigNode::member(const QString &name)
{
    return impl()->member(name);
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigNode::nodeAtPath(const QString &nodePath) const
{
    return impl()->nodeAtPath(nodePath);
}

// -------------------------------------------------------------------------------------------------

ConfigNode *ConfigNode::nodeAtPath(const QString &nodePath)
{
    return impl()->nodeAtPath(nodePath);
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::setValue(const QVariant &value)
{
    return impl()->setValue(value);
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::setElement(const int index, ConfigNode &&value)
{
    return impl()->setElement(index, std::move(value));
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::setMember(const QString &name, ConfigNode &&value)
{
    return impl()->setMember(name, std::move(value));
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::appendElement(ConfigNode &&value)
{
    return impl()->appendElement(std::move(value));
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::removeElement(const int index)
{
    return impl()->removeElement(index);
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::removeMember(const QString &name)
{
    return impl()->removeMember(name);
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::removeAll()
{
    return impl()->removeAll();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::apply(const ConfigNode &otherNode)
{
    return impl()->apply(otherNode);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::validateNodeName(const QString &name)
{
    const QRegularExpression regex(QStringLiteral("^[a-zA-Z][a-zA-Z0-9_]*$|^\\d+$"));
    auto match = regex.match(name);

    return match.hasMatch();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isAbsoluteNodePath(const QString &nodePath)
{
    return nodePath.startsWith(QChar('/'));
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isRelativeNodePath(const QString &nodePath)
{
    return (!isAbsoluteNodePath(nodePath));
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::validateNodePath(const QString &nodePath, const QString &currentNodePath)
{
    // Check for "root" path
    if (nodePath == QStringLiteral("/"))
    {
        return true;
    }

    // Prepend current node path if needed
    QString fullNodePath;

    if (isAbsoluteNodePath(nodePath))
    {
        fullNodePath = nodePath;
    }
    else
    {
        if (!isAbsoluteNodePath(currentNodePath))
        {
            qDebug() << DEBUG_METHOD("validateNodePath")
                     << QString("Error: current node path [%1] must be an absolute node path "
                                "because node path [%2] is also not an absolute path")
                        .arg(currentNodePath, nodePath);
            return false;
        }

        if (nodePath.isEmpty())
        {
            fullNodePath = currentNodePath;
        }
        else if (currentNodePath == QStringLiteral("/"))
        {
            fullNodePath = QChar('/') % nodePath;
        }
        else
        {
            fullNodePath = currentNodePath % QChar('/') % nodePath;
        }
    }

    // Split the node path to individual nodes (without the leading '/')
    const QStringList nodeNames = fullNodePath.mid(1).split(QChar('/'));
    QStringList workingNodeNames;

    for (const QString &nodeName : nodeNames)
    {
        if (nodeName == QStringLiteral(".."))
        {
            if (workingNodeNames.isEmpty())
            {
                qDebug() << DEBUG_METHOD("validateNodePath")
                         << "Error: invalid access to the parent node of the root node in node "
                            "path:" << fullNodePath;
                return false;
            }

            workingNodeNames.removeLast();
            continue;
        }

        if (!validateNodeName(nodeName))
        {
            qDebug() << DEBUG_METHOD("validateNodePath")
                     << QString("Error: invalid node name [%1] in node path [%2]")
                        .arg(nodeName, fullNodePath);
            return false;
        }

        workingNodeNames.append(nodeName);
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

QString ConfigNode::appendNodeToPath(const QString &nodePath, const QString &nodeName)
{
    if (nodePath.isEmpty())
    {
        return nodeName;
    }
    else if (nodePath.endsWith(QChar('/')))
    {
        return nodePath % nodeName;
    }
    else
    {
        return nodePath % QChar('/') % nodeName;
    }
}

} // namespace CppConfigFramework
