// Oulu <https://github.com/SadieCat/oulu/>
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

/** \def OULU_INTERNAL
 * Allows members to be defined as internal-only. This means that they are public for the core
 * library and for tests but private for library consumers.
 */
#ifdef OULU_BUILD
# define OULU_INTERNAL public
#else
# define OULU_INTERNAL private
#endif
