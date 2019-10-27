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

// Own header
#include <CppConfigFramework/ConfigNode.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigDerivedObjectNode.hpp>
#include <CppConfigFramework/ConfigNodeReference.hpp>
#include <CppConfigFramework/ConfigObjectNode.hpp>
#include <CppConfigFramework/ConfigValueNode.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

ConfigNode::ConfigNode(ConfigObjectNode *parent)
    : m_parent(parent)
{
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isValue() const
{
    return (type() == Type::Value);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isObject() const
{
    return (type() == Type::Object);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isNodeReference() const
{
    return (type() == Type::NodeReference);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isDerivedObject() const
{
    return (type() == Type::DerivedObject);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isRoot() const
{
    return (parent() == nullptr);
}

// -------------------------------------------------------------------------------------------------

const ConfigObjectNode *ConfigNode::parent() const
{
    return m_parent;
}

// -------------------------------------------------------------------------------------------------

ConfigObjectNode *ConfigNode::parent()
{
    return m_parent;
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::setParent(ConfigObjectNode *parent)
{
    m_parent = parent;
}

// -------------------------------------------------------------------------------------------------

const ConfigObjectNode *ConfigNode::rootNode() const
{
    // Check if this is the root node
    if (isRoot())
    {
        return isObject() ? &toObject()
                          : nullptr;
    }

    // Find the root node
    const ConfigObjectNode *node = parent();

    while (!node->isRoot())
    {
        node = node->parent();
    }

    return node;
}

// -------------------------------------------------------------------------------------------------

ConfigObjectNode *ConfigNode::rootNode()
{
    // Check if this is the root node
    if (isRoot())
    {
        return isObject() ? &toObject()
                          : nullptr;
    }

    // Find the root node
    ConfigObjectNode *node = parent();

    while (!node->isRoot())
    {
        node = node->parent();
    }

    return node;
}

// -------------------------------------------------------------------------------------------------

const ConfigValueNode &ConfigNode::toValue() const
{
    Q_ASSERT(isValue());
    return static_cast<const ConfigValueNode &>(*this);
}

// -------------------------------------------------------------------------------------------------

ConfigValueNode &ConfigNode::toValue()
{
    Q_ASSERT(isValue());
    return static_cast<ConfigValueNode &>(*this);
}

// -------------------------------------------------------------------------------------------------

const ConfigObjectNode &ConfigNode::toObject() const
{
    Q_ASSERT(isObject());
    return static_cast<const ConfigObjectNode &>(*this);
}

// -------------------------------------------------------------------------------------------------

ConfigObjectNode &ConfigNode::toObject()
{
    Q_ASSERT(isObject());
    return static_cast<ConfigObjectNode &>(*this);
}

// -------------------------------------------------------------------------------------------------

const ConfigNodeReference &ConfigNode::toNodeReference() const
{
    Q_ASSERT(isNodeReference());
    return static_cast<const ConfigNodeReference &>(*this);
}

// -------------------------------------------------------------------------------------------------

ConfigNodeReference &ConfigNode::toNodeReference()
{
    Q_ASSERT(isNodeReference());
    return static_cast<ConfigNodeReference &>(*this);
}

// -------------------------------------------------------------------------------------------------

const ConfigDerivedObjectNode &ConfigNode::toDerivedObject() const
{
    Q_ASSERT(isDerivedObject());
    return static_cast<const ConfigDerivedObjectNode &>(*this);
}

// -------------------------------------------------------------------------------------------------

ConfigDerivedObjectNode &ConfigNode::toDerivedObject()
{
    Q_ASSERT(isDerivedObject());
    return static_cast<ConfigDerivedObjectNode &>(*this);
}

// -------------------------------------------------------------------------------------------------

ConfigNodePath ConfigNode::nodePath() const
{
    // Check for root node
    if (isRoot())
    {
        return ConfigNodePath::ROOT_PATH;
    }

    // Get the base path
    auto basePath = parent()->isRoot() ? ConfigNodePath::ROOT_PATH
                                       : parent()->nodePath();

    // Append the name of this node to the path
    return basePath.append(parent()->name(*this));
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigNode::nodeAtPath(const ConfigNodePath &nodePath) const
{
    // Validate node path
    if (!nodePath.isValid())
    {
        return nullptr;
    }

    // Try to find the node at the specified path
    if (nodePath.isRoot())
    {
        return rootNode();
    }

    const ConfigNode *currentNode = nullptr;

    if (nodePath.isAbsolute())
    {
        currentNode = rootNode();

        if (currentNode == nullptr)
        {
            // Error, the root node is not an Object
            return nullptr;
        }
    }
    else
    {
        currentNode = this;
    }

    for (const QString &nodeName : nodePath.nodeNames())
    {
        // Check if parent node is referenced
        if (nodeName == ConfigNodePath::PARENT_PATH_VALUE)
        {
            if (currentNode->isRoot())
            {
                // Error: parent of the root node was requested
                return nullptr;
            }

            currentNode = currentNode->parent();
            continue;
        }

        // Get the specified member node
        if (!currentNode->isObject())
        {
            // Error, invalid node type
            return nullptr;
        }

        currentNode = currentNode->toObject().member(nodeName);

        if (currentNode == nullptr)
        {
            // Error, node was not found
            return nullptr;
        }
    }

    return currentNode;
}

// -------------------------------------------------------------------------------------------------

ConfigNode *ConfigNode::nodeAtPath(const ConfigNodePath &nodePath)
{
    auto constThis = static_cast<const ConfigNode*>(this);
    return const_cast<ConfigNode *>(constThis->nodeAtPath(nodePath));
}

// -------------------------------------------------------------------------------------------------

QString ConfigNode::typeToString(const ConfigNode::Type type)
{
    switch (type)
    {
        case Type::Value:
        {
            return QStringLiteral("Value");
        }

        case Type::Object:
        {
            return QStringLiteral("Object");
        }

        case Type::NodeReference:
        {
            return QStringLiteral("NodeReference");
        }

        case Type::DerivedObject:
        {
            return QStringLiteral("DerivedObject");
        }
    }

    return {};
}

} // namespace CppConfigFramework
