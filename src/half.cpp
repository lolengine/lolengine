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

#define S4(x)    S1(4*(x)),  S1(4*(x)+1),  S1(4*(x)+2),  S1(4*(x)+3)
#define S16(x)   S4(4*(x)),  S4(4*(x)+1),  S4(4*(x)+2),  S4(4*(x)+3)
#define S64(x)  S16(4*(x)), S16(4*(x)+1), S16(4*(x)+2), S16(4*(x)+3)
#define S256(x) S64(4*(x)), S64(4*(x)+1), S64(4*(x)+2), S64(4*(x)+3)

namespace lol
{

half half::makefast(float f)
{
    /* Lookup table-based algorithm from “Fast Half Float Conversions”
     * by Jeroen van der Zijp, November 2008. No rounding is performed. */
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

half half::makeslow(float f)
{
    union { float f; uint32_t x; } u = { f };

    uint16_t bits = (u.x >> 16) & 0x8000; /* Get the sign */
    uint16_t m = (u.x >> 12) & 0x07ff; /* Keep one extra bit for rounding */
    unsigned int e = (u.x >> 23) & 0xff; /* Using int is faster here */

    /* If zero, or denormal, or exponent underflows too much for a denormal,
     * return signed zero */
    if (e < 103)
        return makebits(bits);

    /* If NaN, Inf or exponent overflow, return NaN or Inf */
    if (e > 142)
    {
        bits |= 0x7c00u;
        bits |= e == 255 && (u.x & 0x007fffffu);
        return makebits(bits);
    }

    /* If exponent underflows but not too much, return a denormal */
    if (e < 113)
    {
        m |= 0x0800u;
        bits |= (m >> (114 - e)) + ((m >> (113 - e)) & 1);
        return makebits(bits);
    }

    bits |= ((e - 112) << 10) | (m >> 1);
    bits += m & 1; /* Overflows here are expected and handled */
    return makebits(bits);
}

} /* namespace lol */

