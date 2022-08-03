/*****************************************************************************
 *
 * sv_ffmpeg.cpp
 *   A collection of miscellaneous ffmpeg-related methods
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

#ifndef __SV_FFMPEG_H
#define __SV_FFMPEG_H

#include "logging.h"
#include "videolibExports.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersrc.h>
#include <libavfilter/buffersink.h>
#include <libavutil/mem.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>




// To surface FFmpeg logs. Process-global.
SVVIDEOLIB_API int  ffmpeg_init();
SVVIDEOLIB_API void ffmpeg_close();
SVVIDEOLIB_API int  ffmpeg_log_open(log_fn_t logFn, int logBufSize);
SVVIDEOLIB_API int  ffmpeg_log_pause();
SVVIDEOLIB_API int  ffmpeg_log_resume();
SVVIDEOLIB_API void ffmpeg_log_close();

#if LIBAVUTIL_VERSION_MAJOR >= 56 // n4.0 or later
    #define FF_FRAME_PTS(f) f->pts
#else
    #define FF_FRAME_PTS(f) f->pkt_pts
#endif


#ifdef __cplusplus
}  /* extern "C" */
#endif

SVVIDEOLIB_API AVIOContext* ffmpeg_create_buffered_io(const char* filename);
SVVIDEOLIB_API int          ffmpeg_close_buffered_io(AVIOContext* ctx);
SVVIDEOLIB_API int          ffmpeg_flush_buffered_io(log_fn_t logFn, const char* src, const char* dst);

extern AVRational AVRATIONAL_MS;

#endif // __FFMPEGLOG_H
