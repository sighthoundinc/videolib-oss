/*****************************************************************************
 *
 * sv_os.h
 *   OS, time, file-system, etc primitives used by videolib.
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

#ifndef SV_OS_H
#define SV_OS_H

/* Consolidate Windows/MINGW definitions */
#if defined(_WIN32) || defined(MINGW) || defined(__MINGW32__)
    #ifndef _WIN32
        #define _WIN32 1
    #endif
    #if defined(MINGW) || defined(__MINGW32__)
        #ifndef __MINGW32__
            #define __MINGW32__ 1
        #endif
    #endif
#endif


#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #if defined (__MINGW32__)
        #include <stdint.h>
    #else
        #include <malloc.h>
    #endif
#else
    #include <string.h>
    #include <stdint.h>
    #include <signal.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #ifndef __APPLE__
        #include <sys/sysinfo.h>
    #endif
    #include <pthread.h>
#endif

#ifndef _MSC_VER
    // pthreads are not available under MSVC
    #include <pthread.h>
    #define SV_HAS_PTHREADS 1
#else
    #define SV_HAS_PTHREADS 0
#endif


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#ifdef __cplusplus
    #define SV_CC extern "C"
#else
    #define SV_CC
#endif

#if _MSC_VER
    #ifdef SVCORE_EXPORTS
        #define SVCORE_VISIBILITY __declspec( dllexport )
    #else
        #define SVCORE_VISIBILITY __declspec( dllimport )
    #endif
#else
    #ifdef SVCORE_EXPORTS
        #define SVCORE_VISIBILITY __attribute__ ((visibility ("default")))
    #else
        #define SVCORE_VISIBILITY
    #endif
#endif

#define SVCORE_API SV_CC SVCORE_VISIBILITY

// ----------------------------------------- Basic types ----------------------
#if defined(_WIN32)
    typedef long long           INT64_T; // MS ...
    typedef unsigned long long  UINT64_T;
    typedef long                THREADID_T;
    #if !defined (__MINGW32__)
        typedef unsigned long       ssize_t;
        typedef unsigned char       uint8_t;
        typedef long long           int64_t;
        typedef unsigned long long  uint64_t;
    #endif
    #define I64FMT      "%I64d"
    #define PATH_SEPA   '\\'
#else
    typedef int64_t     INT64_T; // normal people (e.g. C99)
    typedef uint64_t    UINT64_T;
    typedef pthread_t   THREADID_T;
    #define I64FMT      "%lld"
    #define PATH_SEPA   '/'
#endif
typedef char     CHAR_T;
typedef long     ATOMIC_T;
typedef ATOMIC_T REF_T;

// ------------------------ Cross-OS method name discrepancies ----------------
#ifndef _WIN32
    #define _stricmp strcasecmp
    #define _strnicmp strncasecmp
    #define _stristr strcasestr
#else
    #ifndef __MINGW32__
        #define __builtin_alloca _malloca
    #endif
    #ifndef __cplusplus
        #define inline __inline
    #endif
    #define _stristr sv_strcasestr
    char *sv_strcasestr(char *str, const char *sub);
#endif

// ------------------------ String formatting for c++ only --------------------
#ifdef __cplusplus
    #include <sstream>
    #include <iomanip>
    #ifndef SV_MODULE_NAME
        #define SV_MODULE_NAME "videolib"
    #endif
    #define _STR( msg )   ( ((std::ostringstream&)(std::ostringstream() << \
                            std::skipws << \
                            msg << \
                            '\0')).str().c_str() )
    #define _FMT( msg )   ( ((std::ostringstream&)(std::ostringstream() << \
                            std::skipws << \
                            SV_MODULE_NAME << ": " << \
                            msg << \
                            '\0')).str().c_str() )
#endif

// ----------------------------------------- Time utilities -------------------
SVCORE_API UINT64_T sv_time_get_current_epoch_time();
SVCORE_API UINT64_T sv_time_get_elapsed_time(UINT64_T from);
SVCORE_API UINT64_T sv_time_get_time_diff(UINT64_T from, UINT64_T to);
SVCORE_API UINT64_T sv_time_timeval_to_ms(struct timeval* time);
SVCORE_API void     sv_time_ms_to_timeval(UINT64_T ms, struct timeval* time);
SVCORE_API UINT64_T sv_time_get_timeval_diff_ms(struct timeval* from, struct timeval* to);
#ifdef __cplusplus
class sv_stopwatch
{
    UINT64_T _start;
    UINT64_T _stop;
    bool     _stopped;
public:
    sv_stopwatch() { reset(); }
    INT64_T reset() { _start = sv_time_get_current_epoch_time(); _stopped=false; return _start; }
    INT64_T diff()  { return _stopped?sv_time_get_time_diff(_start,_stop):sv_time_get_elapsed_time(_start); }
    INT64_T stop()  { _stopped=true; _stop = sv_time_get_current_epoch_time(); return diff(); }
};
#endif


// ----------------------------------------- Atomic utilities -----------------
SVCORE_API ATOMIC_T   sv_atomic_inc(ATOMIC_T* val);
SVCORE_API ATOMIC_T   sv_atomic_dec(ATOMIC_T* val);
SVCORE_API ATOMIC_T   sv_atomic_add(ATOMIC_T* val, ATOMIC_T toAdd);
#define ATOMIC_INCREMENT(x) (sv_atomic_inc(&x))
#define ATOMIC_DECREMENT(x) (sv_atomic_dec(&x))


// ----------------------------------------- Thread primitives ----------------
SVCORE_API THREADID_T sv_get_thread_id();

typedef struct sv_mutex   sv_mutex;
typedef struct sv_rwlock  sv_rwlock;

typedef struct sv_event   sv_event;
typedef struct sv_thread  sv_thread;

SVCORE_API sv_rwlock* sv_rwlock_create();
SVCORE_API void       sv_rwlock_destroy(sv_rwlock**);
SVCORE_API void       sv_rwlock_lock_read(sv_rwlock*);
SVCORE_API void       sv_rwlock_lock_write(sv_rwlock*);
SVCORE_API void       sv_rwlock_unlock_read(sv_rwlock*);
SVCORE_API void       sv_rwlock_unlock_write(sv_rwlock*);


SVCORE_API sv_mutex* sv_mutex_create();
SVCORE_API void      sv_mutex_destroy(sv_mutex**);
SVCORE_API void      sv_mutex_enter(sv_mutex*);
SVCORE_API void      sv_mutex_exit(sv_mutex*);


SVCORE_API sv_event* sv_event_create(int autoreset, int triggered_at_start);
SVCORE_API void      sv_event_destroy(sv_event**);
SVCORE_API void      sv_event_set(sv_event*);
SVCORE_API void      sv_event_reset(sv_event*);
SVCORE_API int       sv_event_wait(sv_event*, int timeoutMs);

typedef void*     (*sv_thread_func)(void* context);
SVCORE_API sv_thread* sv_thread_create(sv_thread_func func, void* context);
SVCORE_API int        sv_thread_destroy(sv_thread**);
SVCORE_API int        sv_thread_is_running(sv_thread*);

// ----------------------------------------- I/O ------------------------------
typedef struct sv_mmap   sv_mmap;

SVCORE_API sv_mmap*  sv_open_mmap(const char* location, size_t size);
SVCORE_API void      sv_close_mmap(sv_mmap** pMap);
SVCORE_API uint8_t*  sv_mmap_get_ptr(sv_mmap* pMap);
SVCORE_API size_t    sv_mmap_get_size(sv_mmap* pMap);


// ----------------------------------------- Environment  ---------------------
SVCORE_API int       sv_get_env_var(const char* name, char* value, size_t* bufSize);
SVCORE_API int       sv_get_int_env_var(const char* name, int defaultVal);
SVCORE_API int       sv_get_trace_level(const char* varName);

SVCORE_API void      sv_sleep(UINT64_T ms);
SVCORE_API FILE*     sv_open_file(const char* name, const char* mode);
SVCORE_API int       sv_rename_file(const char* src, const char* dst);

// ----------------------------------------- Strings  -------------------------
SVCORE_API const char* sv_sanitize_uri(const char* uri, char* buffer, size_t bufSize);
typedef void     (*fn_sv_print_buffer_line_cb)(const char* line, void* ctx);
SVCORE_API void      sv_print_buffer(char* buffer, size_t size, fn_sv_print_buffer_line_cb cb, void* ctx);


// ----------------------------------------- Helper methods  ------------------
SVCORE_API void      sv_freep(void* ptr);
SVCORE_API int       sv_get_cpu_count();

// ----------------------------------------- Libraries  -----------------------
typedef struct sv_lib sv_lib;
SVCORE_API char*     sv_get_module_path(const char* name, void* method);
SVCORE_API char*     sv_get_module_dir(const char* name, void* method);
enum PathType {
    DataPath = 0,
    TempPath = 1
};
SVCORE_API void      sv_set_path(int pathType, const char* value);
SVCORE_API char*     sv_get_path(int pathType);
SVCORE_API sv_lib*   sv_load(const char* library);
SVCORE_API void*     sv_get_sym(sv_lib* handle, const char* name);
SVCORE_API void      sv_unload(sv_lib** handle);

SVCORE_API int       sv_transcode_audio();

#endif

