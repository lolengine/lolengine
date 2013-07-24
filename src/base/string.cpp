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
    ret = String::Printf(format, ap);
    va_end(ap);

    return ret;
}

String String::Printf(char const *format, va_list ap)
{
#if defined __CELLOS_LV2__
    using std::vsnprintf;
#endif

    String ret;

    /* Visual Studio 2010 does not support it va_copy. */
#if defined _MSC_VER
#   undef va_copy
#   define va_copy(dst, src) (dst = src)
#endif
    va_list ap2;
    va_copy(ap2, ap);
#if defined _MSC_VER
#   undef va_copy
#endif

    /* vsnprintf() tells us how many character we need, and we need to
     * add one for the terminating null byte. */
    size_t needed = vsnprintf(nullptr, 0, format, ap2) + 1;

    ((Super &)ret).Reserve(needed);
    ret.m_count = needed;

    vsnprintf(&ret[0], needed, format, ap);

    return ret;
}

} /* namespace lol */

