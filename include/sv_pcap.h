/*****************************************************************************
 *
 * sv_pcap.h
 *   Code for collecting network capture on demand. Used by SV
 *   for troubleshooing of camera connection on demand.
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

#ifndef SV_PCAP_H
#define SV_PCAP_H

#include "sv_os.h"

#ifdef SVPCAP_STATIC
    #define SVPCAP_VISIBILITY
#elif _MSC_VER
    #ifdef SVPCAP_EXPORTS
        #define SVPCAP_VISIBILITY __declspec( dllexport )
    #else
        #define SVPCAP_VISIBILITY __declspec( dllimport )
    #endif
#else
    #ifdef SVPCAP_EXPORTS
        #define SVPCAP_VISIBILITY __attribute__ ((visibility ("default")))
    #else
        #define SVPCAP_VISIBILITY
    #endif
#endif

#define SVPCAP_API SV_CC SVPCAP_VISIBILITY
typedef struct sv_pcap    sv_pcap;

enum SvPcapLogLevel { svpllTrace, svpllDebug, svpllInfo, svpllError };

typedef void     (*sv_pcap_log_cb_t)(void* ctx, int level, const char* msg);
typedef sv_pcap* (*sv_capture_traffic_t)(const char* uri,
                                   const char* saveTo,
                                   size_t maxCaptureMsec,
                                   size_t maxCaptureSizeBytes,
                                   sv_pcap_log_cb_t logCb,
                                   void* logCtx);
typedef void     (*sv_stop_capture_t)(sv_pcap** pcap);
typedef int      (*sv_capture_possible_t)();


#endif