# This file is part of C++ Config Framework.
#
# C++ Config Framework is free software: you can redistribute it and/or modify it under the terms
# of the GNU Lesser General Public License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# C++ Config Framework is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along with C++ Config
# Framework. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.5.0)

project(CppConfigFramework VERSION 1.1.0)

set(CMAKE_AUTOMOC ON)

find_package(CedarFramework REQUIRED)

# --------------------------------------------------------------------------------------------------
# Code Coverage
# --------------------------------------------------------------------------------------------------
option(CppConfigFramework_CodeCoverage "C++ Config Framework Code Coverage" OFF)

if (CppConfigFramework_CodeCoverage MATCHES ON)
    include(cmake/CodeCoverage.cmake)
    APPEND_COVERAGE_COMPILER_FLAGS()

    set(excludes_cmake_prefix_path)
    foreach(item ${CMAKE_PREFIX_PATH})
        get_filename_component(item ${item} ABSOLUTE)
        list(APPEND excludes_cmake_prefix_path "'${item}/*'")
    endforeach()

    set(CppConfigFramework_CodeCoverage_EXCLUDES
            "'/opt/*'"
            "'/usr/*'"
            ${excludes_cmake_prefix_path}
            "'${CMAKE_SOURCE_DIR}/tests/*'"
            "'${CMAKE_BINARY_DIR}/*'"
        )
endif()

# --------------------------------------------------------------------------------------------------
# CppConfigFramework library
# --------------------------------------------------------------------------------------------------
add_library(CppConfigFramework SHARED
        inc/CppConfigFramework/ConfigContainerHelper.hpp
        inc/CppConfigFramework/ConfigDerivedObjectNode.hpp
        inc/CppConfigFramework/ConfigItem.hpp
        inc/CppConfigFramework/ConfigNode.hpp
        inc/CppConfigFramework/ConfigNodePath.hpp
        inc/CppConfigFramework/ConfigNodeReference.hpp
        inc/CppConfigFramework/ConfigObjectNode.hpp
        inc/CppConfigFramework/ConfigParameterValidator.hpp
        inc/CppConfigFramework/ConfigReader.hpp
        inc/CppConfigFramework/ConfigReaderBase.hpp
        inc/CppConfigFramework/ConfigReaderRegistry.hpp
        inc/CppConfigFramework/ConfigValueNode.hpp
        inc/CppConfigFramework/ConfigWriter.hpp
        inc/CppConfigFramework/EnvironmentVariables.hpp
        inc/CppConfigFramework/LoggingCategories.hpp

        src/ConfigDerivedObjectNode.cpp
        src/ConfigItem.cpp
        src/ConfigNode.cpp
        src/ConfigNodePath.cpp
        src/ConfigNodeReference.cpp
        src/ConfigObjectNode.cpp
        src/ConfigReader.cpp
        src/ConfigReaderBase.cpp
        src/ConfigReaderRegistry.cpp
        src/ConfigValueNode.cpp
        src/ConfigWriter.cpp
        src/EnvironmentVariables.cpp
        src/LoggingCategories.cpp
    )

set_target_properties(CppConfigFramework PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

include(GenerateExportHeader)
generate_export_header(CppConfigFramework
    EXPORT_FILE_NAME exports/CppConfigFramework/CppConfigFrameworkExport.hpp)

target_include_directories(CppConfigFramework PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/inc>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/exports>
        $<INSTALL_INTERFACE:include>
    )

target_link_libraries(CppConfigFramework PUBLIC
        CedarFramework::CedarFramework
    )

set_target_properties(CppConfigFramework PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )

# --------------------------------------------------------------------------------------------------
# Package
# --------------------------------------------------------------------------------------------------
add_library(CppConfigFramework::CppConfigFramework ALIAS CppConfigFramework)

install(TARGETS CppConfigFramework EXPORT CppConfigFrameworkTargets DESTINATION
        LIBRARY  DESTINATION lib
        ARCHIVE  DESTINATION lib
        RUNTIME  DESTINATION bin
        INCLUDES DESTINATION include
    )

install(
        DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}/inc/CppConfigFramework
        DESTINATION include
        COMPONENT   Devel
    )

install(
        DIRECTORY   ${PROJECT_BINARY_DIR}/exports/CppConfigFramework
        DESTINATION include
        COMPONENT   Devel
    )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/CppConfigFramework/CppConfigFrameworkConfigVersion.cmake"
        VERSION ${CppConfigFramework_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

export(EXPORT CppConfigFrameworkTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/CppConfigFramework/CppConfigFrameworkTargets.cmake"
        NAMESPACE CppConfigFramework::
    )

configure_file(cmake/CppConfigFrameworkConfig.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/CppConfigFramework/CppConfigFrameworkConfig.cmake"
        COPYONLY
    )

set(ConfigPackageLocation lib/cmake/CppConfigFramework)

install(
        EXPORT      CppConfigFrameworkTargets
        FILE        CppConfigFrameworkTargets.cmake
        NAMESPACE   CppConfigFramework::
        DESTINATION ${ConfigPackageLocation}
    )

install(
        FILES
            cmake/CppConfigFrameworkConfig.cmake
            "${CMAKE_CURRENT_BINARY_DIR}/CppConfigFramework/CppConfigFrameworkConfigVersion.cmake"
        DESTINATION ${ConfigPackageLocation}
        COMPONENT   Devel
    )

# --------------------------------------------------------------------------------------------------
# Tests
# --------------------------------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests)
