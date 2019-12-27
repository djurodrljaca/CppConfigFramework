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
 * Contains methods for writing configuration files
 */

#ifndef CPPCONFIGFRAMEWORK_CONFIGWRITER_HPP
#define CPPCONFIGFRAMEWORK_CONFIGWRITER_HPP

// C++ Config Framework includes
#include <CppConfigFramework/ConfigObjectNode.hpp>

// Qt includes
#include <QtCore/QVariant>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

namespace ConfigWriter
{

/*!
 * Writes the Object node to the specified file
 *
 * \param   node        Configuration node
 *
 * \return  JSON document
 */
CPPCONFIGFRAMEWORK_EXPORT QJsonDocument writeToJson(const ConfigObjectNode &node);

/*!
 * Writes the Object node to the specified file
 *
 * \param   node        Configuration node
 * \param   filePath    Path to the output file
 *
 * \retval  true    Success
 * \retval  false   Failure
 */
CPPCONFIGFRAMEWORK_EXPORT bool writeToFile(const ConfigObjectNode &node, const QString &filePath);

} // namespace ConfigWriter

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGWRITER_HPP
