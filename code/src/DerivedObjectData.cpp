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
 * Contains a class for storing of DerivedObject data
 */

// Own header
#include <CppConfigFramework/DerivedObjectData.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigNode.hpp>
#include "DebugHelper.hpp"

// Qt includes

// System includes

// Forward declarations

// Macros
#define DEBUG_METHOD(METHOD)    CPPCONFIGFRAMEWORK_DEBUG_METHOD("DerivedObjectData::" METHOD)

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

// -------------------------------------------------------------------------------------------------
// Implementation
// -------------------------------------------------------------------------------------------------

/*!
 * Private implementation for DerivedObjectData class
 */
class DerivedObjectData::Impl
{
public:
    //! Constructor
    Impl() = default;

    //! Copy constructor
    Impl(const Impl &other) = delete;

    //! Move constructor
    Impl(Impl &&other) noexcept = default;

    //! Destructor
    ~Impl() = default;

    //! Copy assignment operator
    Impl &operator=(const Impl &other) = delete;

    //! Move assignment operator
    Impl &operator=(Impl &&other) noexcept = default;

    //! \copydoc    DerivedObject::bases()
    inline QStringList bases() const
    {
        return m_bases;
    }

    //! \copydoc    DerivedObject::setBases()
    inline void setBases(const QStringList &bases)
    {
        m_bases = bases;
    }

    //! \copydoc    DerivedObject::config()
    inline const ConfigNode &config() const
    {
        return m_config;
    }

    //! \copydoc    DerivedObject::config()
    inline ConfigNode &config()
    {
        return m_config;
    }

private:
    //! List of references to Object nodes to use as the base for the DerivedObject node
    QStringList m_bases;

    //! Node that needs to be applied over the bases of the DerivedObject node
    ConfigNode m_config;
};

// -------------------------------------------------------------------------------------------------
// API
// -------------------------------------------------------------------------------------------------

DerivedObjectData::DerivedObjectData()
    : m_pimpl(std::make_unique<Impl>())
{
}

// -------------------------------------------------------------------------------------------------

DerivedObjectData::DerivedObjectData(DerivedObjectData &&other) noexcept = default;

// -------------------------------------------------------------------------------------------------

DerivedObjectData::~DerivedObjectData() = default;

// -------------------------------------------------------------------------------------------------

DerivedObjectData &DerivedObjectData::operator=(DerivedObjectData &&other) noexcept = default;

// -------------------------------------------------------------------------------------------------

QStringList DerivedObjectData::bases() const
{
    return impl()->bases();
}

// -------------------------------------------------------------------------------------------------

void DerivedObjectData::setBases(const QStringList &bases)
{
    impl()->setBases(bases);
}

// -------------------------------------------------------------------------------------------------

const ConfigNode &DerivedObjectData::config() const
{
    return impl()->config();
}

// -------------------------------------------------------------------------------------------------

ConfigNode &DerivedObjectData::config()
{
    return impl()->config();
}

} // namespace CppConfigFramework
