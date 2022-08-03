/*****************************************************************************
 *
 * sv_module_def.cpp
 *   A module definition helper - used to define node-specific macros.
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


#ifndef SV_MODULE_NAME
    #define SV_MODULE_NAME SV_MODULE_VAR->name
#endif

#include "sv_os.h"

static int _gTraceLevel = sv_get_trace_level("SV_TRACE_" SV_MODULE_ID);
static void        _set_module_trace_level  (int nLevel)
{
    _gTraceLevel = nLevel;
}
#define TRACE_C(l,y) if (_gTraceLevel>l) { SV_MODULE_VAR->logCb(logTrace,y); }
#define TRACE_L TRACE_C
#define TRACE(y) TRACE_L(0,y)

