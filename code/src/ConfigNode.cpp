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
#include "ConfigNodeImpl.hpp"
#include "DebugHelper.hpp"

// Qt includes
#include <QtCore/QRegularExpression>
#include <QtCore/QStringBuilder>
#include <QtCore/QtDebug>

// System includes

// Forward declarations

// Macros
#define DEBUG_METHOD(METHOD)    CPPCONFIGFRAMEWORK_DEBUG_METHOD("ConfigNode::" METHOD)

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

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

bool ConfigNode::isNodeReference() const
{
    return impl()->isNodeReference();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::isDerivedObject() const
{
    return impl()->isDerivedObject();
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

std::vector<const ConfigNode *> ConfigNode::elements() const
{
    return impl()->elements();
}

// -------------------------------------------------------------------------------------------------

std::vector<ConfigNode *> ConfigNode::elements()
{
    return impl()->elements();
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

QString ConfigNode::nodeReference() const
{
    return impl()->nodeReference();
}

// -------------------------------------------------------------------------------------------------

const DerivedObjectData *ConfigNode::derivedObject() const
{
    return impl()->derivedObject();
}

// -------------------------------------------------------------------------------------------------

DerivedObjectData *ConfigNode::derivedObject()
{
    return impl()->derivedObject();
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

void ConfigNode::appendElement(ConfigNode &&value)
{
    return impl()->appendElement(std::move(value));
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::setMember(const QString &name, ConfigNode &&value)
{
    return impl()->setMember(name, std::move(value));
}

// -------------------------------------------------------------------------------------------------

bool ConfigNode::applyObject(const ConfigNode &otherNode)
{
    return impl()->applyObject(otherNode);
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::setNodeReference(const QString &nodePath)
{
    impl()->setNodeReference(nodePath);
}

// -------------------------------------------------------------------------------------------------

void ConfigNode::setDerivedObject(const QStringList &bases, ConfigNode &&config)
{
    impl()->setDerivedObject(bases, std::move(config));
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
    // Check for an empty
    if (nodePath.isEmpty())
    {
        qDebug() << DEBUG_METHOD("validateNodePath")
                 << "Error: an empty path is not valid";
        return false;
    }

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

        if (currentNodePath == QStringLiteral("/"))
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

    if (nodePath.endsWith(QChar('/')))
    {
        return nodePath % nodeName;
    }

    return nodePath % QChar('/') % nodeName;
}

} // namespace CppConfigFramework
