/*****************************************************************************
 *
 * frame_allocator.h
 *   Pool of preallocated frame objects.
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


#ifndef _FRAME_ALLOCATOR_H
#define _FRAME_ALLOCATOR_H

#include "sv_os.h"
#include "streamprv.h"

typedef struct frame_allocator frame_allocator;

typedef struct frame_pooled : public frame_base {
    int                         ownerTag;
    fn_frame_reset              resetter;
    frame_pooled*               next;
    frame_allocator*            fa;
} frame_pooled_t;

SVCORE_API frame_allocator*    create_frame_allocator(const char* name);
SVCORE_API frame_pooled_t*     frame_allocator_get(frame_allocator* fa);
SVCORE_API void                frame_allocator_return(frame_allocator* fa, frame_pooled_t* frame);
SVCORE_API void                frame_allocator_register_frame(frame_allocator* fa, frame_pooled_t* newFrame);
SVCORE_API const char*         frame_allocator_get_name(frame_allocator* fa);
SVCORE_API void                frame_allocator_get_stats(frame_allocator* fa, int* freect, int* allocct);
SVCORE_API void                destroy_frame_allocator(frame_allocator** bfa, fn_stream_log logCb);



#endif