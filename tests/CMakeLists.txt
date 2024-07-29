# Oulu <https://github.com/SadieCat/oulu/>
# SPDX-License-Identifier: LGPL-3.0-or-later

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
	message(FATAL_ERROR "You must run CMake using the CMakeLists.txt in the root directory!")
endif()

find_package("CxxTest" REQUIRED)
if(CXXTEST_FOUND)
	file(GLOB TESTS CONFIGURE_DEPENDS "*.cpp")
	foreach(TEST ${TESTS})
		cmake_path(GET TEST STEM TEST_NAME)
		set(TEST_TARGET "oulu-test-${TEST_NAME}")

		cxxtest_add_test(${TEST_TARGET} "${TEST_NAME}.test.cpp" ${TEST})
		target_link_libraries(${TEST_TARGET} "oulu")
	endforeach()
endif()
