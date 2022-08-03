/*****************************************************************************
 *
 * streamprv.h
 *   videoLib toolkit's internal implementation utilities.
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

#ifndef STREAMPRV_H
#define STREAMPRV_H

#include "stream.h"
#include "streamFactories.h"
#include "logging.h"

#ifdef __cplusplus

#include <list>
//-----------------------------------------------------------------------------
// Frame Queue (these will only be useful from C++)
//-----------------------------------------------------------------------------
typedef std::list<frame_obj*>       FrameList;
typedef FrameList::iterator         FrameIterator;

FrameList*          frame_list_create();
void                frame_list_clear(FrameList*);
void                frame_list_destroy(FrameList**);

extern "C" {

#endif

typedef struct frame_allocator      frame_allocator;

#define INVALID_PTS (uint64_t)-1


//-----------------------------------------------------------------------------
// Stats
//-----------------------------------------------------------------------------
#define STATS_API(type,shorthand)\
typedef struct stats_item_##shorthand {\
    type        min;\
    type        max;\
    type        last;\
    type        cumulative;\
    size_t      sampleCount;\
} stats_item_##shorthand##_t;\
void stats_##shorthand##_init  (stats_item_##shorthand##_t* item);\
void stats_##shorthand##_update(stats_item_##shorthand##_t* item, type value);\
void stats_##shorthand##_add(stats_item_##shorthand##_t* item, type value);\
type stats_##shorthand##_average(stats_item_##shorthand##_t* item);\
void stats_##shorthand##_combine(stats_item_##shorthand##_t* item, stats_item_##shorthand##_t* other);

STATS_API(INT64_T, int);
STATS_API(double, d);

typedef struct stats_snapshot_base {
    INT64_T                     time;
} stats_snapshot_base_t;

//-----------------------------------------------------------------------------
// FPS limiter
//-----------------------------------------------------------------------------
typedef struct  fps_limiter fps_limiter;
fps_limiter*    fps_limiter_create(size_t frameInterval, float limit);
int             fps_limiter_report_frame(fps_limiter*, float* currentFps, int64_t framePts);
void            fps_limiter_use_second_intervals(fps_limiter* limiter, int useSecondIntervals );
void            fps_limiter_use_wall_clock(fps_limiter*, int useWallClock);
void            fps_limiter_use_ts_as_diff(fps_limiter*, int useTsAsDiff );
float           fps_limiter_get_fps(fps_limiter*);
size_t          fps_limiter_get_frames_accepted(fps_limiter*);
size_t          fps_limiter_get_frames_rejected(fps_limiter*);
void            fps_limiter_destroy(fps_limiter** limiter);


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------



frame_base_t*     frame_init                              ( size_t size,
                                                            int tag,
                                                            frame_api_t* api,
                                                            fn_frame_destroy destructor);
stream_base_t*    stream_init                             ( size_t size,
                                                            int tag,
                                                            stream_api_t* api,
                                                            const char* name,
                                                            fn_stream_destroy destructor);


int               get_basic_frame_count                   ();
int               enable_frame_leak_detection             (int enable);
void              print_allocated_frames                  (fn_stream_log log);


int         configure_stream            (stream_obj* stream, const char* prefix, log_fn_t logCb, ...);
void        stream_set_default_log_cb   (fn_stream_log cb);


#ifdef __cplusplus
}  /* extern "C" */
#endif


#define DECLARE_OBJ(type, name, param, magicval, ret) \
    type* name = (type*)param;\
    if (name == NULL || name->magic != magicval)\
        return ret;

#define DECLARE_OBJ_V(type, name, param, magicval) \
    type* name = (type*)param;\
    if (name == NULL || name->magic != magicval)\
        return;

#define COPY_PARAM_BASE(stream, name, _Type, _value, _res)\
    _Type _param = _value;\
    if (*size<sizeof(_Type)){\
        stream->logCb(logError, _FMT("Insufficient storage for parameter '" << name << "': provided=" <<\
                                *size << " required=" << sizeof(_Type)));\
        *size = sizeof(_Type);\
        _res = -1;\
    } else {\
        *size = sizeof(_Type);\
        *(_Type*)value = _param;\
        _res = 0;\
    }

#define COPY_PARAM_ERR(stream, name, _Type, _value, _errval, _res)\
    COPY_PARAM_BASE(stream, name, _Type, _value, _res)\
    _res=( (_res>=0 && _value!=_errval) ? 0 : -1 );

#define COPY_PARAM_IF(stream, nameVar, name, _Type, _value)\
    if (!_stricmp(nameVar, name)) { \
        int _res;\
        COPY_PARAM_BASE(stream, name, _Type, _value, _res)\
        return _res;\
    }

#define COPY_PARAM_IF_SAFE(stream, nameVar, name, _Type, _value, _mutex)\
    if (!_stricmp(nameVar, name)) { \
        int _res;\
        sv_mutex_enter(_mutex);\
        COPY_PARAM_BASE(stream, name, _Type, _value, _res)\
        sv_mutex_exit(_mutex);\
        return _res;\
    }

#define COPY_PARAM_ERR_IF(stream, nameVar, name, _Type, _value, _errval)\
    if (!_stricmp(nameVar, name)) { \
        int _res;\
        COPY_PARAM_ERR(stream, name, _Type, _value, _errval, _res)\
        return _res;\
    }

#define SET_PARAM_IF(stream, nameVar, name, _Type, _value)\
    if (!_stricmp(nameVar, name)) { \
        _value = *(const _Type*)value;\
        return 0;\
    }

#define SET_PARAM_IF_SAFE(stream, nameVar, name, _Type, _value, _mutex)\
    if (!_stricmp(nameVar, name)) { \
        sv_mutex_enter(_mutex);\
        _value = *(const _Type*)value;\
        sv_mutex_exit(_mutex);\
        return 0;\
    }

#define SET_STR_PARAM_IF(stream, nameVar, name, _value)\
    if (!_stricmp(nameVar, name)) { \
        sv_freep(&(_value));\
        _value = strdup(value?(const char*)value:"");\
        return 0;\
    }

#define SET_STR_PARAM_IF_SAFE(stream, nameVar, name, _value, _mutex)\
    if (!_stricmp(nameVar, name)) { \
        sv_mutex_enter(_mutex);\
        sv_freep(&(_value));\
        _value = strdup((const char*)value);\
        sv_mutex_exit(_mutex);\
        return 0;\
    }

#define INSERT_FILTER_F(api,ctx,factory,name,insert_before,flags)\
        api->insert_element(&ctx, &api, insert_before, get_##factory()->create(name), flags);
#define INSERT_FILTER(api,ctx,factory,name,insert_before)\
        INSERT_FILTER_F(api,ctx,factory,name,insert_before,svFlagNone)
#define APPEND_FILTER_F(api,ctx,factory,name,flags)\
        INSERT_FILTER_F(api,ctx,factory,name,NULL,flags)
#define APPEND_FILTER(api,ctx,factory,name)\
        INSERT_FILTER_F(api,ctx,factory,name,NULL,svFlagNone)

#define CONFIG_FILTER(ctx,prefix,logCb,errorLabel,...)\
        if ( configure_stream(ctx, prefix, logCb, ##__VA_ARGS__) < 0 ) {\
            goto errorLabel;\
        }\

#if SIGHTHOUND_VIDEO
SVCORE_API stream_api_t* _enable_timestamp( stream_obj** pCtx,
                                int flags,
                                int timestampFlags,
                                uint64_t offset,
                                const char* name,
                                const char* sInsertAfter,
                                log_fn_t logFn);
#endif

SVCORE_API frame_obj*  alloc_clone_frame    (int ownerTag,
                                frame_allocator* fa,
                                frame_obj* original,
                                INT64_T ptsOverride );



#endif
