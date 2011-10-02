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

real::real(float f) { *this = (double)f; }
real::real(int i) { *this = (double)i; }
real::real(unsigned int i) { *this = (double)i; }

real::real(double d)
{
    union { double d; uint64_t x; } u = { d };

    uint32_t sign = (u.x >> 63) << 31;
    uint32_t exponent = (u.x << 1) >> 53;

    switch (exponent)
    {
    case 0x00:
        m_signexp = sign;
        break;
    case 0x7ff:
        m_signexp = sign | 0x7fffffffu;
        break;
    default:
        m_signexp = sign | (exponent + (1 << 30) - (1 << 10));
        break;
    }

    m_mantissa[0] = u.x >> 36;
    m_mantissa[1] = u.x >> 20;
    m_mantissa[2] = u.x >> 4;
    m_mantissa[3] = u.x << 12;
    memset(m_mantissa + 4, 0, sizeof(m_mantissa) - 4 * sizeof(m_mantissa[0]));
}

real::operator float() const { return (float)(double)(*this); }
real::operator int() const { return (int)(double)(*this); }
real::operator unsigned int() const { return (unsigned int)(double)(*this); }

real::operator double() const
{
    union { double d; uint64_t x; } u;

    /* Get sign */
    u.x = m_signexp >> 31;
    u.x <<= 11;

    /* Compute new exponent */
    uint32_t exponent = (m_signexp << 1) >> 1;
    int e = (int)exponent - (1 << 30) + (1 << 10);

    if (e < 0)
        u.x <<= 52;
    else if (e >= 0x7ff)
    {
        u.x |= 0x7ff;
        u.x <<= 52;
    }
    else
    {
        u.x |= e;

        /* Store mantissa if necessary */
        u.x <<= 16;
        u.x |= m_mantissa[0];
        u.x <<= 16;
        u.x |= m_mantissa[1];
        u.x <<= 16;
        u.x |= m_mantissa[2];
        u.x <<= 4;
        u.x |= m_mantissa[3] >> 12;
        /* Rounding */
        u.x += (m_mantissa[3] >> 11) & 1;
    }

    return u.d;
}

real real::operator +() const
{
    return *this;
}

real real::operator -() const
{
    real ret = *this;
    ret.m_signexp ^= 0x80000000u;
    return ret;
}

real real::operator +(real const &x) const
{
    if (x.m_signexp << 1 == 0)
        return *this;

    /* Ensure both arguments are positive. Otherwise, switch signs,
     * or replace + with -. */
    if (m_signexp >> 31)
        return -(-*this + -x);

    if (x.m_signexp >> 31)
        return *this - (-x);

    /* Ensure *this has the larger exponent (no need for the mantissa to
     * be larger, as in subtraction). Otherwise, switch. */
    if ((m_signexp << 1) < (x.m_signexp << 1))
        return x + *this;

    real ret;

    int e1 = m_signexp - (1 << 30) + 1;
    int e2 = x.m_signexp - (1 << 30) + 1;

    int bigoff = (e1 - e2) / (sizeof(uint16_t) * 8);
    int off = e1 - e2 - bigoff * (sizeof(uint16_t) * 8);

    if (bigoff > BIGITS)
        return *this;

    ret.m_signexp = m_signexp;

    uint32_t carry = 0;
    for (int i = BIGITS; i--; )
    {
        carry += m_mantissa[i];
        if (i - bigoff >= 0)
            carry += x.m_mantissa[i - bigoff] >> off;

        if (i - bigoff > 0)
            carry += (x.m_mantissa[i - bigoff - 1] << (16 - off)) & 0xffffu;
        else if (i - bigoff == 0)
            carry += 0x0001u << (16 - off);

        ret.m_mantissa[i] = carry;
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

real real::operator -(real const &x) const
{
    if (x.m_signexp << 1 == 0)
        return *this;

    /* Ensure both arguments are positive. Otherwise, switch signs,
     * or replace - with +. */
    if (m_signexp >> 31)
        return -(-*this + x);

    if (x.m_signexp >> 31)
        return (*this) + (-x);

    /* Ensure *this is larger than x */
    if (*this < x)
        return -(x - *this);

    real ret;

    int e1 = m_signexp - (1 << 30) + 1;
    int e2 = x.m_signexp - (1 << 30) + 1;

    int bigoff = (e1 - e2) / (sizeof(uint16_t) * 8);
    int off = e1 - e2 - bigoff * (sizeof(uint16_t) * 8);

    if (bigoff > BIGITS)
        return *this;

    ret.m_signexp = m_signexp;

    int32_t carry = 0;
    for (int i = 0; i < bigoff; i++)
    {
        carry -= x.m_mantissa[BIGITS - i];
        carry = (carry & 0xffff0000u) | (carry >> 16);
    }
    carry -= x.m_mantissa[BIGITS - 1 - bigoff] & ((1 << off) - 1);
    carry /= (1 << off);

    for (int i = BIGITS; i--; )
    {
        carry += m_mantissa[i];
        if (i - bigoff >= 0)
            carry -= x.m_mantissa[i - bigoff] >> off;

        if (i - bigoff > 0)
            carry -= (x.m_mantissa[i - bigoff - 1] << (16 - off)) & 0xffffu;
        else if (i - bigoff == 0)
            carry -= 0x0001u << (16 - off);

        ret.m_mantissa[i] = carry;
        carry = (carry & 0xffff0000u) | (carry >> 16);
    }

    carry += 1;

    /* Renormalise if we underflowed the mantissa */
    if (carry == 0)
    {
        /* How much do we need to shift the mantissa? FIXME: this could
         * be computed above */
        off = 0;
        for (int i = 0; i < BIGITS; i++)
        {
            if (!ret.m_mantissa[i])
            {
                off += sizeof(uint16_t) * 8;
                continue;
            }

            for (uint16_t tmp = ret.m_mantissa[i]; tmp < 0x8000u; tmp <<= 1)
                off++;
            break;
        }
        if (off == BIGITS * sizeof(uint16_t) * 8)
            ret.m_signexp &= 0x80000000u;
        else
        {
            off++; /* Shift one more to get rid of the leading one */
            ret.m_signexp -= off;

            bigoff = off / (sizeof(uint16_t) * 8);
            off -= bigoff * sizeof(uint16_t) * 8;

            for (int i = 0; i < BIGITS; i++)
            {
                uint16_t tmp = 0;
                if (i + bigoff < BIGITS)
                    tmp |= ret.m_mantissa[i + bigoff] << off;
                if (i + bigoff + 1 < BIGITS)
                    tmp |= ret.m_mantissa[i + bigoff + 1] >> (16 - off);
                ret.m_mantissa[i] = tmp;
            }
        }
    }

    return ret;
}

real real::operator *(real const &x) const
{
    real ret;

    if (m_signexp << 1 == 0 || x.m_signexp << 1 == 0)
    {
        ret = (m_signexp << 1 == 0) ? *this : x;
        ret.m_signexp ^= x.m_signexp & 0x80000000u;
        return ret;
    }

    ret.m_signexp = (m_signexp ^ x.m_signexp) & 0x80000000u;
    int e = (m_signexp & 0x7fffffffu) - (1 << 30) + 1
          + (x.m_signexp & 0x7fffffffu) - (1 << 30) + 1;

    /* Accumulate low order product; no need to store it, we just
     * want the carry value */
    uint64_t carry = 0;
    for (int i = 0; i < BIGITS; i++)
    {
        for (int j = 0; j < i + 1; j++)
            carry += (uint32_t)m_mantissa[BIGITS - 1 - j]
                   * (uint32_t)x.m_mantissa[BIGITS - 1 + j - i];
        carry >>= 16;
    }

    for (int i = 0; i < BIGITS; i++)
    {
        for (int j = i + 1; j < BIGITS; j++)
            carry += (uint32_t)m_mantissa[BIGITS - 1 - j]
                   * (uint32_t)x.m_mantissa[j - 1 - i];

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

real real::operator /(real const &x) const
{
    return *this * re(x);
}

real &real::operator +=(real const &x)
{
    real tmp = *this;
    return *this = tmp + x;
}

real &real::operator -=(real const &x)
{
    real tmp = *this;
    return *this = tmp - x;
}

real &real::operator *=(real const &x)
{
    real tmp = *this;
    return *this = tmp * x;
}

real &real::operator /=(real const &x)
{
    real tmp = *this;
    return *this = tmp / x;
}

real real::operator <<(int x) const
{
    real tmp = *this;
    return tmp <<= x;
}

real real::operator >>(int x) const
{
    real tmp = *this;
    return tmp >>= x;
}

real &real::operator <<=(int x)
{
    if (m_signexp << 1)
        m_signexp += x;
    return *this;
}

real &real::operator >>=(int x)
{
    if (m_signexp << 1)
        m_signexp -= x;
    return *this;
}

bool real::operator ==(real const &x) const
{
    if ((m_signexp << 1) == 0 && (x.m_signexp << 1) == 0)
        return true;

    if (m_signexp != x.m_signexp)
        return false;

    return memcmp(m_mantissa, x.m_mantissa, sizeof(m_mantissa)) == 0;
}

bool real::operator !=(real const &x) const
{
    return !(*this == x);
}

bool real::operator <(real const &x) const
{
    /* Ensure both numbers are positive */
    if (m_signexp >> 31)
        return (x.m_signexp >> 31) ? -*this > -x : true;

    if (x.m_signexp >> 31)
        return false;

    /* Compare all relevant bits */
    if (m_signexp != x.m_signexp)
        return m_signexp < x.m_signexp;

    for (int i = 0; i < BIGITS; i++)
        if (m_mantissa[i] != x.m_mantissa[i])
            return m_mantissa[i] < x.m_mantissa[i];

    return false;
}

bool real::operator <=(real const &x) const
{
    return !(*this > x);
}

bool real::operator >(real const &x) const
{
    /* Ensure both numbers are positive */
    if (m_signexp >> 31)
        return (x.m_signexp >> 31) ? -*this < -x : false;

    if (x.m_signexp >> 31)
        return true;

    /* Compare all relevant bits */
    if (m_signexp != x.m_signexp)
        return m_signexp > x.m_signexp;

    for (int i = 0; i < BIGITS; i++)
        if (m_mantissa[i] != x.m_mantissa[i])
            return m_mantissa[i] > x.m_mantissa[i];

    return false;
}

bool real::operator >=(real const &x) const
{
    return !(*this < x);
}

bool real::operator !() const
{
    return !(bool)*this;
}

real::operator bool() const
{
    /* A real is "true" if it is non-zero (exponent is non-zero) AND
     * not NaN (exponent is not full bits OR higher order mantissa is zero) */
    uint32_t exponent = m_signexp << 1;
    return exponent && (~exponent || m_mantissa[0] == 0);
}

real re(real const &x)
{
    if (!(x.m_signexp << 1))
    {
        real ret = x;
        ret.m_signexp = x.m_signexp | 0x7fffffffu;
        ret.m_mantissa[0] = 0;
        return ret;
    }

    /* Use the system's float inversion to approximate 1/x */
    union { float f; uint32_t x; } u = { 1.0f }, v = { 1.0f };
    v.x |= (uint32_t)x.m_mantissa[0] << 7;
    v.x |= (uint32_t)x.m_mantissa[1] >> 9;
    v.f = 1.0 / v.f;

    real ret;
    ret.m_mantissa[0] = (v.x >> 7) & 0xffffu;
    ret.m_mantissa[1] = (v.x << 9) & 0xffffu;

    uint32_t sign = x.m_signexp & 0x80000000u;
    ret.m_signexp = sign;

    int exponent = (x.m_signexp & 0x7fffffffu) + 1;
    exponent = -exponent + (v.x >> 23) - (u.x >> 23);
    ret.m_signexp |= (exponent - 1) & 0x7fffffffu;

    /* Five steps of Newton-Raphson seems enough for 32-bigit reals. */
    real two = 2;
    ret = ret * (two - ret * x);
    ret = ret * (two - ret * x);
    ret = ret * (two - ret * x);
    ret = ret * (two - ret * x);
    ret = ret * (two - ret * x);

    return ret;
}

real sqrt(real const &x)
{
    /* if zero, return x */
    if (!(x.m_signexp << 1))
        return x;

    /* if negative, return NaN */
    if (x.m_signexp >> 31)
    {
        real ret;
        ret.m_signexp = 0x7fffffffu;
        ret.m_mantissa[0] = 0xffffu;
        return ret;
    }

    /* Use the system's float inversion to approximate 1/sqrt(x). First
     * we construct a float in the [1..4[ range that has roughly the same
     * mantissa as our real. Its exponent is 0 or 1, depending on the
     * partity of x. The final exponent is 0, -1 or -2. We use the final
     * exponent and final mantissa to pre-fill the result. */
    union { float f; uint32_t x; } u = { 1.0f }, v = { 2.0f };
    v.x -= ((x.m_signexp & 1) << 23);
    v.x |= (uint32_t)x.m_mantissa[0] << 7;
    v.x |= (uint32_t)x.m_mantissa[1] >> 9;
    v.f = 1.0 / sqrtf(v.f);

    real ret;
    ret.m_mantissa[0] = (v.x >> 7) & 0xffffu;
    ret.m_mantissa[1] = (v.x << 9) & 0xffffu;

    uint32_t sign = x.m_signexp & 0x80000000u;
    ret.m_signexp = sign;

    uint32_t exponent = (x.m_signexp & 0x7fffffffu);
    exponent = ((1 << 30) + (1 << 29) -1) - (exponent + 1) / 2;
    exponent = exponent + (v.x >> 23) - (u.x >> 23);
    ret.m_signexp |= exponent & 0x7fffffffu;

    /* Five steps of Newton-Raphson seems enough for 32-bigit reals. */
    real three = 3;
    ret = ret * (three - ret * ret * x);
    ret.m_signexp--;
    ret = ret * (three - ret * ret * x);
    ret.m_signexp--;
    ret = ret * (three - ret * ret * x);
    ret.m_signexp--;
    ret = ret * (three - ret * ret * x);
    ret.m_signexp--;
    ret = ret * (three - ret * ret * x);
    ret.m_signexp--;

    return ret * x;
}

real fabs(real const &x)
{
    real ret = x;
    ret.m_signexp &= 0x7fffffffu;
    return ret;
}

static real fastlog(real const &x)
{
    /* This fast log method is tuned to work on the [1..2] range and
     * no effort whatsoever was made to improve convergence outside this
     * domain of validity. It can converge pretty fast, provided we use
     * the following variable substitutions:
     *    y = sqrt(x)
     *    z = (y - 1) / (y + 1)
     *
     * And the following identities:
     *    ln(x) = 2 ln(y)
     *          = 2 ln((1 + z) / (1 - z))
     *          = 4 z (1 + z^2 / 3 + z^4 / 5 + z^6 / 7...)
     *
     * Any additional sqrt() call would halve the convergence time, but
     * would also impact the final precision. For now we stick with one
     * sqrt() call. */
    real y = sqrt(x);
    real z = (y - (real)1) / (y + (real)1), z2 = z * z, zn = z2;
    real sum = 1.0;

    for (int i = 3; i < 200; i += 2)
    {
        sum += zn / (real)i;
        zn *= z2;
    }

    return z * (sum << 2);
}

static real LOG_2 = fastlog((real)2);

real log(real const &x)
{
    /* Strategy for log(x): if x = 2^E*M then log(x) = E log(2) + log(M),
     * with the property that M is in [1..2[, so fastlog() applies here. */
    real tmp = x;
    if (x.m_signexp >> 31 || x.m_signexp == 0)
    {
        tmp.m_signexp = 0xffffffffu;
        tmp.m_mantissa[0] = 0xffffu;
        return tmp;
    }
    tmp.m_signexp = (1 << 30) - 1;
    return (real)(x.m_signexp - (1 << 30) + 1) * LOG_2 + fastlog(tmp);
}

real exp(real const &x)
{
    /* Strategy for exp(x): the Taylor series does not converge very fast
     * with large positive or negative values.
     *
     * However, we know that the result is going to be in the form M*2^E,
     * where M is the mantissa and E the exponent. We first try to predict
     * a value for E, which is approximately log2(exp(x)) = x / log(2).
     *
     * Let E0 be an integer close to x / log(2). We need to find a value x0
     * such that exp(x) = 2^E0 * exp(x0). We get x0 = x - E0 log(2).
     *
     * Thus the final algorithm:
     *  int E0 = x / log(2)
     *  real x0 = x - E0 log(2)
     *  real x1 = exp(x0)
     *  return x1 * 2^E0
     */
    int e0 = x / LOG_2;
    real x0 = x - (real)e0 * LOG_2;
    real x1 = 1.0, fact = 1.0, xn = x0;

    for (int i = 1; i < 100; i++)
    {
        fact *= (real)i;
        x1 += xn / fact;
        xn *= x0;
    }

    x1.m_signexp += e0;
    return x1;
}

real floor(real const &x)
{
    /* Strategy for floor(x):
     *  - if negative, return -ceil(-x)
     *  - if zero or negative zero, return x
     *  - if less than one, return zero
     *  - otherwise, if e is the exponent, clear all bits except the
     *    first e. */
    if (x < -real::R_0)
        return -ceil(-x);
    if (!x)
        return x;
    if (x < real::R_1)
        return real::R_0;

    real ret = x;
    int exponent = x.m_signexp - (1 << 30) + 1;

    for (int i = 0; i < real::BIGITS; i++)
    {
        if (exponent <= 0)
            ret.m_mantissa[i] = 0;
        else if (exponent < 8 * (int)sizeof(uint16_t))
            ret.m_mantissa[i] &= ~((1 << (16 - exponent)) - 1);

        exponent -= 8 * sizeof(uint16_t);
    }

    return ret;
}

real ceil(real const &x)
{
    /* Strategy for ceil(x):
     *  - if negative, return -floor(-x)
     *  - if x == floor(x), return x
     *  - otherwise, return floor(x) + 1 */
    if (x < -real::R_0)
        return -floor(-x);
    real ret = floor(x);
    if (x == ret)
        return ret;
    else
        return ret + real::R_1;
}

real sin(real const &x)
{
    real ret = 0.0, fact = 1.0, xn = x, x2 = x * x;

    for (int i = 1; ; i += 2)
    {
        real newret = ret + xn / fact;
        if (ret == newret)
            break;
        ret = newret;
        xn *= x2;
        fact *= (real)(-(i + 1) * (i + 2));
    }

    return ret;
}

real cos(real const &x)
{
    real ret = 0.0, fact = 1.0, xn = 1.0, x2 = x * x;

    for (int i = 1; ; i += 2)
    {
        real newret = ret + xn / fact;
        if (ret == newret)
            break;
        ret = newret;
        xn *= x2;
        fact *= (real)(-i * (i + 1));
    }

    return ret;
}

static real asinacos(real const &x, bool is_asin, bool is_negative)
{
    /* Strategy for asin(): in [-0.5..0.5], use a Taylor series around
     * zero. In [0.5..1], use asin(x) = π/2 - 2*asin(sqrt((1-x)/2)), and
     * in [-1..-0.5] just revert the sign.
     * Strategy for acos(): use acos(x) = π/2 - asin(x) and try not to
     * lose the precision around x=1. */
    real absx = fabs(x);
    bool around_zero = (absx < (real::R_1 >> 1));

    if (!around_zero)
        absx = sqrt((real::R_1 - absx) >> 1);

    real ret = absx, xn = absx, x2 = absx * absx, fact1 = 2, fact2 = 1;
    for (int i = 1; i < 280; i++)
    {
        xn *= x2;
        ret += (fact1 * xn / ((real)(2 * i + 1) * fact2)) >> (i * 2);
        fact1 *= (real)((2 * i + 1) * (2 * i + 2));
        fact2 *= (real)((i + 1) * (i + 1));
    }

    if (is_negative)
        ret = -ret;

    if (around_zero)
        ret = is_asin ? ret : real::R_PI_2 - ret;
    else
    {
        real adjust = is_negative ? real::R_PI : real::R_0;
        if (is_asin)
            ret = real::R_PI_2 - adjust - (ret << 1);
        else
            ret = adjust + (ret << 1);
    }

    return ret;
}

real asin(real const &x)
{
    return asinacos(x, true, x.m_signexp >> 31);
}

real acos(real const &x)
{
    return asinacos(x, false, x.m_signexp >> 31);
}

real atan(real const &x)
{
    /* Computing atan(x): we choose a different Taylor series depending on
     * the value of x to help with convergence.
     *
     * If |x| < 0.5 we evaluate atan(y) near 0:
     *  atan(y) = y - y^3/3 + y^5/5 - y^7/7 + y^9/9 ...
     *
     * If 0.5 <= |x| < 1.5 we evaluate atan(1+y) near 0:
     *  atan(1+y) = π/4 + y/(1*2^1) - y^2/(2*2^1) + y^3/(3*2^2)
     *                  - y^5/(5*2^3) + y^6/(6*2^3) - y^7/(7*2^4)
     *                  + y^9/(9*2^5) - y^10/(10*2^5) + y^11/(11*2^6) ...
     *
     * If 1.5 <= |x| < 2 we evaluate atan(sqrt(3)+2y) near 0:
     *  atan(sqrt(3)+2y) = π/3 + 1/2 y - sqrt(3)/2 y^2/2
     *                         + y^3/3 - sqrt(3)/2 y^4/4 + 1/2 y^5/5
     *                         - 1/2 y^7/7 + sqrt(3)/2 y^8/8
     *                         - y^9/9 + sqrt(3)/2 y^10/10 - 1/2 y^11/11
     *                         + 1/2 y^13/13 - sqrt(3)/2 y^14/14
     *                         + y^15/15 - sqrt(3)/2 y^16/16 + 1/2 y^17/17 ...
     *
     * If |x| >= 2 we evaluate atan(y) near +∞:
     *  atan(y) = π/2 - y^-1 + y^-3/3 - y^-5/5 + y^-7/7 - y^-9/9 ...
     */
    real absx = fabs(x);

    if (absx < (real::R_1 >> 1))
    {
        real ret = x, xn = x, mx2 = -x * x;
        for (int i = 3; i < 100; i += 2)
        {
            xn *= mx2;
            ret += xn / (real)i;
        }
        return ret;
    }

    real ret = 0;

    if (absx < (real::R_3 >> 1))
    {
        real y = real::R_1 - absx;
        real yn = y, my2 = -y * y;
        for (int i = 0; i < 200; i += 2)
        {
            ret += (yn / (real)(2 * i + 1)) >> (i + 1);
            yn *= y;
            ret += (yn / (real)(2 * i + 2)) >> (i + 1);
            yn *= y;
            ret += (yn / (real)(2 * i + 3)) >> (i + 2);
            yn *= my2;
        }
        ret = real::R_PI_4 - ret;
    }
    else if (absx < real::R_2)
    {
        real y = (absx - real::R_SQRT3) >> 1;
        real yn = y, my2 = -y * y;
        for (int i = 1; i < 200; i += 6)
        {
            ret += (yn / (real)i) >> 1;
            yn *= y;
            ret -= (real::R_SQRT3 >> 1) * yn / (real)(i + 1);
            yn *= y;
            ret += yn / (real)(i + 2);
            yn *= y;
            ret -= (real::R_SQRT3 >> 1) * yn / (real)(i + 3);
            yn *= y;
            ret += (yn / (real)(i + 4)) >> 1;
            yn *= my2;
        }
        ret = real::R_PI_3 + ret;
    }
    else
    {
        real y = re(absx);
        real yn = y, my2 = -y * y;
        ret = y;
        for (int i = 3; i < 120; i += 2)
        {
            yn *= my2;
            ret += yn / (real)i;
        }
        ret = real::R_PI_2 - ret;
    }

    /* Propagate sign */
    ret.m_signexp |= (x.m_signexp & 0x80000000u);
    return ret;
}

void real::print(int ndigits) const
{
    real const r1 = 1, r10 = 10;
    real x = *this;

    if (x.m_signexp >> 31)
    {
        printf("-");
        x = -x;
    }

    /* Normalise x so that mantissa is in [1..9.999] */
    int exponent = 0;
    if (x.m_signexp)
    {
        for (real div = r1, newdiv; true; div = newdiv)
        {
            newdiv = div * r10;
            if (x < newdiv)
            {
                x /= div;
                break;
            }
            exponent++;
        }
        for (real mul = 1, newx; true; mul *= r10)
        {
            newx = x * mul;
            if (newx >= r1)
            {
                x = newx;
                break;
            }
            exponent--;
        }
    }

    /* Print digits */
    for (int i = 0; i < ndigits; i++)
    {
        int digit = (int)x;
        printf("%i", digit);
        if (i == 0)
            printf(".");
        x -= real(digit);
        x *= r10;
    }

    /* Print exponent information */
    if (exponent < 0)
        printf("e-%i", -exponent);
    else if (exponent > 0)
        printf("e+%i", exponent);

    printf("\n");
}

static real fast_pi()
{
    /* Approximate Pi using Machin's formula: 16*atan(1/5)-4*atan(1/239) */
    real ret = 0.0, x0 = 5.0, x1 = 239.0;
    real const m0 = -x0 * x0, m1 = -x1 * x1, r16 = 16.0, r4 = 4.0;

    /* Degree 240 is required for 512-bit mantissa precision */
    for (int i = 1; i < 240; i += 2)
    {
        ret += r16 / (x0 * (real)i) - r4 / (x1 * (real)i);
        x0 *= m0;
        x1 *= m1;
    }

    return ret;
}

real const real::R_0        = (real)0.0;
real const real::R_1        = (real)1.0;
real const real::R_2        = (real)2.0;
real const real::R_3        = (real)3.0;
real const real::R_10       = (real)10.0;

real const real::R_E        = exp(R_1);
real const real::R_LN2      = log(R_2);
real const real::R_LN10     = log(R_10);
real const real::R_LOG2E    = re(R_LN2);
real const real::R_LOG10E   = re(R_LN10);
real const real::R_PI       = fast_pi();
real const real::R_PI_2     = R_PI >> 1;
real const real::R_PI_3     = R_PI / R_3;
real const real::R_PI_4     = R_PI >> 2;
real const real::R_1_PI     = re(R_PI);
real const real::R_2_PI     = R_1_PI << 1;
real const real::R_2_SQRTPI = re(sqrt(R_PI)) << 1;
real const real::R_SQRT2    = sqrt(R_2);
real const real::R_SQRT3    = sqrt(R_3);
real const real::R_SQRT1_2  = R_SQRT2 >> 1;

} /* namespace lol */

