# Oulu <https://github.com/SadieCat/oulu/>
# SPDX-License-Identifier: LGPL-3.0-or-later

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
	message(FATAL_ERROR "You must run CMake using the CMakeLists.txt in the root directory!")
endif()

find_package("Catch2")
if(Catch2_FOUND)
	include(Catch)
	file(GLOB TESTS CONFIGURE_DEPENDS "*.cpp")
	foreach(TEST ${TESTS})
		cmake_path(GET TEST STEM TEST_NAME)
		set(TEST_TARGET "oulu-test-${TEST_NAME}")

		add_executable(${TEST_TARGET} ${TEST})
		target_link_libraries(${TEST_TARGET} Catch2::Catch2WithMain "oulu")
		catch_discover_tests(${TEST_TARGET})
	endforeach()
endif()

