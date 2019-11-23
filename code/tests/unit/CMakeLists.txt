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

# --------------------------------------------------------------------------------------------------
# Custom (meta) targets
# --------------------------------------------------------------------------------------------------
add_custom_target(all_unit_tests)

# --------------------------------------------------------------------------------------------------
# Helper methods
# --------------------------------------------------------------------------------------------------
function(CppConfigFramework_AddUnitTest)
    # Function parameters
    set(options)                        # Boolean parameters
    set(oneValueParams      TEST_NAME)  # Parameters with one value
    set(multiValueParams)               # Parameters with multiple values

    cmake_parse_arguments(PARAM "${options}" "${oneValueParams}" "${multiValueParams}" ${ARGN})

    # Add test
    CppConfigFramework_AddTest(${ARGN} LABELS CPPCONFIGFRAMEWORK_UNIT_TESTS)

    # Add test to target "all_unit_tests"
    add_dependencies(all_unit_tests ${PARAM_TEST_NAME})
endfunction()

# --------------------------------------------------------------------------------------------------
# Unit tests
# --------------------------------------------------------------------------------------------------
add_subdirectory(ConfigLoader)
add_subdirectory(ConfigNode)
add_subdirectory(ConfigNodePath)
add_subdirectory(ConfigParameterLoader)
add_subdirectory(ConfigParameterValidator)
add_subdirectory(ConfigReader)
add_subdirectory(EnvironmentVariables)

# --------------------------------------------------------------------------------------------------
# Code Coverage
# --------------------------------------------------------------------------------------------------
if (CppConfigFramework_CodeCoverage MATCHES ON)
    # Prepare targets for code coverage
    SETUP_TARGET_FOR_COVERAGE(
        NAME ctest_coverage_all_unit_tests
        EXECUTABLE ctest -L CPPCONFIGFRAMEWORK_UNIT_TESTS -j ${PROCESSOR_COUNT}
        DEPENDENCIES all_unit_tests
        PROJECT_PATH ${PROJECT_SOURCE_DIR}
        EXCLUDES ${CppConfigFramework_CodeCoverage_EXCLUDES}
        )
endif()
