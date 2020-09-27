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
 * Contains a class for the DerivedObject configuration node
 */

// Own header
#include <CppConfigFramework/ConfigDerivedObjectNode.hpp>

// C++ Config Framework includes

// Qt includes
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

ConfigDerivedObjectNode::ConfigDerivedObjectNode(const QList<ConfigNodePath> &bases,
                                                 const ConfigObjectNode &config,
                                                 ConfigObjectNode *parent)
    : ConfigNode(parent),
      m_bases(bases),
      m_config(std::move(config.clone()->toObject()))
{
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigDerivedObjectNode::clone() const
{
    return std::make_unique<ConfigDerivedObjectNode>(m_bases, m_config, nullptr);
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigDerivedObjectNode::type() const
{
    return ConfigNode::Type::DerivedObject;
}

// -------------------------------------------------------------------------------------------------

QList<ConfigNodePath> ConfigDerivedObjectNode::bases() const
{
    return m_bases;
}

// -------------------------------------------------------------------------------------------------

void ConfigDerivedObjectNode::setBases(const QList<ConfigNodePath> &bases)
{
    m_bases = bases;
}

// -------------------------------------------------------------------------------------------------

const ConfigObjectNode &ConfigDerivedObjectNode::config() const
{
    return m_config;
}

// -------------------------------------------------------------------------------------------------

void ConfigDerivedObjectNode::setConfig(const ConfigObjectNode &config)
{
    m_config = std::move(config.clone()->toObject());
}

} // namespace CppConfigFramework

// -------------------------------------------------------------------------------------------------

bool operator==(const CppConfigFramework::ConfigDerivedObjectNode &left,
                const CppConfigFramework::ConfigDerivedObjectNode &right)
{
    return ((left.nodePath() == right.nodePath()) &&
            (left.bases() == right.bases()) &&
            (left.config() == right.config()));
}

// -------------------------------------------------------------------------------------------------

bool operator!=(const CppConfigFramework::ConfigDerivedObjectNode &left,
                const CppConfigFramework::ConfigDerivedObjectNode &right)
{
    return !(left == right);
}
