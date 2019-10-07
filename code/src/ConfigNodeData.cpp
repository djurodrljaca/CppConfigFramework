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
#include "ConfigNodeData.hpp"

// C++ Config Framework includes

// Qt includes

// System includes
#include <functional>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

std::size_t StringHashCalculator::operator()(const QString &value) const
{
    return qHash(value);
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNodeData> ConfigNodeData::create(const ConfigNode::Type type)
{
    switch (type)
    {
        case ConfigNode::Type::Null:
        {
            return std::make_unique<ConfigNodeNullData>();
        }

        case ConfigNode::Type::Value:
        {
            return std::make_unique<ConfigNodeValueData>();
        }

        case ConfigNode::Type::Array:
        {
            return std::make_unique<ConfigNodeArrayData>();
        }

        case ConfigNode::Type::Object:
        {
            return std::make_unique<ConfigNodeObjectData>();
        }

        case ConfigNode::Type::NodeReference:
        {
            return std::make_unique<ConfigNodeReferenceData>();
        }

        case ConfigNode::Type::DerivedObject:
        {
            return std::make_unique<ConfigNodeDerivedObjectData>();
        }
    }

    qFatal("Unexpected type");
    return {};
}

// -------------------------------------------------------------------------------------------------

const QVariant *ConfigNodeData::value() const
{
    return nullptr;
}

// -------------------------------------------------------------------------------------------------

QVariant *ConfigNodeData::value()
{
    return nullptr;
}

// -------------------------------------------------------------------------------------------------

const ConfigNodeData::ArrayNodeData *ConfigNodeData::array() const
{
    return nullptr;
}

// -------------------------------------------------------------------------------------------------

ConfigNodeData::ArrayNodeData *ConfigNodeData::array()
{
    return nullptr;
}

// -------------------------------------------------------------------------------------------------

const ConfigNodeData::ObjectNodeData *ConfigNodeData::object() const
{
    return nullptr;
}

// -------------------------------------------------------------------------------------------------

ConfigNodeData::ObjectNodeData *ConfigNodeData::object()
{
    return nullptr;
}

// -------------------------------------------------------------------------------------------------

const QString *ConfigNodeData::nodeReference() const
{
    return nullptr;
}

// -------------------------------------------------------------------------------------------------

QString *ConfigNodeData::nodeReference()
{
    return nullptr;
}

// -------------------------------------------------------------------------------------------------

const DerivedObjectData *ConfigNodeData::derivedObject() const
{
    return nullptr;
}

// -------------------------------------------------------------------------------------------------

DerivedObjectData *ConfigNodeData::derivedObject()
{
    return nullptr;
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigNodeNullData::type() const
{
    return ConfigNode::Type::Null;
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigNodeValueData::type() const
{
    return ConfigNode::Type::Value;
}

// -------------------------------------------------------------------------------------------------

const QVariant *ConfigNodeValueData::value() const
{
    return &m_value;
}

// -------------------------------------------------------------------------------------------------

QVariant *ConfigNodeValueData::value()
{
    return &m_value;
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigNodeArrayData::type() const
{
    return ConfigNode::Type::Array;
}

// -------------------------------------------------------------------------------------------------

const ConfigNodeData::ArrayNodeData *ConfigNodeArrayData::array() const
{
    return &m_array;
}

// -------------------------------------------------------------------------------------------------

ConfigNodeData::ArrayNodeData *ConfigNodeArrayData::array()
{
    return &m_array;
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigNodeObjectData::type() const
{
    return ConfigNode::Type::Object;
}

// -------------------------------------------------------------------------------------------------

const ConfigNodeData::ObjectNodeData *ConfigNodeObjectData::object() const
{
    return &m_object;
}

// -------------------------------------------------------------------------------------------------

ConfigNodeData::ObjectNodeData *ConfigNodeObjectData::object()
{
    return &m_object;
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigNodeReferenceData::type() const
{
    return ConfigNode::Type::NodeReference;
}

// -------------------------------------------------------------------------------------------------

const QString *ConfigNodeReferenceData::nodeReference() const
{
    return &m_nodeReference;
}

// -------------------------------------------------------------------------------------------------

QString *ConfigNodeReferenceData::nodeReference()
{
    return &m_nodeReference;
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigNodeDerivedObjectData::type() const
{
    return ConfigNode::Type::DerivedObject;
}

// -------------------------------------------------------------------------------------------------

const DerivedObjectData *ConfigNodeDerivedObjectData::derivedObject() const
{
    return &m_data;
}

// -------------------------------------------------------------------------------------------------

DerivedObjectData *ConfigNodeDerivedObjectData::derivedObject()
{
    return &m_data;
}

} // namespace CppConfigFramework
