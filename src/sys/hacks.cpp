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

/*
 * VS 2015 hack: the CRT was fully rewritten, and functions such
 * as _iob_func() and fprintf() have disappeared. Since we link with
 * libSDL which uses these versions, we need to provide them.
 */

#if _MSC_VER >= 1900
#include <cstdio>

extern "C" {

#if _M_X64
void *__imp___iob_func(void)
#else
void *_imp____iob_func(void)
#endif
{
    return NULL;
}

#if _M_X64
int __imp_fprintf(FILE *stream, char const *fmt, ...)
#else
int _imp__fprintf(FILE *stream, char const *fmt, ...)
#endif
{
    va_list va;
    va_start(va, fmt);
    int ret = vfprintf(stream, fmt, va);
    va_end(va);
    return ret;
}

}
#endif

