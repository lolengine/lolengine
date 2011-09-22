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
    uint32_t exponent = (u.x >> 23) & 0xff;

    switch (exponent)
    {
    case 0x00:
    case 0xff:
        m_signexp = sign | exponent;
        break;
    default:
        m_signexp = sign | (exponent + (1 << 30) - (1 << 7));
        break;
    }

    m_mantissa[0] = u.x >> 7;
    m_mantissa[1] = u.x << 9;
    memset(m_mantissa + 2, 0, sizeof(m_mantissa) - sizeof(m_mantissa[0]));
}

real::operator float() const
{
    union { float f; uint32_t x; } u;

    uint32_t sign = m_signexp & 0x80000000u;
    uint32_t exponent = m_signexp & 0x7fffffffu;
    uint32_t mantissa = (m_mantissa[0] << 7) | (m_mantissa[1] >> 9);

    int e = (int)(m_signexp & 0x7fffffffu) - (1 << 30) + (1 << 7);

    if (e < 0)
        u.x = sign;
    else if (e >= 0xff)
        u.x = sign | (0xff << 23);
    else
        u.x = sign | (e << 23) | mantissa;

    return u.f;
}

real real::operator -()
{
    m_signexp ^= 0x80000000u;
    return *this;
}

real real::operator +(real const &x) const
{
    if ((m_signexp << 1) < (x.m_signexp << 1))
        return x + *this;

    if (x.m_signexp << 1 == 0)
        return *this;

    /* For now, assume both numbers are positive. */
    real ret;

    int e1 = (m_signexp & 0x7fffffffu) - (1 << 30) + 1;
    int e2 = (x.m_signexp & 0x7fffffffu) - (1 << 30) + 1;

    int bigoff = (e1 - e2) / (sizeof(uint16_t) * 8);
    int off = e1 - e2 - bigoff * (sizeof(uint16_t) * 8);

    ret.m_signexp = m_signexp;

    uint32_t carry = 0;
    for (int i = 0; i < BIGITS; i++)
    {
        carry = m_mantissa[BIGITS - 1 - i];
        if (BIGITS - 1 - i - bigoff >= 0)
            carry += x.m_mantissa[BIGITS - 1 - i - bigoff] >> off;
        else if (BIGITS - 1 - i - bigoff == -1)
            carry += 0x0001u >> off;

        if (BIGITS - 1 - i - bigoff - 1 >= 0)
            carry += (x.m_mantissa[BIGITS - 1 - i - bigoff - 1] << (16 - off)) & 0xffffu;
        else if (BIGITS - 1 - i - bigoff - 1 == -1)
            carry += 0x0001u << (16 - off);

        ret.m_mantissa[BIGITS - 1 - i] = carry;
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
        ret.m_signexp++;
    }

    return ret;
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

