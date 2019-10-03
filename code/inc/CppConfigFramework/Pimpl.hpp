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
 * A convenience template class for PIMPL (pointer to implementation) with const propagation
 */

#ifndef CPPCONFIGFRAMEWORK_PIMPLWRAPPER_HPP
#define CPPCONFIGFRAMEWORK_PIMPLWRAPPER_HPP

// C++ Config Framework includes

// Qt includes

// System includes
#include <memory>

// Forward declarations

// Macros

/*!
 * Convenience macro for creating a PIMPL class
 *
 * It forward declares the PIMPL class type, declares the PIMPL container (unique pointer), and
 * creates the impl() methods to be used for accessing the PIMPL pointer.
 */
#define CPPCONFIGFRAMEWORK_DECLARE_PIMPL() \
    class Impl; \
    std::unique_ptr<Impl> m_pimpl; \
    inline const Impl *impl() const { return m_pimpl.get(); } \
    inline Impl *impl() { return m_pimpl.get(); }

#endif // CPPCONFIGFRAMEWORK_PIMPLWRAPPER_HPP
