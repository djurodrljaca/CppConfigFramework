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

#pragma once

// C++ Config Framework includes
#include <CppConfigFramework/ConfigObjectNode.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

namespace ConfigWriter
{

/*!
 * Writes the Object node to the C++ Config Framework JSON format
 *
 * \param   node    Configuration node
 *
 * \return  JSON document
 */
CPPCONFIGFRAMEWORK_EXPORT QJsonDocument writeToJsonConfig(const ConfigObjectNode &node);

// -------------------------------------------------------------------------------------------------

/*!
 * Writes the Object node to the specified JSON config file
 *
 * \param   node        Configuration node
 * \param   filePath    Path to the output JSON config file
 *
 * \retval  true    Success
 * \retval  false   Failure
 */
CPPCONFIGFRAMEWORK_EXPORT bool writeToJsonConfigFile(const ConfigObjectNode &node,
                                                     const QString &filePath);

// -------------------------------------------------------------------------------------------------

/*!
 * Converts the Object node (with fully resolved references) to a JSON value
 *
 * \param   node    Configuration node
 *
 * \return  JSON object wrapped in a JSON value or "undefined" in case of failure
 *
 * This function produces a valid output only when sub-nodes of this Object node contain only Object
 * and Value nodes. The output will be a direct representation of the whole data structure without
 * any C++ Config Framework syntax (no "decorator" prefixes in the member names).
 */
CPPCONFIGFRAMEWORK_EXPORT QJsonValue convertToJsonValue(const ConfigObjectNode &node);

} // namespace ConfigWriter

} // namespace CppConfigFramework
