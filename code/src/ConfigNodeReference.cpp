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
 * Contains a class for the NodeReference configuration node
 */

// Own header
#include <CppConfigFramework/ConfigNodeReference.hpp>

// C++ Config Framework includes

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

ConfigNodeReference::ConfigNodeReference(const ConfigNodePath &reference, ConfigObjectNode *parent)
    : ConfigNode(parent),
      m_reference(reference)
{
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigNodeReference::clone() const
{
    return std::make_unique<ConfigNodeReference>(m_reference, nullptr);
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigNodeReference::type() const
{
    return ConfigNode::Type::NodeReference;
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigNodeReference::toSimplifiedVariant() const
{
    return m_reference.path();
}

// -------------------------------------------------------------------------------------------------

ConfigNodePath ConfigNodeReference::reference() const
{
    return m_reference;
}

// -------------------------------------------------------------------------------------------------

void ConfigNodeReference::setReference(const ConfigNodePath &reference)
{
    m_reference = reference;
}

} // namespace CppConfigFramework

// -------------------------------------------------------------------------------------------------

bool operator==(const CppConfigFramework::ConfigNodeReference &left,
                const CppConfigFramework::ConfigNodeReference &right)
{
    return ((left.nodePath() == right.nodePath()) &&
            (left.reference() == right.reference()));
}

// -------------------------------------------------------------------------------------------------

bool operator!=(const CppConfigFramework::ConfigNodeReference &left,
                const CppConfigFramework::ConfigNodeReference &right)
{
    return !(left == right);
}
