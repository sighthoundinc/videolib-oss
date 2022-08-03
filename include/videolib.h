/*****************************************************************************
 *
 * videolib.h
 *   C high-level API used by Sighthound Video application
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

#ifndef VIDEOLIB_H
#define VIDEOLIB_H

#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersrc.h>
#include <libavfilter/buffersink.h>
#include <libavutil/mem.h>
#include <libavutil/imgutils.h>

#ifdef WIN32
    #include <windows.h>
#endif

#include "sv_os.h"

#if LOCAL_CAMERA_SUPPORT
    #include <localVideo.h>
#else
    typedef struct Dimensions {
        int width;
        int height;
    } Dimensions;
#endif

#include "stream.h"
#include "videolibExports.h"
#include "logging.h"


// Output codec settings
#define OUTPUT_FILE_TYPE "mp4"
#define OUTPUT_TIME_BASE_DEN 30
#define OUTPUT_TIME_BASE_NUM 1
#define OUTPUT_PIX_FMT pfmtYUV420P


// This buffer depends somewhat on the bit rate, and somewhat on the codec.
// This is the biggest number of bytes a single encoded frame could have.
// We have no real idea of how to choose it, so we just chose something that's
// much bigger than we've actually seen.  If we get anywhere near this maximum,
// we'll spout out a warning, then reallocate.
#define ENCODE_INIT_BUFFER_SIZE 200000

// Get new frame settings
#define GET_FRAME_PIX_FMT pfmtRGB24

// Output file management settings
#define MAX_FILE_LENGTH 120

// Resource identifier schemes for supported types
#define URI_IP_CAMERA "http:"
#define URI_RTSP_CAMERA "rtsp:"
#define URI_LOCAL_CAMERA "device:"

// The header size of memory mapped files
#define MMAP_HEADER_SIZE 24

// The fastest to update shared frames if not the active view
#define MAX_SMALL_VIEW_REFRESH_MS 300

// In practice I never saw a variation, but I don't want rounding errors to
// cause us to not mark a frame.
#define DRAW_BOX_MS_TOLERANCE 3

typedef struct CodecConfig {
    float   bit_rate_multiplier;
    int     max_bit_rate;
    int     gop_size;
    int     keyint_min;
    char*   preset;
    int     max_width;
    int     max_height;
    // Arbitrary value, that implementation will translate into compression settings
    // -1=Not specified, 0=Original, 10=High, 20=Medium, 30=Low
    int     sv_profile;
} CodecConfig;

typedef struct BoxOverlayInfo {
    int64_t readTimeMs;
    const char* drawboxParams;
} BoxOverlayInfo;

typedef int (*progress_fn_t)(int percent);

typedef struct InputData2 {
    stream_obj*         streamCtx;
    AVRational          timeBase;
    const char*         auxInsertionPoint;

    int                 width;
    int                 height;
    int                 hasResize;
    int                 pixFmt;
    int                 fps;
    int                 hasAudio;

    int                 rawFrameRecording;
    int                 audioVolume;
    log_fn_t            logFn;
} InputData2;


#define W_IN(stream) stream->inputData2.width
#define H_IN(stream) stream->inputData2.height
#define P_FMT_IN(stream) stream->inputData2.pixFmt

//-----------------------------------------------------------------------------
typedef struct HLSProfile {
    int     id;
    int     bitrate;
    int     fps;
    int     height16_9;
    int     height4_3;
    int     height;
    int     width;
    int     remux;
    int     h264profile;
    int     h264level;
    int     active;
} HLSProfile;

//-----------------------------------------------------------------------------
typedef struct StreamData {
    // Used for logging errors / info...
    log_fn_t        logFn;

    // Set to 0 when the stream is closed, or when it should be closed
    volatile int    isRunning;

    // 0 if no recording should take place
    int             shouldRecord;

    // The input data stream
    InputData2      inputData2;
    frame_obj*      lastFrameRead;

    // Mutex
    sv_mutex*       mutex;
    sv_mutex*       graphMutex;


    // Support for sharing display frame data between processes
    stream_obj*     mmapSubgraph;
    const char*     mmapPosition;
    int             mmapWidth;
    int             mmapHeight;
    int             mmapFps;
    int             mmapPaused;
    char*           mmapFilename;

    // Configuration information for the output codec
    CodecConfig*    codecConfig;
    int             enableAudioRecording;

    CodecConfig*    liveCodecConfig;
    int             liveTimestampsEnabled;

    HLSProfile*     hlsProfiles;
    int             hlsProfilesCount;
    int             hlsMaxBitrate;
    int             hlsMaxResolution;

    uint64_t        openStartTime;
    uint64_t        openEndTime;
    int             framesServed;
} StreamData;


typedef struct FrameData {
    // A buffer and size information for the image to be processed.
    uint8_t*    procBuffer;
    int         procWidth;
    int         procHeight;
    int         wasResized;

    int64_t     ms;

    uint8_t*    filename;

    int         isRunning;
    frame_obj*  frame;
    uint8_t     storage[0];
} FrameData;


// Closes and frees all resources associated with an InputData2 structure.
SVVIDEOLIB_API void free_input_data(InputData2* dataPtr);

// Attempts to open an input stream and begin a read thread.  Returns a pointer
// to an allocated StreamData structure or NULL if there was an error.
// If record is 0, no video will be recorded.
enum OpenInputFlags {
    oifWantTCP              = 0x0001,
    oifShouldRecord         = 0x0002,
    oifLiveStream           = 0x0004,
    oifKeyframeOnly         = 0x0008,
    oifEdgeThread           = 0x0010,
    oifRenderAudio          = 0x0020,
    oifDebugClips           = 0x0040,
    oifLimitFps             = 0x0080,
    oifDisableAudio         = 0x0100,
    oifShowRegions          = 0x0200,
    oifFastStart            = 0x0400,
    oifRecordInMemory       = 0x0800,
    oifSimulation           = 0x1000,
};

enum TimestampFlags {
    tsfEnable               = 0x001,
    tsf12HrTime             = 0x002,
    tsfUSDate               = 0x004
};

SVVIDEOLIB_API StreamData* open_stream(const char* inputFilename,
        const char* sanitizedFilename, int width, int height, int fps,
        const char* videoDir, CodecConfig* codecConfig,
        int initialFrameBufferSize,
        int flags,
        log_fn_t logFn);

// Closes and frees all resources associated with a StreamData structure.
SVVIDEOLIB_API void free_stream_data(StreamData** dataPtr);


// Returns a FrameData* for the most recently obtained frame, or NULL if there
// has been no new frame since the last call.
SVVIDEOLIB_API void* get_new_frame(StreamData* data, int isLive);

// Get the frame prior to resize, or NULL if doesn't exist
SVVIDEOLIB_API FrameData* get_large_frame(FrameData* smallFrame);

// Frees all resources associated with a FrameData structure.
SVVIDEOLIB_API void free_frame_data(FrameData** data);

// move recorded file from RAM to permanent storage on disk
SVVIDEOLIB_API int move_recorded_file(log_fn_t logFn, const char* src, const char* dst);

// Create a single clip from a one or more video files
SVVIDEOLIB_API uint64_t create_clip(int numFiles, const char** filenames, uint64_t* fileOffsetMs,
                uint64_t firstMs, uint64_t lastMs, const char* outfile,
                CodecConfig* codecConfig, int timestampFlags, int numBoxes,
                BoxOverlayInfo* boxes, const char* format, int fps, log_fn_t logFn,
                progress_fn_t progressCb);
SVVIDEOLIB_API uint64_t fast_create_clip(int numFiles, const char** filenames, uint64_t* fileOffsetMs,
                     uint64_t firstMs, uint64_t lastMs, const char* outfile,
                     const char* format, log_fn_t logFn,
                     progress_fn_t progressCb);

// Returns a count of the number of local cameras on the system, while
// also caching the names and supported resolutions for each device.
SVVIDEOLIB_API int get_local_camera_count(log_fn_t logFn);

// Returns a count of the number of local cameras on the system, while
// also caching the names for each device.
SVVIDEOLIB_API int get_local_camera_count_without_resolution_cache(log_fn_t logFn);

// Returns the name of a local camera, given its device ID.  Must call
// get_local_camera_count() first and every time a device is added or removed.
SVVIDEOLIB_API char* get_local_camera_name(int deviceId);

// Returns the number of supported resolutions of the given device. You must
// call get_local_camera_count() first and every time a device is added or removed.
SVVIDEOLIB_API int get_number_of_supported_resolutions_of_local_camera(int deviceId);

// Returns a single pair of dimensions (width and height) of the given device
// and dimension index.  You must call get_local_camera_count() first and every
// time a device is added or removed.
SVVIDEOLIB_API struct Dimensions* get_supported_resolution_pair_of_device(int deviceId, int resPair);

// Return info about the frames per second of requests and capture...
SVVIDEOLIB_API void get_fps_info(StreamData *data, float* requestFps, float* captureFps);

// Return info about the size we're processing video at.
SVVIDEOLIB_API int get_proc_width(StreamData *data);
SVVIDEOLIB_API int get_proc_height(StreamData *data);

// Open a memory map to use for viewing rgb frames.  On windows filename will
// be the name of the shared memory rather than an actual path.
SVVIDEOLIB_API int open_mmap(StreamData* data, char* filename);

// Close an open memory map.
SVVIDEOLIB_API void close_mmap(StreamData* data);

// Enable sharing of higher resolution frames through an open mmap.
SVVIDEOLIB_API void set_mmap_params(StreamData* data,
        int enableLargeFrames, int width, int height, int fps);

// Controls audio volume on the stream
SVVIDEOLIB_API void set_audio_volume(StreamData* data, int volume);

// Operating on HLS streams
SVVIDEOLIB_API int set_live_stream_limits(StreamData* data, int maxRes, int maxBitrate);
SVVIDEOLIB_API int refresh_hls_profiles    ( StreamData* data );
SVVIDEOLIB_API int prepare_live_stream(StreamData* data, int profileId );
SVVIDEOLIB_API int enable_live_stream(StreamData* data, int profileId, const char* path,
                         int timestampFlags, int64_t startIndex );
SVVIDEOLIB_API void disable_live_stream(StreamData* data, int profileId );

// Returns the most recently capture frame and returns it as a JPEG image at a
// size of the caller's choice.
SVVIDEOLIB_API void* get_newest_frame_as_jpeg(StreamData* data, int width, int height,
		                       int* size);

// Frees data returned from get_newest_frame_as_jpeg().
SVVIDEOLIB_API void free_newest_frame(void*);

SVVIDEOLIB_API int is_running(StreamData* data);

typedef struct ClipStream ClipStream;


typedef struct ClipFrame {
    // A buffer and size information for the image to be processed.
    uint8_t*    buffer;
    int64_t     ms;

    ClipStream* stream;
    frame_obj*  frame;
} ClipFrame;


typedef struct ClipStream {
    log_fn_t            logFn;
    InputData2          input;
    stream_api_t*       api;
    ClipFrame*          nextFrame;
    char*               filename;
    int64_t             lastMsReturned;
    int                 srcWidth;
    int                 srcHeight;
    int                 outWidth;
    int                 outHeight;
    int                 keyframeOnly;
    int                 muted; /* -1 - no audio, 0 - playing, 1 - muted */
    int                 outstandingFrames;
    int                 freed;
} ClipStream;


// Attempts to open a clip to be read.
SVVIDEOLIB_API ClipStream* open_clip  ( const char* filename,
                        int width,
                        int height,
                        uint64_t timestampOffset,
                        int enableAudio,
                        int enableThread,
                        int enableDebug,
                        int timestampFlags,
                        int keyframeOnly,
                        int numBoxes,
                        BoxOverlayInfo* boxes,
                        int numRegions,
                        BoxOverlayInfo* regions,
                        log_fn_t logFn);

// Set the desired size for returned frames. 0 on success, -1 on error.
SVVIDEOLIB_API int set_output_size(ClipStream* stream, int width, int height);

// Return the height and with that retrieved frames will be in
SVVIDEOLIB_API int get_output_height(ClipStream* stream);
SVVIDEOLIB_API int get_output_width(ClipStream* stream);

// Return the height and with that the input file had
SVVIDEOLIB_API int get_input_height(ClipStream* stream);
SVVIDEOLIB_API int get_input_width(ClipStream* stream);

// Returns 1 if the clip has audio, and 0 otherwise
SVVIDEOLIB_API int clip_has_audio(ClipStream* stream);

// mute/unmute audio playback
SVVIDEOLIB_API int set_mute_audio(ClipStream* stream, int mute);

// Closes and frees all resources associated with a ClipStream structure.
SVVIDEOLIB_API void free_clip_stream(ClipStream** dataPtr);

// Return the length of the clip in milliseconds, -1 on error
SVVIDEOLIB_API int64_t get_clip_length(ClipStream* stream);

// Retrieve the next frame from a clip. Returns a ClipFrame*, NULL on error
// or when there are no more frames.
SVVIDEOLIB_API void* get_next_frame(ClipStream* stream);

// Retrieve the prev frame from a file. Returns a ClipFrame*, NULL on error
// or when there are no more frames.
SVVIDEOLIB_API void* get_prev_frame(ClipStream* stream);

// Retrieve the ms offset of the next frame or -1 if no more frames exist.
SVVIDEOLIB_API int64_t get_next_frame_offset(ClipStream* stream);

// Retrieve the frame closest to the given ms offset.  Returns a ClipFrame*,
// NULL on error.
SVVIDEOLIB_API void* get_frame_at(ClipStream* stream, int64_t ms);

// Frees all resources associated with a ClipFrame structure.
SVVIDEOLIB_API void free_clip_frame(ClipFrame** dataPtr);

// Return an array of millisecond offsets for each frame in the clip.  The
// first entry in the array is the number of frames.  Returns NULL on error.
// This function rewinds the clip to the beginning.
SVVIDEOLIB_API int64_t* get_ms_list(ClipStream* stream);
SVVIDEOLIB_API int64_t* get_ms_list2(const char* filename, log_fn_t logFn);
SVVIDEOLIB_API int64_t get_duration(const char* filename, log_fn_t logFn);

// Frees the memory allocated by a call to get_ms_list.
SVVIDEOLIB_API void free_ms_list(int64_t** msListPtr);

SVVIDEOLIB_API const char** get_module_names();
SVVIDEOLIB_API void set_module_trace_level(const char* module, int level);

SVVIDEOLIB_API int videolib_get_hw_devices(const char** ptrStr, int nSize);
SVVIDEOLIB_API int videolib_set_hw_device(const char* dev);

SVVIDEOLIB_API int flush_output(StreamData* data);
SVVIDEOLIB_API void videolib_set_path(int pathType, const char* value);


#endif // VIDEOLIB_H
