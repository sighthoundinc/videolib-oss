/*****************************************************************************
 *
 * logging.h
 *   Logging primitives used by videoLib (mostly in SV context)
 *
 *****************************************************************************
 *
 * Copyright 2013-2022 Sighthound, Inc.
 *
 * Licensed under the GNU GPLv3 license found at
 * https://www.gnu.org/licenses/gpl-3.0.txt
 *
 * Alternative licensing available from Sighthound, Inc.
 * by emailing opensource@sighthound.com
 *
 * This file is part of the Sighthound Video project which can be found at
 * https://github.url/thing
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; using version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 *****************************************************************************/

#ifndef LOGGING_H
#define LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

#define kLogLevelCritical 50
#define kLogLevelError    40
#define kLogLevelWarning  30
#define kLogLevelInfo     20
#define kLogLevelDebug    10

// The function that should actually do the logging...
typedef void (*log_fn_t)(int level, const char* s);

#ifdef _MSC_VER
#define PRINTF_ATTR(a,b)
#else
#define PRINTF_ATTR(a,b) __attribute__ ((format (printf, a, b)))
#endif

// Our helper functions to call the log function...
void log_impl(log_fn_t logFn, int level, const char* format, va_list ap);
void log_msg(log_fn_t logFn, int level, const char* format, ...) PRINTF_ATTR(3,4);
void log_err(log_fn_t logFn, const char* format, ...) PRINTF_ATTR(2, 3);
void log_warn(log_fn_t logFn, const char* format, ...) PRINTF_ATTR(2, 3);
void log_info(log_fn_t logFn, const char* format, ...) PRINTF_ATTR(2, 3);
void log_dbg(log_fn_t logFn, const char* format, ...) PRINTF_ATTR(2, 3);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif // LOGGING_H
