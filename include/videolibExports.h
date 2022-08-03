/*****************************************************************************
 *
 * videolibExports.h
 *   Export qualifier definitions.
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

#ifndef VIDEOLIB_EXPORTS_H
#define VIDEOLIB_EXPORTS_H

#ifdef __cplusplus
    #define SV_CC extern "C"
#else
    #define SV_CC
#endif

#ifdef _MSC_VER
    #ifdef SVVIDEOLIB_EXPORTS
        #define SVVIDEOLIB_VISIBILITY __declspec( dllexport )
    #else
        #define SVVIDEOLIB_VISIBILITY __declspec( dllimport )
    #endif
#else
    #ifdef SVVIDEOLIB_EXPORTS
        #define SVVIDEOLIB_VISIBILITY __attribute__ ((visibility ("default")))
    #else
        #define SVVIDEOLIB_VISIBILITY
    #endif
#endif

#define SVVIDEOLIB_API SV_CC SVVIDEOLIB_VISIBILITY

#endif // VIDEOLIB_EXPORTS_H
