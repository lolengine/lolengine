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
    return *this * fres(x);
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

real fres(real const &x)
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

    int exponent = (x.m_signexp & 0x7fffffffu) - ((1 << 30) - 1);
    exponent = - (exponent / 2) + (v.x >> 23) - (u.x >> 23);
    ret.m_signexp |= (exponent + ((1 << 30) - 1)) & 0x7fffffffu;

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

    /* FIXME: sum.m_signexp += 2; (but needs private data access) */
    sum *= (real)4;
    return z * sum;
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
     * such that exp(x) = 2^E0 * exp(x0). We get x0 = x - log(E0).
     *
     * Thus the final algorithm:
     *  int E0 = x / log(2)
     *  real x0 = x - log(E0)
     *  real x1 = exp(x0)
     *  return x1 * 2^E0
     */
    int square = 0;

    /* FIXME: this is slow. Find a better way to approximate exp(x) for
     * large values. */
    real tmp = x, one = 1.0;
    while (tmp > one)
    {
        tmp.m_signexp--;
        square++;
    }

    real ret = 1.0, fact = 1.0, xn = tmp;

    for (int i = 1; i < 100; i++)
    {
        fact *= (real)i;
        ret += xn / fact;
        xn *= tmp;
    }

    for (int i = 0; i < square; i++)
        ret = ret * ret;

    return ret;
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

} /* namespace lol */

