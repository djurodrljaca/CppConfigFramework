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

// Own header
#include <CppConfigFramework/ConfigValueNode.hpp>

// C++ Config Framework includes

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

ConfigValueNode::ConfigValueNode(const QJsonValue &value, ConfigObjectNode *parent)
    : ConfigNode(parent),
      m_value(value)
{
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigValueNode::clone() const
{
    return std::make_unique<ConfigValueNode>(m_value, nullptr);
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigValueNode::type() const
{
    return ConfigNode::Type::Value;
}

// -------------------------------------------------------------------------------------------------

QJsonValue ConfigValueNode::toJson() const
{
    return m_value;
}

// -------------------------------------------------------------------------------------------------

QJsonValue ConfigValueNode::value() const
{
    return m_value;
}

// -------------------------------------------------------------------------------------------------

void ConfigValueNode::setValue(const QJsonValue &value)
{
    m_value = value;
}

} // namespace CppConfigFramework

// -------------------------------------------------------------------------------------------------

bool operator==(const CppConfigFramework::ConfigValueNode &left,
                const CppConfigFramework::ConfigValueNode &right)
{
    return ((left.nodePath() == right.nodePath()) &&
            (left.value() == right.value()));
}

// -------------------------------------------------------------------------------------------------

bool operator!=(const CppConfigFramework::ConfigValueNode &left,
                const CppConfigFramework::ConfigValueNode &right)
{
    return !(left == right);
}
