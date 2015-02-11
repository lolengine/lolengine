//
//  Lol Engine
//
//  Copyright © 2010-2015 Sam Hocevar <sam@hocevar.net>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

/*
 * VS 2015 hack: the CRT was fully rewritten, and functions such
 * as _iob_func() and fprintf() have disappeared. Since we link with
 * libSDL which uses these versions, we need to provide them.
 */

#if _MSC_VER >= 1900
#include <cstdio>

extern "C" {

void *__imp___iob_func(void)
{
    return NULL;
}

int __imp_fprintf(FILE *stream, char const *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int ret = vfprintf(stream, fmt, va);
    va_end(va);
    return ret;
}

}
#endif

