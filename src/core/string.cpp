//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
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
#if defined __CELLOS_LV2__
    using std::vsnprintf;
#endif

    String ret;
    va_list ap;

    /* vsnprintf() tells us how many character we need, and we need to
     * add one for the terminating null byte. */
    va_start(ap, format);
    size_t needed = vsnprintf(NULL, 0, format, ap) + 1;
    va_end(ap);

    ((Super &)ret).Reserve(needed);
    ret.m_count = needed;

    /* We don’t use va_copy because Visual Studio 2010 does not support it. */
    va_start(ap, format);
    vsnprintf(&ret[0], needed, format, ap);
    va_end(ap);

    return ret;
}

} /* namespace lol */

