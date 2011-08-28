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

/* Lookup table-based algorithm from “Fast Half Float Conversions”
 * by Jeroen van der Zijp, November 2008. No rounding is performed,
 * and some NaN values may be incorrectly converted to Inf. */
half half::makefast(float f)
{
#define S4(x)    S1(4*(x)),  S1(4*(x)+1),  S1(4*(x)+2),  S1(4*(x)+3)
#define S16(x)   S4(4*(x)),  S4(4*(x)+1),  S4(4*(x)+2),  S4(4*(x)+3)
#define S64(x)  S16(4*(x)), S16(4*(x)+1), S16(4*(x)+2), S16(4*(x)+3)
#define S256(x) S64(4*(x)), S64(4*(x)+1), S64(4*(x)+2), S64(4*(x)+3)

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

    union { float f; uint32_t x; } u = { f };

    uint16_t bits = basetable[(u.x >> 23) & 0x1ff];
    bits |= (u.x & 0x007fffff) >> shifttable[(u.x >> 23) & 0x1ff];
    return makebits(bits);
}

/* This method is faster than the OpenEXR implementation (very often
 * used, eg. in Ogre), with the additional benefit of rounding, inspired
 * by James Tursa’s half-precision code. */
half half::makeslow(float f)
{
    union { float f; uint32_t x; } u = { f };

    uint16_t bits = (u.x >> 16) & 0x8000; /* Get the sign */
    uint16_t m = (u.x >> 12) & 0x07ff; /* Keep one extra bit for rounding */
    unsigned int e = (u.x >> 23) & 0xff; /* Using int is faster here */

    /* If zero, or denormal, or exponent underflows too much for a denormal,
     * return signed zero. */
    if (e < 103)
        return makebits(bits);

    /* If NaN, return NaN. If Inf or exponent overflow, return Inf. */
    if (e > 142)
    {
        bits |= 0x7c00u;
        /* If exponent was 0xff and one mantissa bit was set, it means NaN,
         * not Inf, so make sure we set one mantissa bit too. */
        bits |= e == 255 && (u.x & 0x007fffffu);
        return makebits(bits);
    }

    /* If exponent underflows but not too much, return a denormal */
    if (e < 113)
    {
        m |= 0x0800u;
        /* Extra rounding may overflow and set mantissa to 0 and exponent
         * to 1, which is OK. */
        bits |= (m >> (114 - e)) + ((m >> (113 - e)) & 1);
        return makebits(bits);
    }

    bits |= ((e - 112) << 10) | (m >> 1);
    /* Extra rounding. An overflow will set mantissa to 0 and increment
     * the exponent, which is OK. */
    bits += m & 1;
    return makebits(bits);
}

half::operator float() const
{
    union { float f; uint32_t x; } u;

    uint32_t s = (m_bits & 0x8000u) << 16;

    if ((m_bits & 0x7fffu) == 0)
    {
        u.x = (uint32_t)m_bits << 16;
        return u.f;
    }

    uint32_t e = m_bits & 0x7c00u;
    uint32_t m = m_bits & 0x03ffu;

    if (e == 0)
    {
        static int const shifttable[32] =
        {
            10, 1, 9, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 7, 0,
            2, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 5, 6, 0,
        };

        uint32_t v = m | (m >> 1);
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;

        e = shifttable[(v * 0x07C4ACDDU) >> 27];
        m <<= e;

        /* We don't have to remove the 10th mantissa bit because it gets
         * added to our underestimated exponent. */
        u.x = s | (((112 - e) << 23) + (m << 13));
        return u.f;
    }

    if (e == 0x7c00u)
    {
        /* The amd64 pipeline likes the if() better than a ternary operator
         * or any other trick I could find. --sam */
        if (m == 0)
            u.x = s | 0x7f800000u;
        else
            u.x = s | 0x7fc00000u;

        return u.f;
    }

    u.x = s | (((e >> 10) + 112) << 23) | (m << 13);

    return u.f;
}

} /* namespace lol */

