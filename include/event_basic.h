/*****************************************************************************
 *
 * event_basic.h
 *   Basic event object representation
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

#ifndef __EVENT_BASIC_H
#define __EVENT_BASIC_H

static const int kMaxNameSize = 64;
static const int kMaxProps = 4;


typedef struct basic_prop {
    char                name[kMaxNameSize+1];
    size_t              size;
    char                value[1];
} basic_prop_t;

//-----------------------------------------------------------------------------
typedef struct basic_event : public stream_ev_base_t {
    char                name[kMaxNameSize+1];
    const void*         context;
    int64_t             timestamp;
    basic_prop_t*       properties[kMaxProps];
} basic_event_obj;


//-----------------------------------------------------------------------------
#define BASIC_EVENT_MAGIC 0x3113
#define DECLARE_EVENT(param, name, errval) \
    DECLARE_OBJ(basic_event_obj, name,  param, BASIC_EVENT_MAGIC, errval)
#define DECLARE_EVENT_V(param, name) \
    DECLARE_OBJ_V(basic_event_obj, name,  param, BASIC_EVENT_MAGIC)


//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

stream_ev_api_t*      get_basic_event_api       ();

#ifdef __cplusplus
}  /* extern "C" */
#endif


#endif