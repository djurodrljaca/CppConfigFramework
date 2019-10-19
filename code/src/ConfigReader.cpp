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

// Own header
#include <CppConfigFramework/ConfigReader.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigNode.hpp>
#include "ConfigReaderImpl.hpp"
#include "DebugHelper.hpp"

// Qt includes
#include <QtCore/QtDebug>

// System includes

// Forward declarations

// Macros
#define DEBUG_METHOD(METHOD)    CPPCONFIGFRAMEWORK_DEBUG_METHOD("ConfigReader::" METHOD)

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

ConfigReader::ConfigReader()
    : m_pimpl(std::make_unique<Impl>())
{
}

// -------------------------------------------------------------------------------------------------

uint32_t ConfigReader::referenceResolutionMaxCycles() const
{
    return impl()->referenceResolutionMaxCycles();
}

// -------------------------------------------------------------------------------------------------

void ConfigReader::setReferenceResolutionMaxCycles(const uint32_t referenceResolutionMaxCycles)
{
    impl()->setReferenceResolutionMaxCycles(referenceResolutionMaxCycles);
}

// -------------------------------------------------------------------------------------------------

ConfigReader::ConfigReader(ConfigReader &&other) noexcept = default;

// -------------------------------------------------------------------------------------------------

ConfigReader::~ConfigReader() = default;

// -------------------------------------------------------------------------------------------------

ConfigReader &ConfigReader::operator=(ConfigReader &&other) noexcept = default;

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigReader::read(const QString &filePath,
                                               const QDir &workingDir,
                                               const QString &sourceNode,
                                               const QString &destinationNode)
{
    return impl()->read(filePath, workingDir, sourceNode, destinationNode);
}

} // namespace CppConfigFramework
