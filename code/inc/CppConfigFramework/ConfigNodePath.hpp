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

#ifndef CPPCONFIGFRAMEWORK_CONFIGNODEPATH_HPP
#define CPPCONFIGFRAMEWORK_CONFIGNODEPATH_HPP

// C++ Config Framework includes
#include <CppConfigFramework/CppConfigFrameworkExport.hpp>

// Qt includes
#include <QtCore/QStringList>

// System includes
#include <memory>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

//! This class holds the configuration node path
class CPPCONFIGFRAMEWORK_EXPORT ConfigNodePath
{
public:
    //! Constructor
    ConfigNodePath() = default;

    /*!
     * Constructor
     *
     * \param   path    Node path
     */
    ConfigNodePath(const QString &path);

    //! Copy constructor
    ConfigNodePath(const ConfigNodePath &) = default;

    //! Move constructor
    ConfigNodePath(ConfigNodePath &&) noexcept = default;

    //! Destructor
    ~ConfigNodePath() = default;

    //! Copy assignment operator
    ConfigNodePath &operator=(const ConfigNodePath &) = default;

    //! Move assignment operator
    ConfigNodePath &operator=(ConfigNodePath &&) noexcept = default;

    /*!
     * Checks if this is the root node path
     *
     * \retval  true    This is the root node path
     * \retval  false   This is not the root node path
     *
     * \note    Node path value "/" represents the root node path
     */
    bool isRoot() const;

    /*!
     * Checks if the node path is an absolute path
     *
     * \retval  true    This is an absolute node path
     * \retval  false   This is not an absolute node path
     *
     * \note    An absolute node path is either the root node or it starts with "/"
     * \note    Validity of the node path is not checked!
     */
    bool isAbsolute() const;

    /*!
     * Checks if the node path is a relative path
     *
     * \retval  true    This is a relative node path
     * \retval  false   This is not a relative node path
     *
     * \note    All valid non-absolute and non-empty node paths are relative
     * \note    Validity of the node path is not checked!
     */
    bool isRelative() const;

    /*! Checks if this is this path is valid
     *
     * \retval  true    Node path is valid
     * \retval  false   Node path is invalid
     */
    bool isValid() const;

    /*!
     * Checks if the node path has unresolved references to parent nodes ("..")
     *
     * \retval  true    This node path has unresolved references
     * \retval  false   This node path does not have unresolved references
     */
    bool hasUnresolvedReferences() const;

    /*!
     * Tries to resolve all resolvable references to parent nodes ("..")
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool resolveReferences();

    /*!
     * Gets the node path's value
     *
     * \return  Node path's value
     */
    QString path() const;

    /*!
     * Sets the node path's value
     *
     * \param   path    New node path value
     */
    void setPath(const QString &path);

    /*!
     * Splits the node path to individual node names
     *
     * \return  Node names
     */
    QStringList nodeNames() const;

    /*!
     * Creates a new path from this path and (if needed) the working path
     *
     * \param   workingPath     An absolute path used as the base path if this is a relative path
     *
     * \return  Absolute node path
     */
    ConfigNodePath toAbsolute(const ConfigNodePath &workingPath) const;

    /*!
     * Appends a node to this node path
     *
     * \param   nodeName    Name of the node to append
     *
     * \return  Node path
     */
    ConfigNodePath append(const QString &nodeName) const;

    /*!
     * Appends a node to this node path
     *
     * \param   nodeName    Name of the node to append
     *
     * \return  Reference to this node path instance
     */
    ConfigNodePath &append(const QString &nodeName);

    /*!
     * Appends a relative node path to this node path
     *
     * \param   nodePath    Node path to append
     *
     * \return  Node path
     */
    ConfigNodePath append(const ConfigNodePath &nodePath) const;

    /*!
     * Appends a relative node path to this node path
     *
     * \param   nodePath    Node path to append
     *
     * \return  Reference to this node path instance
     */
    ConfigNodePath &append(const ConfigNodePath &nodePath);

    /*!
     * Validates the configuration node name
     *
     * \param   name    Node name to validate
     *
     * \return  true    Valid node name
     * \return  false   Invalid node name
     *
     * The node name must match the regular expression "^[a-zA-Z][a-zA-Z0-9_]*$".
     */
    static bool validateNodeName(const QString &name);

public:
    //! Root node path value
    static QString ROOT_PATH_VALUE;

    //! Root node path
    static ConfigNodePath ROOT_PATH;

    //! Parent node path value
    static QString PARENT_PATH_VALUE;

    //! Parent node path
    static ConfigNodePath PARENT_PATH;

private:
    //! Node path's value
    QString m_path;
};

} // namespace CppConfigFramework

/*!
 * Global "equal to" operator for CppConfigFramework::ConfigNodePath
 *
 * \param   left    Node path
 * \param   right   Node path
 *
 * \retval  true    Node paths are equal
 * \retval  false   Node paths are not equal
 */
CPPCONFIGFRAMEWORK_EXPORT bool operator==(const CppConfigFramework::ConfigNodePath &left,
                                          const CppConfigFramework::ConfigNodePath &right);

/*!
 * Global "not equal to" operator for CppConfigFramework::ConfigNodePath
 *
 * \param   left    Node path
 * \param   right   Node path
 *
 * \retval  true    Node paths are not equal
 * \retval  false   Node paths are equal
 */
CPPCONFIGFRAMEWORK_EXPORT bool operator!=(const CppConfigFramework::ConfigNodePath &left,
                                          const CppConfigFramework::ConfigNodePath &right);

#endif // CPPCONFIGFRAMEWORK_CONFIGNODEPATH_HPP
