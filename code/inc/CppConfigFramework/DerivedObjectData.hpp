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

#ifndef CPPCONFIGFRAMEWORK_DERIVEDOBJECTDATA_HPP
#define CPPCONFIGFRAMEWORK_DERIVEDOBJECTDATA_HPP

// C++ Config Framework includes
#include <CppConfigFramework/CppConfigFrameworkExport.hpp>
#include <CppConfigFramework/Pimpl.hpp>

// Qt includes
#include <QtCore/QStringList>

// System includes

// Forward declarations
namespace CppConfigFramework
{
class ConfigNode;
}

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

/*!
 * This class holds a configuration node
 */
class CPPCONFIGFRAMEWORK_EXPORT DerivedObjectData
{
public:
    //! Constructor
    DerivedObjectData();

    //! Copy constructor
    DerivedObjectData(const DerivedObjectData &other) = delete;

    //! Move constructor
    DerivedObjectData(DerivedObjectData &&other) noexcept;

    //! Destructor
    ~DerivedObjectData();

    //! Copy assignment operator
    DerivedObjectData &operator=(const DerivedObjectData &other) = delete;

    //! Move assignment operator
    DerivedObjectData &operator=(DerivedObjectData &&other) noexcept;

    //! Gets the list of references to Object nodes to use as the base for the DerivedObject node
    QStringList bases() const;

    //! Sets the list of references to Object nodes to use as the base for the DerivedObject node
    void setBases(const QStringList &bases);

    //! Gets the node that needs to be applied over the bases of the DerivedObject node
    const ConfigNode &config() const;

    //! Sets the node that needs to be applied over the bases of the DerivedObject node
    void setConfig(ConfigNode &&node);

private:
    CPPCONFIGFRAMEWORK_DECLARE_PIMPL()
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_DERIVEDOBJECTDATA_HPP
