/*****************************************************************************
 *
 * nalu.h
 *   NAL units primitives - minimal bitstream parsing needed.
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

#ifndef __NALU_H
#define __NALU_H

#include "stream.h"

enum NALUnitTypes {
     kNALCodedSlice = 1,
     kNALIFrame = 5,
     kNALPPS = 8,
     kNALSPS = 7
};



#ifdef __cplusplus
extern "C" {
#endif



uint8_t*    videolibapi_find_next_nal           ( uint8_t* data,
                                                  int* size,
                                                  size_t* nalHdrSize,
                                                  uint8_t* nalType,
                                                  fn_stream_log logCb );
int         videolibapi_extract_nalu            ( uint8_t* data,
                                                  size_t size,
                                                  uint8_t nalTypeWanted,
                                                  uint8_t** mem,
                                                  size_t* naluSize,
                                                  size_t* remainingSize,
                                                  fn_stream_log logCb);
int         videolibapi_contains_idr_frame      ( uint8_t* data,
                                                  size_t size,
                                                  fn_stream_log logCb );
int         videolibapi_contained_nalu          ( uint8_t* data,
                                                  size_t size,
                                                  fn_stream_log logCb );
uint8_t     videolibapi_get_nalu_type           ( uint8_t* data,
                                                  size_t size );
void        videolibapi_parse_sps               ( unsigned char * pStart,
                                                  unsigned short _nLen,
                                                  int* w, int* h, int* profile,
                                                  int* level );


// Coded slices can be 1-5, 19-21
#define CONTAINS_TIMED_DATA(nalu) ((nalu&0x38001F)!=0)


#ifdef __cplusplus
} // extern "C" {
#endif

#endif // __NALU_H

