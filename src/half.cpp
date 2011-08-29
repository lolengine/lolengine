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

#include "core.h"

using namespace std;

namespace lol
{

/* These macros implement a finite iterator useful to build lookup
 * tables. For instance, S64(0) will call S1(x) for all values of x
 * between 0 and 63.
 * Due to the exponential behaviour of the calls, the stress on the
 * compiler may be important. */
#define S4(x)    S1((x)),   S1((x)+1),     S1((x)+2),     S1((x)+3)
#define S16(x)   S4((x)),   S4((x)+4),     S4((x)+8),     S4((x)+12)
#define S64(x)   S16((x)),  S16((x)+16),   S16((x)+32),   S16((x)+48)
#define S256(x)  S64((x)),  S64((x)+64),   S64((x)+128),  S64((x)+192)
#define S1024(x) S256((x)), S256((x)+256), S256((x)+512), S256((x)+768)

/* Lookup table-based algorithm from “Fast Half Float Conversions”
 * by Jeroen van der Zijp, November 2008. No rounding is performed,
 * and some NaN values may be incorrectly converted to Inf. */
static inline uint16_t float_to_half_nobranch(uint32_t x)
{
    static uint16_t const basetable[512] =
    {
#define S1(i) (((i) < 103) ? 0x0000: \
               ((i) < 113) ? 0x0400 >> (113 - (i)) : \
               ((i) < 143) ? ((i) - 112) << 10 : 0x7c00)
        S256(0),
#undef S1
#define S1(i) (0x8000 | (((i) < 103) ? 0x0000 : \
                         ((i) < 113) ? 0x0400 >> (113 - (i)): \
                         ((i) < 143) ? ((i) - 112) << 10 : 0x7c00))
        S256(0),
#undef S1
    };

    static uint8_t const shifttable[512] =
    {
#define S1(i) (((i) < 103) ? 24 : \
               ((i) < 113) ? 126 - (i) : \
               ((i) < 143 || (i) == 255) ? 13 : 24)
        S256(0), S256(0),
#undef S1
    };

    uint16_t bits = basetable[(x >> 23) & 0x1ff];
    bits |= (x & 0x007fffff) >> shifttable[(x >> 23) & 0x1ff];
    return bits;
}

/* This method is faster than the OpenEXR implementation (very often
 * used, eg. in Ogre), with the additional benefit of rounding, inspired
 * by James Tursa’s half-precision code. */
static inline uint16_t float_to_half_branch(uint32_t x)
{
    uint16_t bits = (x >> 16) & 0x8000; /* Get the sign */
    uint16_t m = (x >> 12) & 0x07ff; /* Keep one extra bit for rounding */
    unsigned int e = (x >> 23) & 0xff; /* Using int is faster here */

    /* If zero, or denormal, or exponent underflows too much for a denormal,
     * return signed zero. */
    if (e < 103)
        return bits;

    /* If NaN, return NaN. If Inf or exponent overflow, return Inf. */
    if (e > 142)
    {
        bits |= 0x7c00u;
        /* If exponent was 0xff and one mantissa bit was set, it means NaN,
         * not Inf, so make sure we set one mantissa bit too. */
        bits |= e == 255 && (x & 0x007fffffu);
        return bits;
    }

    /* If exponent underflows but not too much, return a denormal */
    if (e < 113)
    {
        m |= 0x0800u;
        /* Extra rounding may overflow and set mantissa to 0 and exponent
         * to 1, which is OK. */
        bits |= (m >> (114 - e)) + ((m >> (113 - e)) & 1);
        return bits;
    }

    bits |= ((e - 112) << 10) | (m >> 1);
    /* Extra rounding. An overflow will set mantissa to 0 and increment
     * the exponent, which is OK. */
    bits += m & 1;
    return bits;
}

static int const shifttable[32] =
{
    23, 14, 22, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 20, 0,
    15, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 17, 0, 18, 19, 0,
};
static uint32_t const shiftmagic = 0x07c4acddu;

/* Lookup table-based algorithm from “Fast Half Float Conversions”
 * by Jeroen van der Zijp, November 2008. Tables are generated using
 * the C++ preprocessor, thanks to a branchless implementation also
 * used in half_to_float_branch(). This code is very fast when performing
 * conversions on arrays of values. */
static inline uint32_t half_to_float_nobranch(uint16_t x)
{
#define M3(i) ((i) | ((i) >> 1))
#define M7(i) (M3(i) | (M3(i) >> 2))
#define MF(i) (M7(i) | (M7(i) >> 4))
#define MFF(i) (MF(i) | (MF(i) >> 8))
#define E(i) shifttable[(unsigned int)(MFF(i) * shiftmagic) >> 27]

    static uint32_t const mantissatable[2048] =
    {
#define S1(i) (((i) == 0) ? 0 : ((125 - E(i)) << 23) + ((i) << E(i)))
        S1024(0),
#undef S1
#define S1(i) (0x38000000u + ((i) << 13))
        S1024(0),
#undef S1
    };

    static uint32_t const exponenttable[64] =
    {
#define S1(i) (((i) == 0) ? 0 : \
               ((i) < 31) ? ((i) << 23) : \
               ((i) == 31) ? 0x47800000u : \
               ((i) == 32) ? 0x80000000u : \
               ((i) < 63) ? (0x80000000u + (((i) - 32) << 23)) : 0xc7800000)
        S64(0),
#undef S1
    };

    static int const offsettable[64] =
    {
#define S1(i) (((i) == 0 || (i) == 32) ? 0 : 1024)
        S64(0),
#undef S1
    };

    return mantissatable[offsettable[x >> 10] + (x & 0x3ff)]
            + exponenttable[x >> 10];
}

/* This algorithm is similar to the OpenEXR implementation, except it
 * uses branchless code in the denormal path. This is slower than the
 * table version, but will be more friendly to the cache for occasional
 * uses. */
static inline uint32_t half_to_float_branch(uint16_t x)
{
    uint32_t s = (x & 0x8000u) << 16;

    if ((x & 0x7fffu) == 0)
        return (uint32_t)x << 16;

    uint32_t e = x & 0x7c00u;
    uint32_t m = x & 0x03ffu;

    if (e == 0)
    {
        uint32_t v = m | (m >> 1);
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;

        e = shifttable[(v * shiftmagic) >> 27];

        /* We don't have to remove the 10th mantissa bit because it gets
         * added to our underestimated exponent. */
        return s | (((125 - e) << 23) + (m << e));
    }

    if (e == 0x7c00u)
    {
        /* The amd64 pipeline likes the if() better than a ternary operator
         * or any other trick I could find. --sam */
        if (m == 0)
            return s | 0x7f800000u;
        return s | 0x7fc00000u;
    }

    return s | (((e >> 10) + 112) << 23) | (m << 13);
}

/* Constructor from float. Uses the non-branching version because benchmarks
 * indicate it is always twice as fast. The penalty of loading the lookup
 * tables does not seem important. */
half half::makefast(float f)
{
    union { float f; uint32_t x; } u = { f };
    return makebits(float_to_half_nobranch(u.x));
}

/* Constructor from float with better precision. */
half half::makeslow(float f)
{
    union { float f; uint32_t x; } u = { f };
    return makebits(float_to_half_branch(u.x));
}

/* Cast to float. Uses the branching version because loading the tables
 * for only one value is going to be cache-expensive. */
half::operator float() const
{
    /* FIXME: there is a hidden "this" in this method. Export more
     * code so that it can all work in registers instead. */
    union { float f; uint32_t x; } u;
    u.x = half_to_float_branch(bits);
    return u.f;
}

size_t half::convert(half *dst, float const *src, size_t nelem)
{
    for (size_t i = 0; i < nelem; i++)
    {
        union { float f; uint32_t x; } u;
        u.f = *src++;
        *dst++ = makebits(float_to_half_nobranch(u.x));
    }

    return nelem;
}

size_t half::convert(float *dst, half const *src, size_t nelem)
{
    for (size_t i = 0; i < nelem; i++)
    {
        union { float f; uint32_t x; } u;
        u.x = half_to_float_nobranch((*src++).bits);
        *dst++ = u.f;
    }

    return nelem;
}

} /* namespace lol */

