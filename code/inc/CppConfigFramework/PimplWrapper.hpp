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

namespace CppConfigFramework
{

/*!
 * A convenience template class for PIMPL (pointer to implementation) with const propagation
 *
 * \tparam  T   Actual PIMPL data
 */
template<typename T>
class PimplWrapper
{
public:
    //! Constructor
    PimplWrapper()
        : m_pimpl(std::make_unique<T>())
    {
    }

    //! Constructor
    PimplWrapper(T &&other)
        : m_pimpl(std::make_unique<T>(std::move(other)))
    {
    }

    //! Copy constructor
    PimplWrapper(const PimplWrapper &other) = delete;

    //! Move constructor
    PimplWrapper(PimplWrapper &&other) noexcept = default;

    //! Destructor
    ~PimplWrapper() = default;

    //! Copy assignment operator
    PimplWrapper &operator=(const PimplWrapper &rhs) = delete;

    //! Move assignment operator
    PimplWrapper& operator=(PimplWrapper &&other) noexcept = default;

    //! Member access operator
    const T* operator->() const
    {
        return m_pimpl.get();
    }

    //! Member access operator
    T* operator->()
    {
        return m_pimpl.get();
    }

    //! Dereference operator
    const T& operator*() const
    {
        return *m_pimpl;
    }

    //! Dereference operator
    T& operator*()
    {
        return *m_pimpl;
    }

private:
    //! Actual PIMPL data
    std::unique_ptr<T> m_pimpl;
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_PIMPLWRAPPER_HPP
