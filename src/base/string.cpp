//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#include <cstdarg>

#include "core.h"

namespace lol
{

String String::Printf(char const *format, ...)
{
    String ret;

    va_list ap;
    va_start(ap, format);
    ret = String::VPrintf(format, ap);
    va_end(ap);

    return ret;
}

String String::VPrintf(char const *format, va_list ap)
{
#if defined __CELLOS_LV2__
    using std::vsnprintf;
#endif

    String ret;

    va_list ap2;
#if defined va_copy || !defined _MSC_VER
    /* Visual Studio 2010 does not support va_copy. */
    va_copy(ap2, ap);
#else
    ap2 = ap;
#endif

    /* vsnprintf() tells us how many character we need, and we need to
     * add one for the terminating null byte. */
    size_t needed = vsnprintf(nullptr, 0, format, ap2) + 1;

#if defined va_copy || !defined _MSC_VER
    /* do not call va_end() if va_copy() wasn’t called. */
    va_end(ap2);
#endif

    ((Super &)ret).Reserve(needed);
    ret.m_count = needed;

    vsnprintf(&ret[0], needed, format, ap);

    return ret;
}

} /* namespace lol */

