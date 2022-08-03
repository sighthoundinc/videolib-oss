/*****************************************************************************
 *
 * stream.h
 *   Definition of videoLib's basic types and operations.
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

#ifndef VIDEOLIB_STREAM_API_H
#define VIDEOLIB_STREAM_API_H

#include <stdlib.h>
#include "sv_os.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct stream     stream_obj;
typedef struct frame      frame_obj;
typedef struct event      stream_ev_obj;

struct stream_api;

typedef struct rational {
    int num;
    int denum;
} rational_t;

typedef struct point {
    int     x;
    int     y;
} point_t;

typedef struct image_size {
    int     width;
    int     height;
} image_size_t;

typedef struct rect {
    int     x;
    int     y;
    int     w;
    int     h;
} rect_t;


enum SeekFlag {
    sfBackward = 1,
    sfFrame    = 2,
    sfAny      = 4,
    sfByte     = 8,
    sfPrecise  = 16
};

// Video quality profile
enum SVVideoProfile {
    svvpNotSpecified=-1,
    svvpOriginal=0,
    svvpHighest=1,
    svvpVeryHigh=5,
    svvpHigh=10,
    svvpMedium=20,
    svvpLow=30
};

enum CodecId {
    streamUnknown,
    streamBitmap,
    streamH264,
    streamMP4,
    streamMJPEG,
    streamJPG,
    streamGIF,
    streamLinear,
    streamPCMU,
    streamPCMA,
    streamAAC,
    streamTransparentBit = 0x800000
};

// Subset of pixfmt's we're able to operate on
enum PixFmt {
    pfmtUndefined = -1,
    pfmtRGB24, // PIX_FMT_RGB24
    pfmtBGR24, // PIX_FMT_BGR24
    pfmtRGB8,  // PIX_FMT_RGB8 -- gif needs this
    pfmtYUV420P, // PIX_FMT_YUV420P
    pfmtYUV422P, // PIX_FMT_YUV422P
    pfmtYUV444P, // PIX_FMT_YUV422P
    pfmtYUYV422, // PIX_FMT_YUYV422
    pfmtNV12,
    pfmtNV16,
    pfmtNV20,
    pfmtNV21,
    pfmtYUVJ420P,
    pfmtYUVJ422P,
    pfmtYUVJ444P,
    pfmtRGBA,
    pfmtARGB
};

enum SampleFormat {
    sfmtUndefined = -1,
    sfmtDouble,
    sfmtFloat,
    sfmtInt32,
    sfmtInt16,
    sfmtInt8
};

enum MediaType {
    mediaUnknown = 0,
    mediaAudio,
    mediaVideo,
    mediaVideoTime,     // placeholder for a video frame, with no data attached
    mediaText,
    mediaMetadata,
    mediaTotal = mediaMetadata
};

// This comes handy when detemining what these values mean
// http://blog.pearce.org.nz/2013/11/what-does-h264avc1-codecs-parameters.html
enum H264Profiles {
    h264Baseline    = 66,
    h264Main        = 77,
    h264High        = 100,
    h264High10      = 110,
    h264Extended    = 88
};

enum Flags {
    svFlagNone = 0x00000000,
    svFlagStreamInitialized = 0x00000001,
    svFlagStreamOpen = 0x00000002
};

// Log API
enum Level { logTrace=10, logDebug=10, logInfo=20, logWarning=30, logError=40, logAlert=50 };
typedef void        (*fn_stream_log)                (int level, const CHAR_T* s);

// Frame API. To begin with, we only use it to access the data; however can be extended to access/provide metadata about the frame
typedef frame_obj*  (*fn_frame_create)            ();
typedef INT64_T     (*fn_frame_get_ts)            (frame_obj* frame);
typedef int         (*fn_frame_set_ts)            (frame_obj* frame, INT64_T ts);
typedef size_t      (*fn_frame_get_size)          (frame_obj* frame);
typedef int         (*fn_frame_set_size)          (frame_obj* frame, size_t size);
typedef int         (*fn_frame_get_int_attribute) (frame_obj* frame);
typedef int         (*fn_frame_set_int_attribute) (frame_obj* frame, int attr);
typedef const void* (*fn_frame_get_data)          (frame_obj* frame);
typedef void*       (*fn_frame_get_buffer)        (frame_obj* frame, size_t size);
typedef int         (*fn_frame_set_user_context)  (frame_obj* frame, void* ctx);
typedef void*       (*fn_frame_get_user_context)  (frame_obj* frame);
typedef void*       (*fn_frame_get_backing_obj)   (frame_obj* frame, const char* objType);
typedef int         (*fn_frame_set_backing_obj)   (frame_obj* frame, const char* objType, void* obj);
typedef int         (*fn_frame_serialize)         (frame_obj* frame, const char* location);
typedef int         (*fn_frame_deserialize)       (frame_obj* frame, const char* location);
typedef void        (*fn_frame_destroy)           (frame_obj* frame);
typedef void        (*fn_frame_reset)             (frame_obj* frame);

// Stream API
typedef stream_obj* (*fn_stream_create)           (const char* name);
typedef int         (*fn_stream_set_source)       (stream_obj* stream, stream_obj* src, INT64_T flags);
typedef int         (*fn_stream_set_log_cb)       (stream_obj* stream, fn_stream_log log);
typedef const char* (*fn_stream_get_name)         (stream_obj* stream);
typedef stream_obj* (*fn_stream_find_element)     (stream_obj* stream, const char* name);
typedef int         (*fn_stream_remove_element)   (stream_obj** pStream, struct stream_api** pAPI, const char* name, stream_obj** removedStream);
typedef int         (*fn_stream_insert_element)   (stream_obj** pStream, struct stream_api** pAPI, const char* insertBefore, stream_obj* newElement, INT64_T flags);
typedef int         (*fn_stream_set_param)        (stream_obj* stream, const CHAR_T* name, const void* value);
typedef int         (*fn_stream_get_param)        (stream_obj* stream, const CHAR_T* name, void* value, size_t* size);
typedef int         (*fn_stream_open_in)          (stream_obj* stream);
typedef int         (*fn_stream_seek)             (stream_obj* stream, INT64_T timestamp, int flags);
typedef size_t      (*fn_stream_get_width)        (stream_obj* stream);
typedef size_t      (*fn_stream_get_height)       (stream_obj* stream);
typedef int         (*fn_stream_get_pixel_format) (stream_obj* stream);
typedef int         (*fn_stream_read_frame)       (stream_obj* stream, frame_obj** frame);
typedef int         (*fn_stream_print_pipeline)   (stream_obj* stream, char* buffer, size_t size);
typedef int         (*fn_stream_close)            (stream_obj* stream);
typedef void        (*fn_stream_destroy)          (stream_obj* stream);

typedef void        (*fn_set_trace_level)         (int level);

typedef void        (*fn_stream_event_cb)         (stream_obj* stream, stream_ev_obj* ev);

typedef stream_ev_obj* (*fn_event_create)         (const char* name);
typedef const char* (*fn_event_get_name)          (stream_ev_obj* ev);
typedef int64_t     (*fn_event_get_ts)            (stream_ev_obj* ev);
typedef int         (*fn_event_set_ts)            (stream_ev_obj* ev, int64_t ts);
typedef const void* (*fn_event_get_context)       (stream_ev_obj* ev);
typedef int         (*fn_event_set_context)       (stream_ev_obj* ev, const void* context);
typedef int         (*fn_event_get_property)      (stream_ev_obj* ev, const CHAR_T* name, void* value, size_t* size);
typedef int         (*fn_event_set_property)      (stream_ev_obj* ev, const CHAR_T* name, void* value, size_t size);
typedef void        (*fn_event_destroy)           (stream_ev_obj* ev);


typedef struct stream_ev_api {
    fn_event_create             create;
    fn_event_get_name           get_name;
    fn_event_get_ts             get_ts;
    fn_event_set_ts             set_ts;
    fn_event_get_context        get_context;
    fn_event_set_context        set_context;
    fn_event_get_property       get_property;
    fn_event_set_property       set_property;
} stream_ev_api_t;

typedef struct frame_api {
    fn_frame_create             create;
    fn_frame_get_size           get_data_size;
    fn_frame_get_ts             get_pts;
    fn_frame_set_ts             set_pts;
    fn_frame_get_ts             get_dts;
    fn_frame_set_ts             set_dts;
    fn_frame_get_size           get_width;
    fn_frame_set_size           set_width;
    fn_frame_get_size           get_height;
    fn_frame_set_size           set_height;
    fn_frame_get_int_attribute  get_pixel_format;
    fn_frame_set_int_attribute  set_pixel_format;
    fn_frame_get_int_attribute  get_media_type;
    fn_frame_set_int_attribute  set_media_type;
    fn_frame_get_data           get_data;
    fn_frame_get_buffer         get_buffer;
    fn_frame_get_int_attribute  get_keyframe_flag;
    fn_frame_set_int_attribute  set_keyframe_flag;
    fn_frame_get_user_context   get_user_context;
    fn_frame_set_user_context   set_user_context;
    fn_frame_get_backing_obj    get_backing_obj;
    fn_frame_set_backing_obj    set_backing_obj;
    fn_frame_serialize          serialize;
    fn_frame_deserialize        deserialize;
} frame_api_t;

typedef struct stream_api {
    fn_stream_create            create;
    fn_stream_set_source        set_source;
    fn_stream_set_log_cb        set_log_cb;
    fn_stream_get_name          get_name;
    fn_stream_find_element      find_element;
    fn_stream_remove_element    remove_element;
    fn_stream_insert_element    insert_element;
    fn_stream_set_param         set_param;
    fn_stream_get_param         get_param;
    fn_stream_open_in           open_in;
    fn_stream_seek              seek;
    fn_stream_get_width         get_width;
    fn_stream_get_height        get_height;
    fn_stream_get_pixel_format  get_pixel_format;
    fn_stream_read_frame        read_frame;
    fn_stream_print_pipeline    print_pipeline;
    fn_stream_close             close;
    fn_set_trace_level          set_trace_level;
} stream_api_t;

SVCORE_API frame_api_t*      frame_get_api                           (frame_obj* frame);
SVCORE_API stream_api_t*     stream_get_api                          (stream_obj* stream);
SVCORE_API stream_ev_api_t*  stream_ev_get_api                       (stream_ev_obj* event);

/*
    Custom objects (nodes, frames, events) implemented externally to videolib,
    must inherit from these base structs.
*/
typedef struct stream_base {
    long                        magic;
    REF_T                       refcount;
    const char*                 name;
    stream_api_t*               api;
    stream_api_t*               sourceApi;
    stream_obj*                 source;
    int                         sourceInitialized;
    fn_stream_log               logCb;
    fn_stream_destroy           destructor;
    int                         passthrough;
} stream_base_t;

typedef struct frame_base {
    long                        magic;
    REF_T                       refcount;
    frame_api_t*                api;
    fn_frame_destroy            destructor;
} frame_base_t;

typedef struct stream_ev_base {
    long                        magic;
    REF_T                       refcount;
    stream_ev_api_t*            api;
    fn_event_destroy            destructor;
} stream_ev_base_t;



/*
    Frame reference counting model:
    - it is a responsibility of read_frame to return a frame
      with a reference added on behalf of a caller
    - if the caller doesn't pass the frame through,
      it has to release this reference
    - if the caller stores the frame, and passes it on before clearing the stored frame,
      it has to add and maintain its own reference
*/
SVCORE_API REF_T             frame_ref                               (frame_obj* frame);
SVCORE_API REF_T             frame_unref                             (frame_obj** frame);
SVCORE_API void              frame_set                               (frame_obj** dst, frame_obj* src);
SVCORE_API REF_T             stream_ref                              (stream_obj* stream);
SVCORE_API REF_T             stream_unref                            (stream_obj** stream);
SVCORE_API void              stream_set                              (stream_obj** dst, stream_obj* src);
SVCORE_API void              stream_destroy                          (stream_obj* stream);
SVCORE_API const char*       stream_param_name_apply_scope           (stream_obj* stream, const char* name);
SVCORE_API REF_T             stream_ev_ref                           (stream_ev_obj* stream);
SVCORE_API REF_T             stream_ev_unref                         (stream_ev_obj** stream);
SVCORE_API void              stream_ev_set                           (stream_ev_obj** dst, stream_ev_obj* src);
SVCORE_API void              stream_set_default_log_cb               (fn_stream_log log);


#ifdef __cplusplus
}  /* extern "C" */
#endif


#endif // VIDEOLIB_STREAM_API_H

