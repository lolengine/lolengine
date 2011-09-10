//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cmath>
#include <cstdio>

#if defined __ANDROID__
#   include <android/log.h>
#else
#   include <cstdarg>
#endif

#include "core.h"

namespace lol
{

/*
 * Public Log class
 */

void Log::Debug(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
#if defined __ANDROID__
    __android_log_vprint(ANDROID_LOG_DEBUG, "LOL", fmt, ap);
#else
    fprintf(stderr, "DEBUG: ");
    vfprintf(stderr, fmt, ap);
#endif
    va_end(ap);
}

void Log::Info(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
#if defined __ANDROID__
    __android_log_vprint(ANDROID_LOG_INFO, "LOL", fmt, ap);
#else
    fprintf(stderr, "INFO: ");
    vfprintf(stderr, fmt, ap);
#endif
    va_end(ap);
}

void Log::Warn(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
#if defined __ANDROID__
    __android_log_vprint(ANDROID_LOG_WARN, "LOL", fmt, ap);
#else
    fprintf(stderr, "WARN: ");
    vfprintf(stderr, fmt, ap);
#endif
    va_end(ap);
}

void Log::Error(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
#if defined __ANDROID__
    __android_log_vprint(ANDROID_LOG_ERROR, "LOL", fmt, ap);
#else
    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, fmt, ap);
#endif
    va_end(ap);
}

} /* namespace lol */

