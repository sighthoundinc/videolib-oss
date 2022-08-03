/*****************************************************************************
 *
 * streamFactories.h
 *   Definition of factory methods for various stream graph nodes implemented
 *   by videoLib.
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

#ifndef STREAM_FACTORIES_H
#define STREAM_FACTORIES_H

#include "stream.h"
#include "videolibExports.h"




SVVIDEOLIB_API stream_api_t*     get_ffmpeg_demux_api          ();     // ffmpeg demux
SVVIDEOLIB_API stream_api_t*     get_ffsink_stream_api         ();     // recording node
SVVIDEOLIB_API stream_api_t*     get_ffdec_stream_api          ();     // ffmpeg-based decoder
SVVIDEOLIB_API stream_api_t*     get_ffenc_stream_api          ();     // ffmpeg-based encoder
#if LOCAL_CAMERA_SUPPORT
SVVIDEOLIB_API stream_api_t*     get_lvl_stream_api            ();     // local (USB) camera demux
#endif
// SVVIDEOLIB_API stream_api_t*     get_live555_stream_api        ();
SVVIDEOLIB_API stream_api_t*     get_live555_demux_stream_api  ();
SVVIDEOLIB_API stream_api_t*     get_input_iterator_api        ();     // live555-based demux
SVVIDEOLIB_API stream_api_t*     get_resize_factory_api        ();     // implementation of resize and color conversion, "routing" to the fastest option for the platform
SVVIDEOLIB_API stream_api_t*     get_resize_filter_api         ();     // ffmpeg based resize and color conversion
SVVIDEOLIB_API stream_api_t*     get_splitter_api              ();     // splitter, allowing for branching of the streams
SVVIDEOLIB_API stream_api_t*     get_limiter_filter_api        ();     // fps limiter
SVVIDEOLIB_API stream_api_t*     get_mmap_sink_api             ();     // memory map node for video frames
SVVIDEOLIB_API stream_api_t*     get_ff_filter_api             ();
SVVIDEOLIB_API stream_api_t*     get_tc_api                    ();
SVVIDEOLIB_API stream_api_t*     get_metainject_filter_api     ();
#ifdef AUDIO_RENDER_SUPPORT
SVVIDEOLIB_API stream_api_t*     get_audio_renderer_api        ();
SVVIDEOLIB_API stream_api_t*     get_audio_renderer_pa_api     ();
#endif
SVVIDEOLIB_API stream_api_t*     get_recorder_sync_api         ();
SVVIDEOLIB_API stream_api_t*     get_jitbuf_stream_api         ();
SVVIDEOLIB_API stream_api_t*     get_pixelate_filter_api       ();
SVVIDEOLIB_API stream_api_t*     get_resample_filter_api       ();
#ifdef WITH_IPP
SVVIDEOLIB_API stream_api_t*     get_ipp_resize_filter_api     ();
SVVIDEOLIB_API stream_api_t*     get_ipp_cc_filter_api         ();
#endif
SVVIDEOLIB_API stream_api_t*     get_live555_demux_stream_api  ();

SVCORE_API stream_api_t*         get_default_stream_api        ();

#ifdef WIN32
SVVIDEOLIB_API stream_api_t*     get_mfdec_stream_api          ();
#define get_audiodec_api get_mfdec_stream_api
#define get_audioenc_api get_mfdec_stream_api
#else
#define get_audiodec_api get_ffdec_stream_api
#define get_audioenc_api get_ffenc_stream_api
#endif

#if !SIGHTHOUND_VIDEO
SVVIDEOLIB_API stream_api_t*     get_ffmpeg_stream_api         ();     // combo ffmpeg decoder + demux
SVVIDEOLIB_API stream_api_t*     get_filter_inserter_api       ();
SVVIDEOLIB_API stream_api_t*     get_frame_injector_stream_api ();
SVVIDEOLIB_API stream_api_t*     get_rev_reader_filter_api     ();
#endif


#define default_set_source                  get_default_stream_api()->set_source
#define default_set_log_cb                  get_default_stream_api()->set_log_cb
#define default_get_name                    get_default_stream_api()->get_name
#define default_find_element                get_default_stream_api()->find_element
#define default_remove_element              get_default_stream_api()->remove_element
#define default_insert_element              get_default_stream_api()->insert_element
#define default_print_pipeline              get_default_stream_api()->print_pipeline
#define default_set_param                   get_default_stream_api()->set_param
#define default_get_param                   get_default_stream_api()->get_param
#define default_open_in                     get_default_stream_api()->open_in
#define default_seek                        get_default_stream_api()->seek
#define default_get_width                   get_default_stream_api()->get_width
#define default_get_height                  get_default_stream_api()->get_height
#define default_get_pixel_format            get_default_stream_api()->get_pixel_format
#define default_read_frame                  get_default_stream_api()->read_frame
#define default_close                       get_default_stream_api()->close



#endif // STREAM_FACTORIES_H