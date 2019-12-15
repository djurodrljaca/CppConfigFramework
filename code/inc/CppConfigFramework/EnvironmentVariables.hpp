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
 * Contains a class for accessing environment variables
 */

#ifndef CPPCONFIGFRAMEWORK_ENVIRONMENTVARIABLES_HPP
#define CPPCONFIGFRAMEWORK_ENVIRONMENTVARIABLES_HPP

// C++ Config Framework includes
#include <CppConfigFramework/CppConfigFrameworkExport.hpp>

// Qt includes
#include <QtCore/QHash>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

/*!
 * This class gives access to system and local environment variables. It can be used for accessing
 * the environment variable values and for expanding environment variable references in a string.
 *
 * System environment variables are read when an instance of this class is created and they are used
 * as the initial environment variables.
 *
 * \note    If a system environment variable is updated after that then that change will not be
 *          applied to an already constructed instance of this class!
 *
 * If an attempt is made to set an environment variable that does not exist then a new variable is
 * created.
 */
class CPPCONFIGFRAMEWORK_EXPORT EnvironmentVariables
{
public:
    /*!
     * Loads environment variables from the current process
     */
    static EnvironmentVariables loadFromProcess();

    /*!
     * Gets the names of all stored environment variables
     *
     * \return  List of environment variable names
     */
    QStringList names() const;

    /*!
     * Checks if an environment variable with the specified name can be found
     *
     * \param   name    Environment variable name
     *
     * \retval  true    Found
     * \retval  false   Not found
     */
    bool contains(const QString &name) const;

    /*!
     * Gets the environment variable value
     *
     * \param   name    Environment variable name
     *
     * \return  Environment variable value or an empty string if the variable cannot be found
     */
    QString value(const QString &name) const;

    /*!
     * Sets the environment variable value
     *
     * \param   name    Environment variable name
     * \param   value   Value to set
     */
    void setValue(const QString &name, const QString &value);

    /*!
     * Expands all references to environment variables in the text
     *
     * \param   text    Text to expand
     *
     * \return  Expanded text or an empty string if all references to environment variables were not
     *          expanded
     */
    QString expandText(const QString &text) const;

private:
    //! Holds the local environment variables
    QHash<QString, QString> m_variables;
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_ENVIRONMENTVARIABLES_HPP
