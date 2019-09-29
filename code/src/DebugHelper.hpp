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
 * Contains helper macros for debugging
 */

#ifndef CPPCONFIGFRAMEWORK_DEBUGHELPER_HPP
#define CPPCONFIGFRAMEWORK_DEBUGHELPER_HPP

// C++ Config Framework includes

// Qt includes

// System includes

// Forward declarations

// Macros
#define CPPCONFIGFRAMEWORK_STRINGIFY(PARAM)     #PARAM
#define CPPCONFIGFRAMEWORK_TO_STRING(PARAM)     CPPCONFIGFRAMEWORK_STRINGIFY(PARAM)
#define CPPCONFIGFRAMEWORK_DEBUG_METHOD(METHOD) "CppConfigFramework::" METHOD ":" CPPCONFIGFRAMEWORK_TO_STRING(__LINE__) ":"

#endif // CPPCONFIGFRAMEWORK_DEBUGHELPER_HPP
