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
 * Contains a class for the Value configuration node
 */

// Own header
#include "TestClasses.hpp"

// C++ Config Framework includes

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

bool TestRequiredConfigParameter::loadConfigParameters(const ConfigObjectNode &config,
                                                       QString *error)
{
    return loadRequiredConfigParameter(&param, "param", config, error);
}

// -------------------------------------------------------------------------------------------------

bool TestOptionalConfigParameter::loadConfigParameters(const ConfigObjectNode &config,
                                                       QString *error)
{
    return loadOptionalConfigParameter(&param, "param", config, error);
}

} // namespace CppConfigFramework
