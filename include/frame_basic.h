/*****************************************************************************
 *
 * frame_basic.h
 *   Basic frame object around a buffer.
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

#ifndef __FRAME_BASIC_H
#define __FRAME_BASIC_H

#define DEBUG_FRAME_ALLOC 0
#define ENABLE_SERIALIZATION 0

#include "frame_allocator.h"

//-----------------------------------------------------------------------------
typedef struct basic_frame : public frame_pooled {
    size_t              width;
    size_t              height;
    int                 pixelFormat;
    uint8_t*            mem;
    uint8_t*            data;
    size_t              allocSize;
    size_t              dataSize;
    int64_t             pts;
    int64_t             dts;
    int                 keyframe;
    int                 mediaType;
    void*               userContext;
#if ENABLE_SERIALIZATION
    const char*         serializationLocation;
#endif
#if DEBUG_FRAME_ALLOC>0
    fn_stream_log       logCb;
#endif
    frame_obj*          srcFrame; // source frame from which this frame was derived
} basic_frame_obj;


//-----------------------------------------------------------------------------
#define BASIC_STREAM_MAGIC 0x2112
#define DECLARE_FRAME(param, name, errval) \
    DECLARE_OBJ(basic_frame_obj, name,  param, BASIC_STREAM_MAGIC, errval)
#define DECLARE_FRAME_V(param, name) \
    DECLARE_OBJ_V(basic_frame_obj, name,  param, BASIC_STREAM_MAGIC)


//-----------------------------------------------------------------------------
SVCORE_API frame_api_t*      get_basic_frame_api           ();

SVCORE_API basic_frame_obj*  alloc_basic_frame             (int ownerTag,
                                                 size_t desiredSize,
                                                 fn_stream_log logCb);
SVCORE_API basic_frame_obj*  alloc_basic_frame2            (int ownerTag,
                                                 size_t desiredSize,
                                                 fn_stream_log logCb,
                                                 frame_allocator* bfa);
SVCORE_API basic_frame_obj*  clone_basic_frame             (basic_frame_obj* src,
                                                 fn_stream_log logCb,
                                                 frame_allocator* bfa);
SVCORE_API int               grow_basic_frame              (basic_frame_obj* frame,
                                                 size_t desiredSize,
                                                 int keepData);
SVCORE_API size_t            get_basic_frame_free_space    (basic_frame_obj* frame);
SVCORE_API int               ensure_basic_frame_free_space (basic_frame_obj* frame,
                                                 size_t additionalSpace);
SVCORE_API int               append_basic_frame            (basic_frame_obj* frame,
                                                 uint8_t* data,
                                                 size_t size);
SVCORE_API int               enable_basic_frame_alloc_pool (int enable);


#endif
