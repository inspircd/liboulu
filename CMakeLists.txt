# Oulu <https://github.com/inspircd/liboulu/>
# SPDX-License-Identifier: LGPL-3.0-or-later

cmake_minimum_required(VERSION "3.21" FATAL_ERROR)

project("Oulu" LANGUAGES "CXX")

if(CMAKE_BUILD_TYPE STREQUAL "")
	message(STATUS "CMAKE_BUILD_TYPE has not been specified; defaulting to Release")
	set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_TYPE MATCHES "^(Debug|RelWithDebInfo)$")
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# Oulu requires C++20
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory("oulu")

option(OULU_BUILD_TESTS "Whether to also build unit tests" ${PROJECT_IS_TOP_LEVEL})
if(OULU_BUILD_TESTS)
	enable_testing()
	add_subdirectory("tests")
endif()
