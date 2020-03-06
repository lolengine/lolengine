//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdio>
#include <cstdlib>

#if defined(_WIN32)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
#endif

#if __ANDROID__
#   include <android/log.h>
#   include <unistd.h> /* for gettid() */
#elif __NX__
#   include "../private/nx/nx-log.h"
#else
#   include <cstdarg>
#endif

namespace lol
{

/*
 * Public log class
 */

void msg::debug(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    helper(message_type::debug, fmt, ap);
    va_end(ap);
}

void msg::info(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    helper(message_type::info, fmt, ap);
    va_end(ap);
}

void msg::warn(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    helper(message_type::warning, fmt, ap);
    va_end(ap);
}

void msg::error(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    helper(message_type::error, fmt, ap);
    va_end(ap);
}

/*
 * Private helper function
 */

void msg::helper(message_type type, char const *fmt, va_list ap)
{
    /* Unless this is a debug build, ignore debug messages unless
     * the LOL_DEBUG environment variable is set. */
#if !defined LOL_BUILD_DEBUG && !_DEBUG
    if (type == message_type::debug)
    {
        static bool const disable_debug = sys::getenv("LOL_DEBUG").size() > 0;
        if (disable_debug)
            return;
    }
#endif

#if __ANDROID__
    static int const prio[] =
    {
        ANDROID_LOG_DEBUG,
        ANDROID_LOG_INFO,
        ANDROID_LOG_WARN,
        ANDROID_LOG_ERROR
    };

    std::string buf = vformat(fmt, ap);
    __android_log_print(prio[(int)type], "LOL", "[%d] %s", (int)gettid(), &buf[0]);

#elif __NX__
    nx::helper(type, fmt, ap);
#else
    static char const * const prefix[] =
    {
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
    };

#   if defined _WIN32
    std::string buf = std::string(prefix[(int)type]) + ": " + vformat(fmt, ap);

    array<WCHAR> widechar;
    widechar.resize(buf.length() + 1);
    MultiByteToWideChar(CP_UTF8, 0, buf.c_str(), (int)buf.length() + 1, widechar.data(), widechar.count());
    OutputDebugStringW(widechar.data());
#   elif defined __EMSCRIPTEN__
    fprintf(stdout, "%s: ", prefix[(int)type]);
    vfprintf(stdout, fmt, ap);
    fflush(stdout);
#   else
    fprintf(stderr, "%s: ", prefix[(int)type]);
    vfprintf(stderr, fmt, ap);
#   endif
#endif
}

} /* namespace lol */

