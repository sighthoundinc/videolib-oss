/*****************************************************************************
 *
 * localVideo.h
 *   API for the portion of Sighthound Video integrating with local/USB camera
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

#ifndef LOCALVIDEO_H
#define LOCALVIDEO_H

#include "logging.h"

#ifdef __cplusplus
    #define SVLVL_CC extern "C"
#else
    #define SVLVL_CC
#endif

#ifdef SVLVL_STATIC
    #define SVLVL_VISIBILITY
#elif _MSC_VER
    #ifdef SVLVL_EXPORTS
        #define SVLVL_VISIBILITY __declspec( dllexport )
    #else
        #define SVLVL_VISIBILITY __declspec( dllimport )
    #endif
#else
    #ifdef SVLVL_EXPORTS
        #define SVLVL_VISIBILITY __attribute__ ((visibility ("default")))
    #else
        #define SVLVL_VISIBILITY
    #endif
#endif

#define SVLVL_API SVLVL_CC SVLVL_VISIBILITY


typedef void* LocalVideoHandle;

#define PIXEL_FORMAT_RGB24      1
#define PIXEL_FORMAT_YUYV422    2

/* Define booleans */
typedef int BOOL;
#define FALSE 0
#define TRUE 1

typedef struct Dimensions {
    int width;
    int height;
}Dimensions;

// Must be called before any of the other APIs, and returns a
// handle to the instance to be passed into other the other functions.
// The two exceptions are lvlSetVerbose() and lvlListDevices(),
// which can be called without calling lvlNew().
SVLVL_API LocalVideoHandle lvlNew(log_fn_t logFn, const char* dataDir);

// Frees handle, frees data structures and cleans up
SVLVL_API void lvlDelete(LocalVideoHandle lvlH);

// Turns on verbose output, especially during setup, for debugging purposes
SVLVL_API void lvlSetVerbose(BOOL verbose);

// Caches a list of the cameras names on the local machine and returns the
// number of cameras available. Also caches the number of supported resolutions
// per camera, and the list of supported resolutions per camera. All of this
// information can then be accessed using lvlGetDeviceName(),
// lvlGetNumSupportedResolutionsOfDevice(), and lvlGetSupportedResolutionPairOfDevice().
SVLVL_API int lvlListDevices(log_fn_t logFn);
SVLVL_API int lvlListDevicesWithoutResolutionList(log_fn_t logFn);

// Returns a camera name given a device ID.  You must call lvlListDevices()
// first and after any camera insertions or removals.
SVLVL_API char* lvlGetDeviceName(int deviceID);

// Returns the number of supported resolutions of the given device. You must
// call lvlListDevices() first and after any camera inserstions or removals.
SVLVL_API int lvlGetNumSupportedResolutionsOfDevice(int deviceID);

// Returns a single pair of dimensions (width and height) of the given device
// and dimension index.  You must call lvlListDevices() first and after
// any camera insertions or removals.
SVLVL_API Dimensions* lvlGetSupportedResolutionPairOfDevice(int deviceID, int resPairIdx);

// Given a camera's device ID, initialize that camera for streaming. Needs
// to be called before lvlGetPixels().
SVLVL_API BOOL lvlSetupDevice(LocalVideoHandle lvlH, int deviceID, int width, int height);

// Checks whether the device has been set up, *for this instance of the
// library*.  Does not tell you whether another process has the camera open.
SVLVL_API BOOL lvlIsDeviceSetup(LocalVideoHandle lvlH, int deviceID);

// Returns the width of the video frames that will be returned by lvlGetPixels.
SVLVL_API int lvlGetWidth(LocalVideoHandle lvlH, int deviceID);

// Returns the height of the video frames that will be returned by lvlGetPixels.
SVLVL_API int lvlGetHeight(LocalVideoHandle lvlH, int deviceID);

// Returns the total size in bytes of the video frames that will be returned
// by lvlGetPixels.
SVLVL_API int lvlGetSize(LocalVideoHandle lvlH, int deviceID);

// Returns the pixel format of the video frames that will be returned, using
// FFmpeg's definitions in libavutil/avutil.h.
SVLVL_API int lvlGetPixelFormat();

// Checks whether there is a new frame available.  Note that this is not
// useful on Mac, because currently lvlGetPixels calls this internally
// on that platform.  Easily fixed if we need it.
SVLVL_API BOOL lvlIsFrameNew(LocalVideoHandle lvlH, int deviceID);

// Returns the data of the latest frame into the passed-in buffer.  The buffer
// must be at least lvlGetSize() big and allocated and freed by the caller.
// The booleans are not supported on Mac. Returns true if successful.
SVLVL_API BOOL lvlGetPixels (LocalVideoHandle lvlH, int id, unsigned char* pixels,
                   BOOL flipRedAndBlue, BOOL flipImage);

// Shows a settings window UI that the user can use to configure brightness,
// etc for the camera.  Blocks on Mac but not Windows.  To use in
// production code, would probably require a little more work.
SVLVL_API void lvlShowSettingsWindow(LocalVideoHandle lvlH, int deviceID);

// Must be called after lvlSetupDevice() and before lvlDelete().  Shuts down
// the camera and frees it for use by other programs.
SVLVL_API void lvlStopDevice(LocalVideoHandle lvlH, int deviceID);


#endif  /* LOCALVIDEO_H */

