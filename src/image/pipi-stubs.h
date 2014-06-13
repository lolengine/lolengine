/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2006 Sam Hocevar <sam@zoy.org>
 *                All Rights Reserved
 *
 *  $Id$
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the Do What The Fuck You Want To
 *  Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

/*
 *  This file contains replacements for commonly found object types and
 *  function prototypes that are sometimes missing.
 */

#ifndef __PIPI_STUBS_H__
#define __PIPI_STUBS_H__

/* errno handling */
#if defined HAVE_ERRNO_H && !defined __KERNEL__
#   include <errno.h>
static inline void seterrno(int e) { errno = e; }
static inline int geterrno(void) { return errno; }
#else
#   define seterrno(x) do { (void)(x); } while(0)
#   define geterrno(x) 0
#endif

/* debug messages */
#if defined DEBUG && !defined __KERNEL__
#   include <stdio.h>
#   include <stdarg.h>
static inline void debug(const char *format, ...)
{
    int saved_errno = geterrno();
    va_list args;
    va_start(args, format);
    fprintf(stderr, "** libpipi debug ** ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
    seterrno(saved_errno);
}
#else
#   define debug(format, ...) do {} while(0)
#endif

/* hton16() and hton32() */
#if defined HAVE_HTONS && !defined __KERNEL__
#   if defined HAVE_ARPA_INET_H
#       include <arpa/inet.h>
#   elif defined HAVE_NETINET_IN_H
#       include <netinet/in.h>
#   endif
#   define hton16 htons
#   define hton32 htonl
#else
#   if defined __KERNEL__
        /* Nothing to do */
#   elif defined HAVE_ENDIAN_H
#       include <endian.h>
#   endif
static inline uint16_t hton16(uint16_t x)
{
    /* This is compile-time optimised with at least -O1 or -Os */
#if defined HAVE_ENDIAN_H
    if(__BYTE_ORDER == __BIG_ENDIAN)
#else
    uint32_t const dummy = 0x12345678;
    if(*(uint8_t const *)&dummy == 0x12)
#endif
        return x;
    else
        return (x >> 8) | (x << 8);
}

static inline uint32_t hton32(uint32_t x)
{
    /* This is compile-time optimised with at least -O1 or -Os */
#if defined HAVE_ENDIAN_H
    if(__BYTE_ORDER == __BIG_ENDIAN)
#else
    uint32_t const dummy = 0x12345678;
    if(*(uint8_t const *)&dummy == 0x12)
#endif
        return x;
    else
        return (x >> 24) | ((x >> 8) & 0x0000ff00)
                | ((x << 8) & 0x00ff0000) | (x << 24);
}
#endif

#endif /* __PIPI_STUBS_H__ */

