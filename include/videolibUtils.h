/*****************************************************************************
 *
 * videolibUtils.h
 *   collection of random methods that didn't fit anywhere else
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


#ifndef VIDEOLIB_UTILS_H
#define VIDEOLIB_UTILS_H


#if SIGHTHOUND_VIDEO
#ifdef __cplusplus
extern "C" {
#endif
#include "videolib.h"
#ifdef __cplusplus
} // extern "C" {
#endif
#endif // #if SIGHTHOUND_VIDEO

#include "nalu.h"
#include "sv_pixfmt.h"
#include "streamprv.h"
#include "sv_ffmpeg.h"

#define CHECKPOINT() log_err(data->logFn, "CP %d", __LINE__);
#define CHECKPOINT_L() log_err(logFn, "CP %d", __LINE__);

#define _kDefAlign 1


#if SIGHTHOUND_VIDEO
#define videolibapi_free_proc_frame free_frame_data     // maintain compatibility with .py module
void        videolibapi_free_proc_frame			( FrameData** procFrameData );
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if SIGHTHOUND_VIDEO
FrameData*  videolibutils_prepare_proc_frame    ( StreamData* data,
                                                    frame_obj* frame);
FrameData*  videolibutils_get_empty_proc_frame  ( );
#endif

// Calculate the actual output size of a video based on the input size and
// requested output sizes.
#define     videolibapi_preserve_aspect_ratio preserve_aspect_ratio

SVVIDEOLIB_API
void        videolibapi_preserve_aspect_ratio   ( int origWidth,
                                                  int origHeight,
                                                  int* targetWidth,
                                                  int* targetHeight,
                                                  int forceAlignment);


uint8_t*    videolibapi_spspps_to_extradata     ( uint8_t* sps,
                                                  int spsSize,
                                                  uint8_t* pps,
                                                  int ppsSize,
                                                  int ffmpegAlloc,
                                                  int* extradataSize );

int         videolibapi_extradata_to_spspps     ( uint8_t *extradata,
                                                  int extradataSize,
                                                  uint8_t** sps,
                                                  size_t* spsSize,
                                                  uint8_t** pps,
                                                  size_t* ppsSize );

int         videolibapi_get_ffmpeg_rotation     ( AVFormatContext* f, int stream);

#undef av_err2str
#define av_err2str(errnum) av_make_error_string((char*)__builtin_alloca(AV_ERROR_MAX_STRING_SIZE), AV_ERROR_MAX_STRING_SIZE, errnum)

#define ffframe_to_svframe(ffframe, svframe)\
          av_image_copy_to_buffer ( (unsigned char*)svframe->data,\
                        svframe->allocSize,\
                        ffframe->data,\
                        ffframe->linesize,\
                        (enum AVPixelFormat)ffframe->format,\
                        ffframe->width,\
                        ffframe->height,\
                        _kDefAlign)



#ifdef __cplusplus
} // extern "C" {
#endif

#endif // VIDEOLIB_UTILS_H
