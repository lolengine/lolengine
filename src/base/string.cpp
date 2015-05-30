//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdio>

#if defined(_WIN32)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#include <cstdarg>

namespace lol
{

String String::format(char const *format, ...)
{
    String ret;

    va_list ap;
    va_start(ap, format);
    ret = String::vformat(format, ap);
    va_end(ap);

    return ret;
}

String String::vformat(char const *format, va_list ap)
{
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
    /* do not call va_end() if va_copy() wasn't called. */
    va_end(ap2);
#endif

    ((super &)ret).reserve(needed);
    ret.m_count = needed;

    vsnprintf(&ret[0], needed, format, ap);

    return ret;
}

} /* namespace lol */

