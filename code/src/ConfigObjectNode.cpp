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

// Own header
#include <CppConfigFramework/ConfigObjectNode.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigDerivedObjectNode.hpp>
#include <CppConfigFramework/ConfigNodeReference.hpp>
#include <CppConfigFramework/ConfigValueNode.hpp>

// Qt includes
#include <QtCore/QJsonObject>
#include <QtCore/QStringBuilder>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

ConfigObjectNode::ConfigObjectNode(ConfigObjectNode *parent)
    : ConfigNode(parent)
{
}

// -------------------------------------------------------------------------------------------------

ConfigObjectNode::ConfigObjectNode(std::initializer_list<std::pair<QString, ConfigNode &&>> args)
    : ConfigNode(nullptr)
{
    for (auto it = args.begin(); it != args.end(); it++)
    {
        setMember(it->first, it->second);
    }
}

// -------------------------------------------------------------------------------------------------

ConfigObjectNode::ConfigObjectNode(ConfigObjectNode &&other) noexcept
    : ConfigNode(other.parent()),
      m_members(std::move(other.m_members))
{
    for (const auto &member : m_members)
    {
        member.second->setParent(this);
    }
}

// -------------------------------------------------------------------------------------------------

ConfigObjectNode &ConfigObjectNode::operator=(ConfigObjectNode &&other) noexcept
{
    if (&other == this)
    {
        return *this;
    }

    setParent(other.parent());
    m_members = std::move(other.m_members);

    for (const auto &member : m_members)
    {
        member.second->setParent(this);
    }

    return *this;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigObjectNode::clone() const
{
    auto clonedNode = std::make_unique<ConfigObjectNode>(nullptr);

    for (const auto &member : m_members)
    {
        clonedNode->setMember(member.first, member.second->clone());
    }

    return clonedNode;
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigObjectNode::type() const
{
    return ConfigNode::Type::Object;
}

// -------------------------------------------------------------------------------------------------

int ConfigObjectNode::count() const
{
    return static_cast<int>(m_members.size());
}

// -------------------------------------------------------------------------------------------------

bool ConfigObjectNode::contains(const QString &name) const
{
    return (m_members.find(name) != m_members.end());
}

// -------------------------------------------------------------------------------------------------

QStringList ConfigObjectNode::names() const
{
    QStringList nameList;

    for (auto &it : m_members)
    {
        nameList.append(it.first);
    }

    return nameList;
}

// -------------------------------------------------------------------------------------------------

QString ConfigObjectNode::name(const ConfigNode &node) const
{
    for (auto &it : m_members)
    {
        if (it.second.get() == &node)
        {
            return it.first;
        }
    }

    return {};
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigObjectNode::member(const QString &name) const
{
    auto it = m_members.find(name);

    if (it == m_members.end())
    {
        return nullptr;
    }

    return it->second.get();
}

// -------------------------------------------------------------------------------------------------

ConfigNode *ConfigObjectNode::member(const QString &name)
{
    auto it = m_members.find(name);

    if (it == m_members.end())
    {
        return nullptr;
    }

    return it->second.get();
}

// -------------------------------------------------------------------------------------------------

bool ConfigObjectNode::setMember(const QString &name, std::unique_ptr<ConfigNode> &&node)
{
    // Make sure that name and node are both valid
    if ((!ConfigNodePath::validateNodeName(name)) || (!node))
    {
        return false;
    }

    // Set the parent to this node
    node->setParent(this);

    // Insert or replace the member
    auto it = m_members.find(name);

    if (it == m_members.end())
    {
        // Insert a new item
        m_members.emplace(name, std::move(node));
    }
    else
    {
        // Replace the existing item
        it->second = std::move(node);
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigObjectNode::setMember(const QString &name, const ConfigNode &node)
{
    return setMember(name, node.clone());
}

// -------------------------------------------------------------------------------------------------

bool ConfigObjectNode::remove(const QString &name)
{
    auto it = m_members.find(name);

    if (it == m_members.end())
    {
        return false;
    }

    m_members.erase(it);
    return true;
}

// -------------------------------------------------------------------------------------------------

void ConfigObjectNode::removeAll()
{
    m_members.clear();
}

// -------------------------------------------------------------------------------------------------

void ConfigObjectNode::apply(const ConfigObjectNode &other)
{
    // Merge nodes
    for (const QString &name : other.names())
    {
        const ConfigNode *memberOther = other.member(name);
        Q_ASSERT(memberOther != nullptr);

        // Check if a member with the same name already exists
        ConfigNode *memberThis = member(name);

        if (memberThis == nullptr)
        {
            // A member with the same name doesn't exist, copy the item and add it to this node as
            // a new member
            setMember(name, *memberOther);
            continue;
        }

        // Apply other node's item to this node
        if (memberThis->isValue() && memberOther->isValue())
        {
            // Overwrite this node's value with the other node's value
            memberThis->toValue().setValue(memberOther->toValue().value());
        }
        else if (memberThis->isObject() && memberOther->isObject())
        {
            // Merge object items
            memberThis->toObject().apply(memberOther->toObject());
        }
        else
        {
            // For all other type combinations just overwrite this node's member with the other
            // node's member
            setMember(name, *memberOther);
        }
    }
}

} // namespace CppConfigFramework

// -------------------------------------------------------------------------------------------------

bool operator==(const CppConfigFramework::ConfigObjectNode &left,
                const CppConfigFramework::ConfigObjectNode &right)
{
    if ((left.nodePath() != right.nodePath()) ||
        (left.count() != right.count()))
    {
        return false;
    }

    for (const QString &name : left.names())
    {
        const auto *leftMemberNode = left.member(name);
        const auto *rightMemberNode = right.member(name);

        if (rightMemberNode == nullptr)
        {
            // Member with the specified name was not found
            return false;
        }

        if (leftMemberNode->type() != rightMemberNode->type())
        {
            return false;
        }

        switch (leftMemberNode->type())
        {
            case CppConfigFramework::ConfigNode::Type::Value:
            {
                if (leftMemberNode->toValue() != rightMemberNode->toValue())
                {
                    return false;
                }
                break;
            }

            case CppConfigFramework::ConfigNode::Type::Object:
            {
                if (leftMemberNode->toObject() != rightMemberNode->toObject())
                {
                    return false;
                }
                break;
            }

            case CppConfigFramework::ConfigNode::Type::NodeReference:
            {
                if (leftMemberNode->toNodeReference() != rightMemberNode->toNodeReference())
                {
                    return false;
                }
                break;
            }

            case CppConfigFramework::ConfigNode::Type::DerivedObject:
            {
                if (leftMemberNode->toDerivedObject() != rightMemberNode->toDerivedObject())
                {
                    return false;
                }
                break;
            }

            default:
            {
                return false;
            }
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool operator!=(const CppConfigFramework::ConfigObjectNode &left,
                const CppConfigFramework::ConfigObjectNode &right)
{
    return !(left == right);
}
