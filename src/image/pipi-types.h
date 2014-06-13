/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
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
 *  This file contains definitions for the C99 integer types.
 */

#ifndef __PIPI_TYPES_H__
#define __PIPI_TYPES_H__

#ifndef PIPI_TYPES
#   define PIPI_TYPES 1
#endif

/* mode 1: standard <stdint.h> header is present, just include it */
#if PIPI_TYPES == 1
#   include <stdint.h>
#   include <unistd.h>

/* mode 2: standard <inttypes.h> header is present, just include it */
#elif PIPI_TYPES == 2
#   include <inttypes.h>
#   include <unistd.h>

/* mode 3: <windows.h> indicates Win32, only (u)intptr_t is present
 * FIXME: Win64 probably doesn't work that way */
#elif PIPI_TYPES == 3
#include <windows.h>

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

typedef int ssize_t;
typedef unsigned int size_t;

/* fallback: nothing is known, we assume the platform is 32-bit and
 * sizeof(long) == sizeof(void *). We don't typedef directly because we
 * have no idea what other typedefs have already been made. */
#else
typedef signed char _pipi_int8_t;
typedef signed short _pipi_int16_t;
typedef signed long int _pipi_int32_t;
typedef signed long long int _pipi_int64_t;
#   undef int8_t
#   define int8_t _pipi_int8_t
#   undef int16_t
#   define int16_t _pipi_int16_t
#   undef int32_t
#   define int32_t _pipi_int32_t
#   undef int64_t
#   define int64_t _pipi_int64_t

typedef unsigned char _pipi_uint8_t;
typedef unsigned short _pipi_uint16_t;
typedef unsigned long int _pipi_uint32_t;
typedef unsigned long long int _pipi_uint64_t;
#   undef uint8_t
#   define uint8_t _pipi_uint8_t
#   undef uint16_t
#   define uint16_t _pipi_uint16_t
#   undef uint32_t
#   define uint32_t _pipi_uint32_t
#   undef uint64_t
#   define uint64_t _pipi_uint64_t

typedef long int _pipi_intptr_t;
typedef unsigned long int _pipi_uintptr_t;
#   undef intptr_t
#   define intptr_t _pipi_intptr_t
#   undef uintptr_t
#   define uintptr_t _pipi_uintptr_t

typedef int _pipi_ssize_t;
typedef unsigned int _pipi_size_t;
#   undef ssize_t
#   define ssize_t _pipi_ssize_t
#   undef size_t
#   define size_t _pipi_size_t

#endif

#endif /* __PIPI_TYPES_H__ */

