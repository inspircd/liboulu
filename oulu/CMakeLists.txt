# Oulu <https://github.com/inspircd/liboulu/>
# SPDX-License-Identifier: LGPL-3.0-or-later

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
	message(FATAL_ERROR "You must run CMake using the CMakeLists.txt in the root directory!")
endif()

file(GLOB OULU_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.hpp")
add_library("oulu" STATIC ${OULU_SOURCES})
target_compile_definitions("oulu" PRIVATE "OULU_BUILD")
