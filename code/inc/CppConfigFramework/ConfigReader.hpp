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
 * Contains a class for reading configuration files
 */

#ifndef CPPCONFIGFRAMEWORK_CONFIGREADER_HPP
#define CPPCONFIGFRAMEWORK_CONFIGREADER_HPP

// C++ Config Framework includes
#include <CppConfigFramework/ConfigNode.hpp>
#include <CppConfigFramework/CppConfigFrameworkExport.hpp>
#include <CppConfigFramework/Pimpl.hpp>

// Qt includes
#include <QtCore/QString>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

/*!
 * This class reads and holds the configuration
 */
class CPPCONFIGFRAMEWORK_EXPORT ConfigReader
{
public:
    //! Constructor
    ConfigReader();

    //! Copy constructor
    ConfigReader(const ConfigReader &other) = delete;

    //! Move constructor
    ConfigReader(ConfigReader &&other) noexcept;

    //! Destructor
    ~ConfigReader();

    //! Copy assignment operator
    ConfigReader &operator=(const ConfigReader &other) = delete;

    //! Move assignment operator
    ConfigReader &operator=(ConfigReader &&other) noexcept;

    /*!
     * Read the specified config file
     *
     * \param   filePath        Path to the configuration file
     * \param   sourceNode      Node path to the node that needs to be extracted from this
     *                          configuration file (must be absolute node path)
     * \param   destinationNode Node path to the destination node where the result needs to be
     *                          stored (must be absolute node path)
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    std::unique_ptr<ConfigNode> read(const QString &filePath,
                                     const QString &sourceNode = QStringLiteral("/"),
                                     const QString &destinationNode = QStringLiteral("/"));

private:
    constexpr static int s_referenceResolutionMaxCycles = 100;
    CPPCONFIGFRAMEWORK_DECLARE_PIMPL()
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGREADER_HPP
