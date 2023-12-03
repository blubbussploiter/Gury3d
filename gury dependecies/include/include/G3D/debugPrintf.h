/**
 @file debugPrintf.h
 
 <CODE>debugPrintf(char* fmt ...);</CODE>
 Prints to the debug window (win32) or stderr. 
 
 @maintainer Morgan McGuire, matrix@graphics3d.com
 
 @created 2001-08-26
 @edited  2002-08-07

 Copyright 2000-2006, Morgan McGuire.
 All rights reserved.
 */

#ifndef G3D_DEBUGPRINTF_H
#define G3D_DEBUGPRINTF_H

#include "G3D/platform.h"
#include <stdio.h>
#include <cstdarg>
#include "G3D/format.h"
#include <string>


namespace G3D {

/**
 @fn void __cdecl debugPrintf(const char *fmt ...)
 
  Prints a string from arguments of the style of printf.
  If G3D_DEBUG is not defined, does nothing.  On Windows, the
  string is printed to the output window in MSVC.  On other
  platforms, the string is printed to stderr.
 */

#ifdef G3D_DEBUG

    // This function is inlined so that it can be turned off depending
    // on the linked program's G3D_DEBUG setting (not the G3D_DEBUG setting
    // the library was compiled against).
    inline void __cdecl debugPrintf(const char* fmt ...) {

        va_list argList;
        va_start(argList, fmt);

        #ifdef G3D_WIN32
            const int MAX_STRING_LEN = 1024;
            std::string s = G3D::vformat(fmt, argList);
            // Windows can't handle really long strings sent to
            // the console, so we break the string.
            if (s.size() < MAX_STRING_LEN) {
                OutputDebugStringA(s.c_str());
            } else {
                for (unsigned int i = 0; i < s.size(); i += MAX_STRING_LEN) {
                    std::string sub = s.substr(i, MAX_STRING_LEN);
                    OutputDebugStringA(sub.c_str());
                }
            }
        #else
            vfprintf(stderr, fmt, argList);
        #endif

        va_end(argList);
    }

#else

    // The compiler should optimize away this empty call, unless the
    // arguments are function calls.
    inline void __cdecl debugPrintf(const char* fmt ...) {
        (void)fmt;
    };

#endif

}; // namespace

#endif

