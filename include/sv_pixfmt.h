/*****************************************************************************
 *
 * sv_pixfmt.h
 *   Conversion between ffmpeg and videoLib pixel formats.
 *   Only small subset of those is covered, will be added as need.
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

#ifndef SV_PIXFMT_H
#define SV_PIXFMT_H

#ifdef __cplusplus
extern "C" {
#include <libavutil/pixfmt.h>
#include <libavutil/samplefmt.h>
#endif
#include "stream.h"

enum AVPixelFormat         svpfmt_to_ffpfmt     ( int svpfmt,
                                                    enum AVColorRange* colorRange );
enum AVPixelFormat         svpfmt_to_ffpfmt_ext ( int svpfmt,
                                                    enum AVColorRange* colorRange,
                                                    int dstCodec );
int                        ffpfmt_to_svpfmt     ( enum AVPixelFormat ffpfmt,
                                                    enum AVColorRange colorRange );
int                        ffsfmt_to_svsfmt     ( enum AVSampleFormat ffsfmt,
                                                    int* interleaved,
                                                    int* sampleSize );
int                        svsfmt_to_ffsfmt     ( int format,
                                                  int interleaved );

#ifdef __cplusplus
} // extern "C"
#endif


#endif