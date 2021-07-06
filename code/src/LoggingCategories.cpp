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
 * Contains logging categories
 */

// Own header
#include <CppConfigFramework/LoggingCategories.hpp>

// C++ Config Framework includes

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

namespace LoggingCategory
{

const QLoggingCategory ConfigItem("CppConfigFramework.ConfigItem");
const QLoggingCategory ConfigNodePath("CppConfigFramework.ConfigNodePath");
const QLoggingCategory ConfigParameterValidator("CppConfigFramework.ConfigParameterValidator");
const QLoggingCategory ConfigReader("CppConfigFramework.ConfigReader");
const QLoggingCategory ConfigWriter("CppConfigFramework.ConfigWriter");

} // namespace LoggingCategory

} // namespace CppConfigFramework
