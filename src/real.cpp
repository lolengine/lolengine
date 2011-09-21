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

#include <cstring>
#include <cstdio>

#include "core.h"

using namespace std;

namespace lol
{

real::real(float f)
{
    union { float f; uint32_t x; } u = { f };

    uint32_t sign = u.x & 0x80000000u;
    int e = ((u.x >> 23) & 0xff) + (1 << 30) - (1 << 7);

    m_signexp = sign | e;
    m_mantissa[0] = u.x >> 7;
    m_mantissa[1] = u.x << 9;
    memset(m_mantissa + 2, 0, sizeof(m_mantissa) - sizeof(m_mantissa[0]));
}

real::operator float() const
{
    union { float f; uint32_t x; } u;

    u.x = m_mantissa[0] << 7;
    u.x |= m_mantissa[1] >> 9;
    u.x |= ((m_signexp & 0x7fffffffu) - (1 << 30) + (1 << 7)) << 23;
    u.x |= m_signexp & 0x80000000u;

    return u.f;
}

real real::operator *(real const &x) const
{
    real ret;

    ret.m_signexp = (m_signexp ^ x.m_signexp) & 0x80000000u;
    int e = (m_signexp & 0x7fffffffu) - (1 << 30) + 1
          + (x.m_signexp & 0x7fffffffu) - (1 << 30) + 1;

    /* Accumulate low order product; no need to store it, we just
     * want the carry value */
    uint32_t carry = 0;
    for (int i = 0; i < BIGITS; i++)
    {
        for (int j = 0; j < i + 1; j++)
            carry += m_mantissa[BIGITS - 1 - j]
                   * x.m_mantissa[BIGITS - 1 + j - i];
        carry >>= 16;
    }

    for (int i = 0; i < BIGITS; i++)
    {
        for (int j = i + 1; j < BIGITS; j++)
            carry += m_mantissa[BIGITS - 1 - j]
                   * x.m_mantissa[j - 1 - i];

        carry += m_mantissa[BIGITS - 1 - i];
        carry += x.m_mantissa[BIGITS - 1 - i];
        ret.m_mantissa[BIGITS - 1 - i] = carry & 0xffffu;
        carry >>= 16;
    }

    /* Renormalise in case we overflowed the mantissa */
    if (carry)
    {
        carry--;
        for (int i = 0; i < BIGITS; i++)
        {
            uint16_t tmp = ret.m_mantissa[i];
            ret.m_mantissa[i] = (carry << 15) | (tmp >> 1);
            carry = tmp & 0x0001u;
        }
        e++;
    }

    ret.m_signexp |= e + (1 << 30) - 1;

    return ret;
}

void real::print() const
{
    printf("%x  %08x  ", m_signexp >> 31, (m_signexp << 1) >> 1);
    for (int i = 0; i < BIGITS; i++)
        printf("%04x ", m_mantissa[i]);
    printf("\n");
}

} /* namespace lol */

