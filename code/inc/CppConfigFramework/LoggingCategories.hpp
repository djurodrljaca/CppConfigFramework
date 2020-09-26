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

#pragma once

// C++ Config Framework includes
#include <CppConfigFramework/CppConfigFrameworkExport.hpp>

// Qt includes
#include <QtCore/QLoggingCategory>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

namespace LoggingCategory
{

//! Logging category for ConfigLoader
CPPCONFIGFRAMEWORK_EXPORT extern const QLoggingCategory ConfigLoader;

//! Logging category for ConfigNodePath
CPPCONFIGFRAMEWORK_EXPORT extern const QLoggingCategory ConfigNodePath;

//! Logging category for ConfigParameterValidator
CPPCONFIGFRAMEWORK_EXPORT extern const QLoggingCategory ConfigParameterValidator;

//! Logging category for ConfigReader
CPPCONFIGFRAMEWORK_EXPORT extern const QLoggingCategory ConfigReader;

//! Logging category for ConfigWriter
CPPCONFIGFRAMEWORK_EXPORT extern const QLoggingCategory ConfigWriter;

} // namespace LoggingCategory

} // namespace CppConfigFramework
