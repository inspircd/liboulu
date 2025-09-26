// Oulu <https://github.com/inspircd/liboulu/>
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

/** \def OULU_ATTR_NOT_NULL(...)
 * Enables the compile-time checking of arguments that must never be be null. If a function is
 * marked with this attribute then the compiler will warn if a null pointer is passed to any of the
 * specified arguments.
 */
#ifdef __GNUC__
# define OULU_ATTR_NOT_NULL(...) __attribute__((nonnull(__VA_ARGS__)))
#else
# define OULU_ATTR_NOT_NULL(...)
#endif

/** \def OULU_INTERNAL
 * Allows members to be defined as internal-only. This means that they are public for the core
 * library and for tests but private for library consumers.
 */
#ifdef OULU_BUILD
# define OULU_INTERNAL public
#else
# define OULU_INTERNAL private
#endif

/** \def OULU_STRINGIFY(ARG)
 * Allows stringifying an argument which is passed to a preprocessor macro.
 */
#define OULU_STRINGIFY(ARG) OULU_STRINGIFY_INTERNAL(ARG)
#define OULU_STRINGIFY_INTERNAL(ARG) #ARG
