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
 * Contains a class for the configuration node path
 */

// Own header
#include <CppConfigFramework/ConfigNodePath.hpp>

// C++ Config Framework includes

// Qt includes
#include <QtCore/QRegularExpression>
#include <QtCore/QStringBuilder>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

// Constants
QString ConfigNodePath::ROOT_PATH_VALUE = QStringLiteral("/");
ConfigNodePath ConfigNodePath::ROOT_PATH = ConfigNodePath(ConfigNodePath::ROOT_PATH_VALUE);

QString ConfigNodePath::PARENT_PATH_VALUE = QStringLiteral("..");
ConfigNodePath ConfigNodePath::PARENT_PATH = ConfigNodePath(ConfigNodePath::PARENT_PATH_VALUE);

static QChar NODE_PATH_SEPARATOR = QChar('/');

// -------------------------------------------------------------------------------------------------

ConfigNodePath::ConfigNodePath(const QString &path)
    : m_path(path)
{
}

// -------------------------------------------------------------------------------------------------

bool ConfigNodePath::operator==(const ConfigNodePath &other) const
{
    return (m_path == other.m_path);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNodePath::operator!=(const ConfigNodePath &other) const
{
    return (m_path != other.m_path);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNodePath::isRoot() const
{
    return (m_path == ROOT_PATH_VALUE);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNodePath::isAbsolute() const
{
    return m_path.startsWith(NODE_PATH_SEPARATOR);
}

// -------------------------------------------------------------------------------------------------

bool ConfigNodePath::isRelative() const
{
    // Check for an empty
    if (m_path.isEmpty())
    {
        // Error, an empty path is not a relative path
        return false;
    }

    return (!isAbsolute());
}

// -------------------------------------------------------------------------------------------------

bool ConfigNodePath::isValid() const
{
    // Check for an empty
    if (m_path.isEmpty())
    {
        // Error, an empty path is not valid
        return false;
    }

    // Check for "root" path
    if (isRoot())
    {
        return true;
    }

    // Use different algorithms for absolute and relative path validation
    if (isAbsolute())
    {
        // Make sure that the individual node names are valid and that there is no attempt to access
        // the parent node of the root node
        QStringList workingNodeNames;

        for (const QString &nodeName : nodeNames())
        {
            if (nodeName == PARENT_PATH_VALUE)
            {
                if (workingNodeNames.isEmpty())
                {
                    // Error, the root node does not have a parent node
                    return false;
                }

                workingNodeNames.removeLast();
                continue;
            }

            if (!validateNodeName(nodeName))
            {
                // Error, invalid node name
                return false;
            }

            workingNodeNames.append(nodeName);
        }
    }
    else
    {
        // Make sure that just the individual node names are valid
        for (const QString &nodeName : nodeNames())
        {
            if ((nodeName != PARENT_PATH_VALUE) && (!validateNodeName(nodeName)))
            {
                return false;
            }
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigNodePath::hasUnresolvedReferences() const
{
    // Check for an empty
    if (m_path.isEmpty())
    {
        // Error, an empty path is not valid
        return false;
    }

    // Check for "root" path
    if (isRoot())
    {
        return false;
    }

    // Check if ".." is the only node, the first node, middle node or the last node
    static const QRegularExpression s_regex(QStringLiteral("(?>^|\\/)\\.\\.(?>\\/|$)"));

    auto match = s_regex.match(m_path);
    return match.hasMatch();
}

// -------------------------------------------------------------------------------------------------

bool ConfigNodePath::resolveReferences()
{
    // Check for unresolved references
    if (!hasUnresolvedReferences())
    {
        // Nothing to do
        return true;
    }

    // Use different algorithms for absolute and relative path validation
    if (isAbsolute())
    {
        // Make sure that the individual node names are valid and that there is no attempt to access
        // the parent node of the root node
        QStringList workingNodeNames;

        for (const QString &nodeName : nodeNames())
        {
            if (nodeName.isEmpty())
            {
                // Error, empty node name is not allowed
                return false;
            }

            if (nodeName == PARENT_PATH_VALUE)
            {
                if (workingNodeNames.isEmpty())
                {
                    // Error, the root node does not have a parent node
                    return false;
                }

                workingNodeNames.removeLast();
                continue;
            }

            workingNodeNames.append(nodeName);
        }

        if (workingNodeNames.isEmpty())
        {
            m_path = ROOT_PATH_VALUE;
        }
        else
        {
            m_path = NODE_PATH_SEPARATOR % workingNodeNames.join(NODE_PATH_SEPARATOR);
        }
    }
    else
    {
        // Make sure that just the individual node names are valid
        QStringList workingNodeNames;

        for (const QString &nodeName : nodeNames())
        {
            if (nodeName.isEmpty())
            {
                // Error, empty node name is not allowed
                return false;
            }

            if (nodeName == PARENT_PATH_VALUE)
            {
                // Only remove the node name if the last node in it is a non-parent node reference
                if ((!workingNodeNames.isEmpty()) && (workingNodeNames.last() != PARENT_PATH_VALUE))
                {
                    workingNodeNames.removeLast();
                    continue;
                }
            }

            workingNodeNames.append(nodeName);
        }

        if (workingNodeNames.isEmpty())
        {
            // Error, empty result
            return false;
        }

        m_path = workingNodeNames.join(NODE_PATH_SEPARATOR);
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

QString ConfigNodePath::path() const
{
    return m_path;
}

// -------------------------------------------------------------------------------------------------

void ConfigNodePath::setPath(const QString &path)
{
    m_path = path;
}

// -------------------------------------------------------------------------------------------------

QStringList ConfigNodePath::nodeNames() const
{
    return (isAbsolute() ? m_path.mid(1).split(NODE_PATH_SEPARATOR)
                         : m_path.split(NODE_PATH_SEPARATOR));
}

// -------------------------------------------------------------------------------------------------

ConfigNodePath ConfigNodePath::toAbsolute(const ConfigNodePath &workingPath) const
{
    // Check if this path is valid
    if (!isValid())
    {
        // Error, this path must be a valid path
        return {};
    }

    // Check if this path is already absolute
    if (isAbsolute())
    {
        // This is an absolute path, just make a copy of this path
        return *this;
    }

    // Validate the working path
    if (workingPath.isRelative() || (!workingPath.isValid()))
    {
        // Error, working path must be a valid absolute path
        return {};
    }

    // Append this path to the working path
    return workingPath.append(*this);
}

// -------------------------------------------------------------------------------------------------

ConfigNodePath ConfigNodePath::append(const QString &nodeName) const
{
    return ConfigNodePath(*this).append(nodeName);
}

// -------------------------------------------------------------------------------------------------

ConfigNodePath &ConfigNodePath::append(const QString &nodeName)
{
    // Make sure that this node path and the specified node name are valid
    if ((!isValid()) || (!validateNodeName(nodeName)))
    {
        // Error, invalidate this path and return it
        m_path.clear();
        return *this;
    }

    // Append the specified node path to this path
    if (isRoot())
    {
        m_path.append(nodeName);
    }
    else
    {
        m_path.append(NODE_PATH_SEPARATOR % nodeName);
    }

    return *this;
}

// -------------------------------------------------------------------------------------------------

ConfigNodePath ConfigNodePath::append(const ConfigNodePath &nodePath) const
{
    return ConfigNodePath(*this).append(nodePath);
}

// -------------------------------------------------------------------------------------------------

ConfigNodePath &ConfigNodePath::append(const ConfigNodePath &nodePath)
{
    // Make sure that both paths are valid and that the specified node path is a relative path
    if ((!isValid()) || nodePath.isAbsolute() || (!nodePath.isValid()))
    {
        // Error, invalidate this path and return it
        m_path.clear();
        return *this;
    }

    // Append the specified node path to this path
    if (isRoot())
    {
        m_path.append(nodePath.path());
    }
    else
    {
        m_path.append(NODE_PATH_SEPARATOR % nodePath.path());
    }

    return *this;
}

// -------------------------------------------------------------------------------------------------

bool ConfigNodePath::validateNodeName(const QString &name)
{
    // Check if the name starts with a letter and continues with an optional string of alphanumeric
    // and "_" characters
    static const QRegularExpression s_regex(QStringLiteral("^[a-zA-Z][a-zA-Z0-9_]*$"));

    auto match = s_regex.match(name);
    return match.hasMatch();
}

} // namespace CppConfigFramework
