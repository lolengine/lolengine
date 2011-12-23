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

real::real()
{
    m_mantissa = new uint32_t[BIGITS];
    m_signexp = 0;
}

real::real(real const &x)
{
    m_mantissa = new uint32_t[BIGITS];
    memcpy(m_mantissa, x.m_mantissa, BIGITS * sizeof(uint32_t));
    m_signexp = x.m_signexp;
}

real const &real::operator =(real const &x)
{
    if (&x != this)
    {
        memcpy(m_mantissa, x.m_mantissa, BIGITS * sizeof(uint32_t));
        m_signexp = x.m_signexp;
    }

    return *this;
}

real::~real()
{
    delete[] m_mantissa;
}

real::real(float f) { new(this) real((double)f); }
real::real(int i) { new(this) real((double)i); }
real::real(unsigned int i) { new(this) real((double)i); }

real::real(double d)
{
    new(this) real();

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

    m_mantissa[0] = u.x >> 20;
    m_mantissa[1] = u.x << 12;
    memset(m_mantissa + 2, 0, (BIGITS - 2) * sizeof(m_mantissa[0]));
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
        u.x <<= 32;
        u.x |= m_mantissa[0];
        u.x <<= 20;
        u.x |= m_mantissa[1] >> 12;
        /* Rounding */
        u.x += (m_mantissa[1] >> 11) & 1;
    }

    return u.d;
}

/*
 * Create a real number from an ASCII representation
 */
real::real(char const *str)
{
    real ret = 0;
    int exponent = 0;
    bool comma = false, nonzero = false, negative = false, finished = false;

    for (char const *p = str; *p && !finished; p++)
    {
        switch (*p)
        {
        case '-':
        case '+':
            if (p != str)
                break;
            negative = (*p == '-');
            break;
        case '.':
            if (comma)
                finished = true;
            comma = true;
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            if (nonzero)
            {
                real x = ret + ret;
                x = x + x + ret;
                ret = x + x;
            }
            if (*p != '0')
            {
                ret += (int)(*p - '0');
                nonzero = true;
            }
            if (comma)
                exponent--;
            break;
        case 'e':
        case 'E':
            exponent += atoi(p + 1);
            finished = true;
            break;
        default:
            finished = true;
            break;
        }
    }

    if (exponent)
        ret *= pow(R_10, (real)exponent);

    if (negative)
        ret = -ret;

    new(this) real(ret);
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

    int bigoff = (e1 - e2) / BIGIT_BITS;
    int off = e1 - e2 - bigoff * BIGIT_BITS;

    if (bigoff > BIGITS)
        return *this;

    ret.m_signexp = m_signexp;

    uint64_t carry = 0;
    for (int i = BIGITS; i--; )
    {
        carry += m_mantissa[i];
        if (i - bigoff >= 0)
            carry += x.m_mantissa[i - bigoff] >> off;

        if (off && i - bigoff > 0)
            carry += (x.m_mantissa[i - bigoff - 1] << (BIGIT_BITS - off)) & 0xffffffffu;
        else if (i - bigoff == 0)
            carry += (uint64_t)1 << (BIGIT_BITS - off);

        ret.m_mantissa[i] = carry;
        carry >>= BIGIT_BITS;
    }

    /* Renormalise in case we overflowed the mantissa */
    if (carry)
    {
        carry--;
        for (int i = 0; i < BIGITS; i++)
        {
            uint32_t tmp = ret.m_mantissa[i];
            ret.m_mantissa[i] = (carry << (BIGIT_BITS - 1)) | (tmp >> 1);
            carry = tmp & 1u;
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

    int bigoff = (e1 - e2) / BIGIT_BITS;
    int off = e1 - e2 - bigoff * BIGIT_BITS;

    if (bigoff > BIGITS)
        return *this;

    ret.m_signexp = m_signexp;

    int64_t carry = 0;
    for (int i = 0; i < bigoff; i++)
    {
        carry -= x.m_mantissa[BIGITS - 1 - i];
        /* Emulates a signed shift */
        carry >>= BIGIT_BITS;
        carry |= carry << BIGIT_BITS;
    }
    if (bigoff < BIGITS)
        carry -= x.m_mantissa[BIGITS - 1 - bigoff] & (((int64_t)1 << off) - 1);
    carry /= (int64_t)1 << off;

    for (int i = BIGITS; i--; )
    {
        carry += m_mantissa[i];
        if (i - bigoff >= 0)
            carry -= x.m_mantissa[i - bigoff] >> off;

        if (off && i - bigoff > 0)
            carry -= (x.m_mantissa[i - bigoff - 1] << (BIGIT_BITS - off)) & 0xffffffffu;
        else if (i - bigoff == 0)
            carry -= (uint64_t)1 << (BIGIT_BITS - off);

        ret.m_mantissa[i] = carry;
        carry >>= BIGIT_BITS;
        carry |= carry << BIGIT_BITS;
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
                off += BIGIT_BITS;
                continue;
            }

            for (uint32_t tmp = ret.m_mantissa[i]; tmp < 0x80000000u; tmp <<= 1)
                off++;
            break;
        }
        if (off == BIGITS * BIGIT_BITS)
            ret.m_signexp &= 0x80000000u;
        else
        {
            off++; /* Shift one more to get rid of the leading one */
            ret.m_signexp -= off;

            bigoff = off / BIGIT_BITS;
            off -= bigoff * BIGIT_BITS;

            for (int i = 0; i < BIGITS; i++)
            {
                uint32_t tmp = 0;
                if (i + bigoff < BIGITS)
                    tmp |= ret.m_mantissa[i + bigoff] << off;
                if (off && i + bigoff + 1 < BIGITS)
                    tmp |= ret.m_mantissa[i + bigoff + 1] >> (BIGIT_BITS - off);
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
    uint64_t carry = 0, hicarry = 0, prev;
    for (int i = 0; i < BIGITS; i++)
    {
        for (int j = 0; j < i + 1; j++)
        {
            prev = carry;
            carry += (uint64_t)m_mantissa[BIGITS - 1 - j]
                   * (uint64_t)x.m_mantissa[BIGITS - 1 + j - i];
            if (carry < prev)
                hicarry++;
        }
        carry >>= BIGIT_BITS;
        carry |= hicarry << BIGIT_BITS;
        hicarry >>= BIGIT_BITS;
    }

    for (int i = 0; i < BIGITS; i++)
    {
        for (int j = i + 1; j < BIGITS; j++)
        {
            prev = carry;
            carry += (uint64_t)m_mantissa[BIGITS - 1 - j]
                   * (uint64_t)x.m_mantissa[j - 1 - i];
            if (carry < prev)
                hicarry++;
        }
        prev = carry;
        carry += m_mantissa[BIGITS - 1 - i];
        carry += x.m_mantissa[BIGITS - 1 - i];
        if (carry < prev)
            hicarry++;
        ret.m_mantissa[BIGITS - 1 - i] = carry & 0xffffffffu;
        carry >>= BIGIT_BITS;
        carry |= hicarry << BIGIT_BITS;
        hicarry >>= BIGIT_BITS;
    }

    /* Renormalise in case we overflowed the mantissa */
    if (carry)
    {
        carry--;
        for (int i = 0; i < BIGITS; i++)
        {
            uint32_t tmp = ret.m_mantissa[i];
            ret.m_mantissa[i] = (carry << (BIGIT_BITS - 1)) | (tmp >> 1);
            carry = tmp & 1u;
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

real const &real::operator +=(real const &x)
{
    real tmp = *this;
    return *this = tmp + x;
}

real const &real::operator -=(real const &x)
{
    real tmp = *this;
    return *this = tmp - x;
}

real const &real::operator *=(real const &x)
{
    real tmp = *this;
    return *this = tmp * x;
}

real const &real::operator /=(real const &x)
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

real const &real::operator <<=(int x)
{
    if (m_signexp << 1)
        m_signexp += x;
    return *this;
}

real const &real::operator >>=(int x)
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

    return memcmp(m_mantissa, x.m_mantissa, BIGITS * sizeof(uint32_t)) == 0;
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
    v.x |= x.m_mantissa[0] >> 9;
    v.f = 1.0 / v.f;

    real ret;
    ret.m_mantissa[0] = v.x << 9;

    uint32_t sign = x.m_signexp & 0x80000000u;
    ret.m_signexp = sign;

    int exponent = (x.m_signexp & 0x7fffffffu) + 1;
    exponent = -exponent + (v.x >> 23) - (u.x >> 23);
    ret.m_signexp |= (exponent - 1) & 0x7fffffffu;

    /* FIXME: 1+log2(BIGITS) steps of Newton-Raphson seems to be enough for
     * convergence, but this hasn't been checked seriously. */
    for (int i = 1; i <= real::BIGITS; i *= 2)
        ret = ret * (real::R_2 - ret * x);

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
    v.x |= x.m_mantissa[0] >> 9;
    v.f = 1.0 / sqrtf(v.f);

    real ret;
    ret.m_mantissa[0] = v.x << 9;

    uint32_t sign = x.m_signexp & 0x80000000u;
    ret.m_signexp = sign;

    uint32_t exponent = (x.m_signexp & 0x7fffffffu);
    exponent = ((1 << 30) + (1 << 29) - 1) - (exponent + 1) / 2;
    exponent = exponent + (v.x >> 23) - (u.x >> 23);
    ret.m_signexp |= exponent & 0x7fffffffu;

    /* FIXME: 1+log2(BIGITS) steps of Newton-Raphson seems to be enough for
     * convergence, but this hasn't been checked seriously. */
    for (int i = 1; i <= real::BIGITS; i *= 2)
    {
        ret = ret * (real::R_3 - ret * ret * x);
        ret.m_signexp--;
    }

    return ret * x;
}

real cbrt(real const &x)
{
    /* if zero, return x */
    if (!(x.m_signexp << 1))
        return x;

    /* Use the system's float inversion to approximate cbrt(x). First
     * we construct a float in the [1..8[ range that has roughly the same
     * mantissa as our real. Its exponent is 0, 1 or 2, depending on the
     * value of x. The final exponent is 0 or 1 (special case). We use
     * the final exponent and final mantissa to pre-fill the result. */
    union { float f; uint32_t x; } u = { 1.0f }, v = { 1.0f };
    v.x += ((x.m_signexp % 3) << 23);
    v.x |= x.m_mantissa[0] >> 9;
    v.f = powf(v.f, 0.33333333333333333f);

    real ret;
    ret.m_mantissa[0] = v.x << 9;

    uint32_t sign = x.m_signexp & 0x80000000u;
    ret.m_signexp = sign;

    int exponent = (x.m_signexp & 0x7fffffffu) - (1 << 30) + 1;
    exponent = exponent / 3 + (v.x >> 23) - (u.x >> 23);
    ret.m_signexp |= (exponent + (1 << 30) - 1) & 0x7fffffffu;

    /* FIXME: 1+log2(BIGITS) steps of Newton-Raphson seems to be enough for
     * convergence, but this hasn't been checked seriously. */
    for (int i = 1; i <= real::BIGITS; i *= 2)
    {
        static real third = re(real::R_3);
        ret = third * (x / (ret * ret) + (ret << 1));
    }

    return ret;
}

real pow(real const &x, real const &y)
{
    if (!y)
        return real::R_1;
    if (!x)
        return real::R_0;
    if (x > real::R_0)
        return exp(y * log(x));
    else /* x < 0 */
    {
        /* Odd integer exponent */
        if (y == (round(y >> 1) << 1))
            return exp(y * log(-x));

        /* Even integer exponent */
        if (y == round(y))
            return -exp(y * log(-x));

        /* FIXME: negative nth root */
        return real::R_0;
    }
}

real gamma(real const &x)
{
    /* We use Spouge's formula. FIXME: precision is far from acceptable,
     * especially with large values. We need to compute this with higher
     * precision values in order to attain the desired accuracy. It might
     * also be useful to sort the ck values by decreasing absolute value
     * and do the addition in this order. */
    int a = ceilf(logf(2) / logf(2 * M_PI) * real::BIGITS * real::BIGIT_BITS);

    real ret = sqrt(real::R_PI << 1);
    real fact_k_1 = real::R_1;

    for (int k = 1; k < a; k++)
    {
        real a_k = (real)(a - k);
        real ck = pow(a_k, (real)((float)k - 0.5)) * exp(a_k)
                / (fact_k_1 * (x + (real)(k - 1)));
        ret += ck;
        fact_k_1 *= (real)-k;
    }

    ret *= pow(x + (real)(a - 1), x - (real::R_1 >> 1));
    ret *= exp(-x - (real)(a - 1));

    return ret;
}

real fabs(real const &x)
{
    real ret = x;
    ret.m_signexp &= 0x7fffffffu;
    return ret;
}

static real fast_log(real const &x)
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
    real z = (y - real::R_1) / (y + real::R_1), z2 = z * z, zn = z2;
    real sum = real::R_1;

    for (int i = 3; ; i += 2)
    {
        real newsum = sum + zn / (real)i;
        if (newsum == sum)
            break;
        sum = newsum;
        zn *= z2;
    }

    return z * (sum << 2);
}

real log(real const &x)
{
    /* Strategy for log(x): if x = 2^E*M then log(x) = E log(2) + log(M),
     * with the property that M is in [1..2[, so fast_log() applies here. */
    real tmp = x;
    if (x.m_signexp >> 31 || x.m_signexp == 0)
    {
        tmp.m_signexp = 0xffffffffu;
        tmp.m_mantissa[0] = 0xffffffffu;
        return tmp;
    }
    tmp.m_signexp = (1 << 30) - 1;
    return (real)(int)(x.m_signexp - (1 << 30) + 1) * real::R_LN2
           + fast_log(tmp);
}

real log2(real const &x)
{
    /* Strategy for log2(x): see log(x). */
    real tmp = x;
    if (x.m_signexp >> 31 || x.m_signexp == 0)
    {
        tmp.m_signexp = 0xffffffffu;
        tmp.m_mantissa[0] = 0xffffffffu;
        return tmp;
    }
    tmp.m_signexp = (1 << 30) - 1;
    return (real)(int)(x.m_signexp - (1 << 30) + 1)
           + fast_log(tmp) * real::R_LOG2E;
}

real log10(real const &x)
{
    return log(x) * real::R_LOG10E;
}

static real fast_exp_sub(real const &x, real const &y)
{
    /* This fast exp method is tuned to work on the [-1..1] range and
     * no effort whatsoever was made to improve convergence outside this
     * domain of validity. The argument y is used for cases where we
     * don't want the leading 1 in the Taylor series. */
    real ret = real::R_1 - y, fact = real::R_1, xn = x;

    for (int i = 1; ; i++)
    {
        real newret = ret + xn;
        if (newret == ret)
            break;
        ret = newret;
        real mul = (i + 1);
        fact *= mul;
        ret *= mul;
        xn *= x;
    }
    ret /= fact;

    return ret;
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
    int e0 = x / real::R_LN2;
    real x0 = x - (real)e0 * real::R_LN2;
    real x1 = fast_exp_sub(x0, real::R_0);
    x1.m_signexp += e0;
    return x1;
}

real exp2(real const &x)
{
    /* Strategy for exp2(x): see strategy in exp(). */
    int e0 = x;
    real x0 = x - (real)e0;
    real x1 = fast_exp_sub(x0 * real::R_LN2, real::R_0);
    x1.m_signexp += e0;
    return x1;
}

real sinh(real const &x)
{
    /* We cannot always use (exp(x)-exp(-x))/2 because we'll lose
     * accuracy near zero. We only use this identity for |x|>0.5. If
     * |x|<=0.5, we compute exp(x)-1 and exp(-x)-1 instead. */
    bool near_zero = (fabs(x) < real::R_1 >> 1);
    real x1 = near_zero ? fast_exp_sub(x, real::R_1) : exp(x);
    real x2 = near_zero ? fast_exp_sub(-x, real::R_1) : exp(-x);
    return (x1 - x2) >> 1;
}

real tanh(real const &x)
{
    /* See sinh() for the strategy here */
    bool near_zero = (fabs(x) < real::R_1 >> 1);
    real x1 = near_zero ? fast_exp_sub(x, real::R_1) : exp(x);
    real x2 = near_zero ? fast_exp_sub(-x, real::R_1) : exp(-x);
    real x3 = near_zero ? x1 + x2 + real::R_2 : x1 + x2;
    return (x1 - x2) / x3;
}

real cosh(real const &x)
{
    /* No need to worry about accuracy here; maybe the last bit is slightly
     * off, but that's about it. */
    return (exp(x) + exp(-x)) >> 1;
}

real frexp(real const &x, int *exp)
{
    if (!x)
    {
        *exp = 0;
        return x;
    }

    real ret = x;
    int exponent = (ret.m_signexp & 0x7fffffffu) - (1 << 30) + 1;
    *exp = exponent + 1;
    ret.m_signexp -= exponent + 1;
    return ret;
}

real ldexp(real const &x, int exp)
{
    real ret = x;
    if (ret)
        ret.m_signexp += exp;
    return ret;
}

real modf(real const &x, real *iptr)
{
    real absx = fabs(x);
    real tmp = floor(absx);

    *iptr = copysign(tmp, x);
    return copysign(absx - tmp, x);
}

real copysign(real const &x, real const &y)
{
    real ret = x;
    ret.m_signexp &= 0x7fffffffu;
    ret.m_signexp |= y.m_signexp & 0x80000000u;
    return ret;
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
        else if (exponent < real::BIGIT_BITS)
            ret.m_mantissa[i] &= ~((1 << (real::BIGIT_BITS - exponent)) - 1);

        exponent -= real::BIGIT_BITS;
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

real round(real const &x)
{
    if (x < real::R_0)
        return -round(-x);

    return floor(x + (real::R_1 >> 1));
}

real fmod(real const &x, real const &y)
{
    if (!y)
        return real::R_0; /* FIXME: return NaN */

    if (!x)
        return x;

    real tmp = round(x / y);
    return x - tmp * y;
}

real sin(real const &x)
{
    bool switch_sign = x.m_signexp & 0x80000000u;

    real absx = fmod(fabs(x), real::R_PI << 1);
    if (absx > real::R_PI)
    {
        absx -= real::R_PI;
        switch_sign = !switch_sign;
    }

    if (absx > real::R_PI_2)
        absx = real::R_PI - absx;

    real ret = real::R_0, fact = real::R_1, xn = absx, mx2 = -absx * absx;
    for (int i = 1; ; i += 2)
    {
        real newret = ret + xn;
        if (newret == ret)
            break;
        ret = newret;
        real mul = (i + 1) * (i + 2);
        fact *= mul;
        ret *= mul;
        xn *= mx2;
    }
    ret /= fact;

    /* Propagate sign */
    if (switch_sign)
        ret.m_signexp ^= 0x80000000u;
    return ret;
}

real cos(real const &x)
{
    return sin(real::R_PI_2 - x);
}

real tan(real const &x)
{
    /* Constrain input to [-π,π] */
    real y = fmod(x, real::R_PI);

    /* Constrain input to [-π/2,π/2] */
    if (y < -real::R_PI_2)
        y += real::R_PI;
    else if (y > real::R_PI_2)
        y -= real::R_PI;

    /* In [-π/4,π/4] return sin/cos */
    if (fabs(y) <= real::R_PI_4)
        return sin(y) / cos(y);

    /* Otherwise, return cos/sin */
    if (y > real::R_0)
        y = real::R_PI_2 - y;
    else
        y = -real::R_PI_2 - y;

    return cos(y) / sin(y);
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
    for (int i = 1; ; i++)
    {
        xn *= x2;
        real mul = (real)(2 * i + 1);
        real newret = ret + ((fact1 * xn / (mul * fact2)) >> (i * 2));
        if (newret == ret)
            break;
        ret = newret;
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
        for (int i = 3; ; i += 2)
        {
            xn *= mx2;
            real newret = ret + xn / (real)i;
            if (newret == ret)
                break;
            ret = newret;
        }
        return ret;
    }

    real ret = 0;

    if (absx < (real::R_3 >> 1))
    {
        real y = real::R_1 - absx;
        real yn = y, my2 = -y * y;
        for (int i = 0; ; i += 2)
        {
            real newret = ret + ((yn / (real)(2 * i + 1)) >> (i + 1));
            yn *= y;
            newret += (yn / (real)(2 * i + 2)) >> (i + 1);
            yn *= y;
            newret += (yn / (real)(2 * i + 3)) >> (i + 2);
            if (newret == ret)
                break;
            ret = newret;
            yn *= my2;
        }
        ret = real::R_PI_4 - ret;
    }
    else if (absx < real::R_2)
    {
        real y = (absx - real::R_SQRT3) >> 1;
        real yn = y, my2 = -y * y;
        for (int i = 1; ; i += 6)
        {
            real newret = ret + ((yn / (real)i) >> 1);
            yn *= y;
            newret -= (real::R_SQRT3 >> 1) * yn / (real)(i + 1);
            yn *= y;
            newret += yn / (real)(i + 2);
            yn *= y;
            newret -= (real::R_SQRT3 >> 1) * yn / (real)(i + 3);
            yn *= y;
            newret += (yn / (real)(i + 4)) >> 1;
            if (newret == ret)
                break;
            ret = newret;
            yn *= my2;
        }
        ret = real::R_PI_3 + ret;
    }
    else
    {
        real y = re(absx);
        real yn = y, my2 = -y * y;
        ret = y;
        for (int i = 3; ; i += 2)
        {
            yn *= my2;
            real newret = ret + yn / (real)i;
            if (newret == ret)
                break;
            ret = newret;
        }
        ret = real::R_PI_2 - ret;
    }

    /* Propagate sign */
    ret.m_signexp |= (x.m_signexp & 0x80000000u);
    return ret;
}

real atan2(real const &y, real const &x)
{
    if (!y)
    {
        if ((x.m_signexp >> 31) == 0)
            return y;
        if (y.m_signexp >> 31)
            return -real::R_PI;
        return real::R_PI;
    }

    if (!x)
    {
        if (y.m_signexp >> 31)
            return -real::R_PI;
        return real::R_PI;
    }

    /* FIXME: handle the Inf and NaN cases */
    real z = y / x;
    real ret = atan(z);
    if (x < real::R_0)
        ret += (y > real::R_0) ? real::R_PI : -real::R_PI;
    return ret;
}

void real::hexprint() const
{
    printf("%08x", m_signexp);
    for (int i = 0; i < BIGITS; i++)
        printf(" %08x", m_mantissa[i]);
    printf("\n");
}

void real::print(int ndigits) const
{
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
        for (real div = R_1, newdiv; true; div = newdiv)
        {
            newdiv = div * R_10;
            if (x < newdiv)
            {
                x /= div;
                break;
            }
            exponent++;
        }
        for (real mul = 1, newx; true; mul *= R_10)
        {
            newx = x * mul;
            if (newx >= R_1)
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
        x *= R_10;
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

    for (int i = 1; ; i += 2)
    {
        real newret = ret + r16 / (x0 * (real)i) - r4 / (x1 * (real)i);
        if (newret == ret)
            break;
        ret = newret;
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

real const real::R_LN2      = fast_log(R_2);
real const real::R_LN10     = log(R_10);
real const real::R_LOG2E    = re(R_LN2);
real const real::R_LOG10E   = re(R_LN10);
real const real::R_E        = exp(R_1);
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

