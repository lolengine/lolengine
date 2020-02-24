//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <new>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <cmath>

namespace lol
{

/*
 * Initialisation order is not important because everything is
 * done on demand, but here is the dependency list anyway:
 *  - fast_log() requires R_1
 *  - log() requires R_LN2
 *  - inverse() require R_2
 *  - exp() requires R_0, R_1, R_LN2
 *  - sqrt() requires R_3
 */

static real fast_log(real const &x);

static real load_min();
static real load_max();
static real load_pi();

/* These getters do not need caching, their return values are small */
template<> inline real const real::R_0() { return real(); }
template<> inline real const real::R_INF() { real ret; ret.m_inf = true; return ret; }
template<> inline real const real::R_NAN() { real ret; ret.m_nan = true; return ret; }

#define LOL_CONSTANT_GETTER(name, value) \
    template<> inline real const& real::name() \
    { \
        static real ret; \
        static int prev_bigit_count = -1; \
        /* If the default bigit count has changed, we must recompute
         * the value with the desired precision. */ \
        if (prev_bigit_count != global_bigit_count()) \
        { \
            ret = (value); \
            prev_bigit_count = global_bigit_count(); \
        } \
        return ret; \
    }

LOL_CONSTANT_GETTER(R_1,        real(1.0));
LOL_CONSTANT_GETTER(R_2,        real(2.0));
LOL_CONSTANT_GETTER(R_3,        real(3.0));
LOL_CONSTANT_GETTER(R_10,       real(10.0));

LOL_CONSTANT_GETTER(R_MIN,      load_min());
LOL_CONSTANT_GETTER(R_MAX,      load_max());

LOL_CONSTANT_GETTER(R_LN2,      fast_log(R_2()));
LOL_CONSTANT_GETTER(R_LN10,     log(R_10()));
LOL_CONSTANT_GETTER(R_LOG2E,    inverse(R_LN2()));
LOL_CONSTANT_GETTER(R_LOG10E,   inverse(R_LN10()));
LOL_CONSTANT_GETTER(R_E,        exp(R_1()));
LOL_CONSTANT_GETTER(R_PI,       load_pi());
LOL_CONSTANT_GETTER(R_PI_2,     R_PI() / 2);
LOL_CONSTANT_GETTER(R_PI_3,     R_PI() / R_3());
LOL_CONSTANT_GETTER(R_PI_4,     R_PI() / 4);
LOL_CONSTANT_GETTER(R_TAU,      R_PI() + R_PI());
LOL_CONSTANT_GETTER(R_1_PI,     inverse(R_PI()));
LOL_CONSTANT_GETTER(R_2_PI,     R_1_PI() * 2);
LOL_CONSTANT_GETTER(R_2_SQRTPI, inverse(sqrt(R_PI())) * 2);
LOL_CONSTANT_GETTER(R_SQRT2,    sqrt(R_2()));
LOL_CONSTANT_GETTER(R_SQRT3,    sqrt(R_3()));
LOL_CONSTANT_GETTER(R_SQRT1_2,  R_SQRT2() / 2);

#undef LOL_CONSTANT_GETTER

/*
 * Now carry on with the rest of the Real class.
 */

template<> inline real::Real(int32_t i) { new(this) real((double)i); }
template<> inline real::Real(uint32_t i) { new(this) real((double)i); }
template<> inline real::Real(float f) { new(this) real((double)f); }

template<> inline real::Real(int64_t i)
{
    new(this) real((uint64_t)std::abs(i));
    m_sign = i < 0;
}

template<> inline real::Real(uint64_t i)
{
    new(this) real();
    if (i)
    {
        /* Only works with 32-bit bigits for now */
        static_assert(sizeof(bigit_t) == 4, "bigit_t must be 32-bit");

        int delta = 1;
        while ((i >> 63) == 0)
        {
            i <<= 1;
            ++delta;
        }
        i <<= 1; /* Remove implicit one */

        m_exponent = 64 - delta;
        m_mantissa.resize(DEFAULT_BIGIT_COUNT);
        m_mantissa[0] = (bigit_t)(i >> 32);
        if (bigit_count() > 1)
            m_mantissa[1] = (bigit_t)i;
    }
}

template<> inline real::Real(double d)
{
    union { double d; uint64_t x; } u = { d };

    m_sign = bool(u.x >> 63);

    exponent_t exponent = (u.x << 1) >> 53;

    switch (exponent)
    {
    case 0x00: /* +0 / -0 */
        break;
    case 0x7ff: /* Inf/NaN (FIXME: handle NaN!) */
        m_inf = true;
        break;
    default:
        /* Only works with 32-bit bigits for now */
        static_assert(sizeof(bigit_t) == 4, "bigit_t must be 32-bit");
        m_exponent = exponent - ((1 << 10) - 1);
        m_mantissa.resize(DEFAULT_BIGIT_COUNT);
        m_mantissa[0] = (bigit_t)(u.x >> 20);
        if (bigit_count() > 1)
            m_mantissa[1] = (bigit_t)(u.x << 12);
        break;
    }
}

template<> inline real::Real(long double f)
{
    /* We don’t know the long double layout, so we get rid of the
     * exponent, then load it into a real in two steps. */
    int exponent;
    f = frexpl(f, &exponent);
    new(this) real(double(f));
    *this += double(f - (long double)*this);
    m_exponent += exponent;
}

template<> inline real::operator float() const { return (float)(double)*this; }
template<> inline real::operator int32_t() const { return (int32_t)(double)floor(*this); }
template<> inline real::operator uint32_t() const { return (uint32_t)(double)floor(*this); }

template<> inline real::operator uint64_t() const
{
    uint32_t msb = (uint32_t)ldexp(*this, -32);
    uint64_t ret = ((uint64_t)msb << 32)
                 | (uint32_t)(*this - ldexp((real)msb, 32));
    return ret;
}

template<> inline real::operator int64_t() const
{
    /* If number is positive, convert it to uint64_t first. If it is
     * negative, switch its sign first. */
    return is_negative() ? -(int64_t)-*this : (int64_t)(uint64_t)*this;
}

template<> inline real::operator double() const
{
    union { double d; uint64_t x; } u;

    /* Get sign */
    u.x = (is_negative() ? 1 : 0) << 11;

    /* Compute new exponent (FIXME: handle Inf/NaN) */
    int64_t e = m_exponent + ((1 << 10) - 1);

    if (is_zero())
        u.x <<= 52;
    else if (e < 0) /* if exponent underflows, set to zero */
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
        if (bigit_count() > 0)
            u.x |= m_mantissa[0];
        u.x <<= 20;
        if (bigit_count() > 1)
        {
            u.x |= m_mantissa[1] >> 12;
            /* Rounding */
            u.x += (m_mantissa[1] >> 11) & 1;
        }
    }

    return u.d;
}

template<> inline real::operator long double() const
{
    double hi = double(*this);
    double lo = double(*this - hi);
    return (long double)(hi) + (long double)(lo);
}

/*
 * Create a real number from an ASCII representation
 */
template<> inline real::Real(char const *str)
{
    real ret = 0;
    exponent_t exponent = 0;
    bool hex = false, comma = false, nonzero = false, negative = false, finished = false;

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
        case 'x':
        case 'X':
            /* This character is only valid for 0x... and 0X... numbers */
            if (p != str + 1 || str[0] != '0')
                finished = true;
            hex = true;
            break;
        case 'p':
        case 'P':
            if (hex)
                exponent += atoi(p + 1);
            finished = true;
            break;
        case 'e':
        case 'E':
            if (!hex)
            {
                exponent += atoi(p + 1);
                finished = true;
                break;
            }
            LOL_ATTR_FALLTHROUGH
        case 'a': case 'b': case 'c': case 'd': case 'f':
        case 'A': case 'B': case 'C': case 'D': case 'F':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            if (nonzero)
            {
                /* Multiply ret by 10 or 16 depending the base. */
                if (!hex)
                {
                    real x = ret + ret;
                    ret = x + x + ret;
                }
                ret.m_exponent += hex ? 4 : 1;
            }
            if (*p != '0')
            {
                ret += (*p >= 'a' && *p <= 'f') ? (int)(*p - 'a' + 10)
                     : (*p >= 'A' && *p <= 'F') ? (int)(*p - 'A' + 10)
                     : (int)(*p - '0');
                nonzero = true;
            }
            if (comma)
                exponent -= hex ? 4 : 1;
            break;
        default:
            finished = true;
            break;
        }
    }

    if (hex)
        ret.m_exponent += exponent;
    else if (exponent)
        ret *= pow(R_10(), (real)exponent);

    if (negative)
        ret = -ret;

    *this = ret;
}

template<> inline real real::operator +() const
{
    return *this;
}

template<> inline real real::operator -() const
{
    real ret = *this;
    ret.m_sign ^= true;
    return ret;
}

template<> inline real real::operator +(real const &x) const
{
    if (x.is_zero())
        return *this;

    if (is_zero())
        return x;

    /* Ensure both arguments are positive. Otherwise, switch signs,
     * or replace + with -. */
    if (is_negative())
        return -(-*this + -x);

    if (x.is_negative())
        return *this - (-x);

    /* Ensure *this has the larger exponent (no need for the mantissa to
     * be larger, as in subtraction). Otherwise, switch. */
    if (m_exponent < x.m_exponent)
        return x + *this;

    int64_t e1 = m_exponent;
    int64_t e2 = x.m_exponent;

    int64_t bigoff = (e1 - e2) / bigit_bits();
    int64_t off = e1 - e2 - bigoff * bigit_bits();

    /* FIXME: ensure we have the same number of bigits */
    if (bigoff > bigit_count())
        return *this;

    real ret;
    ret.m_mantissa.resize(bigit_count());
    ret.m_exponent = m_exponent;

    uint64_t carry = 0;
    for (int i = bigit_count(); i--; )
    {
        carry += m_mantissa[i];
        if (i - bigoff >= 0)
            carry += x.m_mantissa[i - bigoff] >> off;

        if (off && i - bigoff > 0)
            carry += (x.m_mantissa[i - bigoff - 1] << (bigit_bits() - off)) & 0xffffffffu;
        else if (i - bigoff == 0)
            carry += (uint64_t)1 << (bigit_bits() - off);

        ret.m_mantissa[i] = (uint32_t)carry;
        carry >>= bigit_bits();
    }

    /* Renormalise in case we overflowed the mantissa */
    if (carry)
    {
        carry--;
        for (int i = 0; i < bigit_count(); ++i)
        {
            uint32_t tmp = ret.m_mantissa[i];
            ret.m_mantissa[i] = ((uint32_t)carry << (bigit_bits() - 1))
                              | (tmp >> 1);
            carry = tmp & 1u;
        }
        ret.m_exponent++;
    }

    return ret;
}

template<> inline real real::operator -(real const &x) const
{
    if (x.is_zero())
        return *this;

    if (is_zero())
        return -x;

    /* Ensure both arguments are positive. Otherwise, switch signs,
     * or replace - with +. */
    if (is_negative())
        return -(-*this + x);

    if (x.is_negative())
        return (*this) + (-x);

    /* Ensure *this is larger than x */
    if (*this < x)
        return -(x - *this);

    exponent_t e1 = m_exponent;
    exponent_t e2 = x.m_exponent;

    exponent_t bigoff = (e1 - e2) / bigit_bits();
    exponent_t off = e1 - e2 - bigoff * bigit_bits();

    /* FIXME: ensure we have the same number of bigits */
    if (bigoff > bigit_count())
        return *this;

    real ret;
    ret.m_mantissa.resize(bigit_count());
    ret.m_exponent = m_exponent;

    /* int64_t instead of uint64_t to preserve sign through shifts */
    exponent_t carry = 0;
    for (int i = 0; i < bigoff; ++i)
    {
        carry -= x.m_mantissa[bigit_count() - 1 - i];
        /* Emulates a signed shift */
        carry >>= bigit_bits();
        carry |= carry << bigit_bits();
    }
    if (bigoff < bigit_count())
        carry -= x.m_mantissa[bigit_count() - 1 - bigoff] & (((exponent_t)1 << off) - 1);
    carry /= (exponent_t)1 << off;

    for (int i = bigit_count(); i--; )
    {
        carry += m_mantissa[i];
        if (i - bigoff >= 0)
            carry -= x.m_mantissa[i - bigoff] >> off;

        if (off && i - bigoff > 0)
            carry -= (x.m_mantissa[i - bigoff - 1] << (bigit_bits() - off)) & 0xffffffffu;
        else if (i - bigoff == 0)
            carry -= (uint64_t)1 << (bigit_bits() - off);

        ret.m_mantissa[i] = (bigit_t)carry;
        carry >>= bigit_bits();
        carry |= carry << bigit_bits();
    }

    carry += 1;

    /* Renormalise if we underflowed the mantissa */
    if (carry == 0)
    {
        /* How much do we need to shift the mantissa? FIXME: this could
         * be computed above */
        off = 0;
        for (int i = 0; i < bigit_count(); ++i)
        {
            if (!ret.m_mantissa[i])
            {
                off += bigit_bits();
                continue;
            }

            /* “~tmp > tmp” checks that the MSB is not set */
            for (bigit_t tmp = ret.m_mantissa[i]; ~tmp > tmp; tmp <<= 1)
                off++;
            break;
        }
        if (off == total_bits())
            ret.m_mantissa.resize(0);
        else
        {
            off++; /* Shift once more to get rid of the leading 1 */
            ret.m_exponent -= off;

            bigoff = off / bigit_bits();
            off -= bigoff * bigit_bits();

            for (int i = 0; i < bigit_count(); ++i)
            {
                bigit_t tmp = 0;
                if (i + bigoff < bigit_count())
                    tmp |= ret.m_mantissa[i + bigoff] << off;
                if (off && i + bigoff + 1 < bigit_count())
                    tmp |= ret.m_mantissa[i + bigoff + 1] >> (bigit_bits() - off);
                ret.m_mantissa[i] = tmp;
            }
        }
    }

    return ret;
}

template<> inline real real::operator *(real const &x) const
{
    real ret;

    /* The sign is easy to compute */
    ret.m_sign = is_negative() ^ x.is_negative();

    /* If any operand is zero, return zero. FIXME: 0 * Inf? */
    if (is_zero() || x.is_zero())
        return ret;

    ret.m_mantissa.resize(bigit_count());
    ret.m_exponent = m_exponent + x.m_exponent;

    /* Accumulate low order product; no need to store it, we just
     * want the carry value */
    uint64_t carry = 0, hicarry = 0, prev;
    for (int i = 0; i < bigit_count(); ++i)
    {
        for (int j = 0; j < i + 1; j++)
        {
            prev = carry;
            carry += (uint64_t)m_mantissa[bigit_count() - 1 - j]
                   * (uint64_t)x.m_mantissa[bigit_count() - 1 + j - i];
            if (carry < prev)
                hicarry++;
        }
        carry >>= bigit_bits();
        carry |= hicarry << bigit_bits();
        hicarry >>= bigit_bits();
    }

    /* Multiply the other components */
    for (int i = 0; i < bigit_count(); ++i)
    {
        for (int j = i + 1; j < bigit_count(); j++)
        {
            prev = carry;
            carry += (uint64_t)m_mantissa[bigit_count() - 1 - j]
                   * (uint64_t)x.m_mantissa[j - 1 - i];
            if (carry < prev)
                hicarry++;
        }
        prev = carry;
        carry += m_mantissa[bigit_count() - 1 - i];
        carry += x.m_mantissa[bigit_count() - 1 - i];
        if (carry < prev)
            hicarry++;
        ret.m_mantissa[bigit_count() - 1 - i] = carry & ~(bigit_t)0;
        carry >>= bigit_bits();
        carry |= hicarry << bigit_bits();
        hicarry >>= bigit_bits();
    }

    /* Renormalise in case we overflowed the mantissa */
    if (carry)
    {
        carry--;
        for (int i = 0; i < bigit_count(); ++i)
        {
            bigit_t tmp = ret.m_mantissa[i];
            ret.m_mantissa[i] = ((bigit_t)carry << (bigit_bits() - 1))
                              | (tmp >> 1);
            carry = tmp & 1u;
        }
        ++ret.m_exponent;
    }

    return ret;
}

template<> inline real real::operator /(real const &x) const
{
    return *this * inverse(x);
}

template<> inline real const &real::operator +=(real const &x)
{
    real tmp = *this;
    return *this = tmp + x;
}

template<> inline real const &real::operator -=(real const &x)
{
    real tmp = *this;
    return *this = tmp - x;
}

template<> inline real const &real::operator *=(real const &x)
{
    real tmp = *this;
    return *this = tmp * x;
}

template<> inline real const &real::operator /=(real const &x)
{
    real tmp = *this;
    return *this = tmp / x;
}

template<> inline bool real::operator ==(real const &x) const
{
    /* If NaN is involved, return false */
    if (is_nan() || x.is_nan())
        return false;

    /* If both zero, they are equal; if either is zero, they are different */
    if (is_zero() || x.is_zero())
        return is_zero() && x.is_zero();

    /* FIXME: handle NaN/Inf */
    return m_exponent == x.m_exponent && m_mantissa == x.m_mantissa;
}

template<> inline bool real::operator !=(real const &x) const
{
    return !(is_nan() || x.is_nan() || *this == x);
}

template<> inline bool real::operator <(real const &x) const
{
    /* If NaN is involved, return false */
    if (is_nan() || x.is_nan())
        return false;

    /* Ensure we are positive */
    if (is_negative())
        return -*this > -x;

    /* If x is zero or negative, we can’t be < x */
    if (x.is_negative() || x.is_zero())
        return false;

    /* If we are zero, we must be < x */
    if (is_zero())
        return true;

    /* Compare exponents */
    if (m_exponent != x.m_exponent)
        return m_exponent < x.m_exponent;

    /* Compare all relevant bits */
    for (int i = 0; i < bigit_count(); ++i)
        if (m_mantissa[i] != x.m_mantissa[i])
            return m_mantissa[i] < x.m_mantissa[i];

    return false;
}

template<> inline bool real::operator <=(real const &x) const
{
    return !(is_nan() || x.is_nan() || *this > x);
}

template<> inline bool real::operator >(real const &x) const
{
    /* If NaN is involved, return false */
    if (is_nan() || x.is_nan())
        return false;

    /* Ensure we are positive */
    if (is_negative())
        return -*this < -x;

    /* If x is zero, we’re > x iff we’re non-zero since we’re positive */
    if (x.is_zero())
        return !is_zero();

    /* If x is strictly negative, we’re > x */
    if (x.is_negative())
        return true;

    /* If we are zero, we can’t be > x */
    if (is_zero())
        return false;

    /* Compare exponents */
    if (m_exponent != x.m_exponent)
        return m_exponent > x.m_exponent;

    /* Compare all relevant bits */
    for (int i = 0; i < bigit_count(); ++i)
        if (m_mantissa[i] != x.m_mantissa[i])
            return m_mantissa[i] > x.m_mantissa[i];

    return false;
}

template<> inline bool real::operator >=(real const &x) const
{
    return !(is_nan() || x.is_nan() || *this < x);
}

template<> inline bool real::operator !() const
{
    return !(bool)*this;
}

template<> inline real::operator bool() const
{
    /* A real is "true" if it is non-zero AND not NaN */
    return !is_zero() && !is_nan();
}

template<> inline real min(real const &a, real const &b)
{
    return (a < b) ? a : b;
}

template<> inline real max(real const &a, real const &b)
{
    return (a > b) ? a : b;
}

template<> inline real clamp(real const &x, real const &a, real const &b)
{
    return (x < a) ? a : (x > b) ? b : x;
}

template<> inline real inverse(real const &x)
{
    real ret;

    /* If zero, return infinite */
    if (x.is_zero())
        return copysign(real::R_INF(), x);

    /* Use the system’s float inversion to approximate 1/x */
    union { float f; uint32_t x; } u = { 1.0f };
    u.x |= x.m_mantissa[0] >> 9;
    u.f = 1.0f / u.f;

    ret.m_mantissa.resize(x.bigit_count());
    ret.m_mantissa[0] = u.x << 9;
    ret.m_sign = x.m_sign;
    ret.m_exponent = -x.m_exponent + (u.x >> 23) - 0x7f;

    /* FIXME: 1+log2(bigit_count) steps of Newton-Raphson seems to be enough for
     * convergence, but this hasn’t been checked seriously. */
    for (int i = 1; i <= x.bigit_count(); i *= 2)
        ret = ret * (real::R_2() - ret * x);

    return ret;
}

template<> inline real sqrt(real const &x)
{
    /* if zero, return x (FIXME: negative zero?) */
    if (x.is_zero())
        return x;

    /* if negative, return NaN */
    if (x.is_negative())
        return real::R_NAN();

    int tweak = x.m_exponent & 1;

    /* Use the system’s float inversion to approximate 1/sqrt(x). First
     * we construct a float in the [1..4[ range that has roughly the same
     * mantissa as our real. Its exponent is 0 or 1, depending on the
     * parity of x’s exponent. The final exponent is 0, -1 or -2. We use
     * the final exponent and final mantissa to pre-fill the result. */
    union { float f; uint32_t x; } u = { 1.0f };
    u.x += tweak << 23;
    u.x |= x.m_mantissa[0] >> 9;
    u.f = 1.0f / sqrtf(u.f);

    real ret;
    ret.m_mantissa.resize(x.bigit_count());
    ret.m_mantissa[0] = u.x << 9;

    ret.m_exponent = -(x.m_exponent - tweak) / 2 + (u.x >> 23) - 0x7f;

    /* FIXME: 1+log2(bigit_count()) steps of Newton-Raphson seems to be enough for
     * convergence, but this hasn’t been checked seriously. */
    for (int i = 1; i <= x.bigit_count(); i *= 2)
    {
        ret = ret * (real::R_3() - ret * ret * x);
        --ret.m_exponent;
    }

    return ret * x;
}

template<> inline real cbrt(real const &x)
{
    /* if zero, return x */
    if (x.is_zero())
        return x;

    int tweak = x.m_exponent % 3;
    if (tweak < 0)
        tweak += 3;

    /* Use the system’s float inversion to approximate cbrt(x). First
     * we construct a float in the [1..8[ range that has roughly the same
     * mantissa as our real. Its exponent is 0, 1 or 2, depending on the
     * value of x. The final exponent is 0 or 1 (special case). We use
     * the final exponent and final mantissa to pre-fill the result. */
    union { float f; uint32_t x; } u = { 1.0f };
    u.x += tweak << 23;
    u.x |= x.m_mantissa[0] >> 9;
    u.f = powf(u.f, 1.f / 3);

    real ret;
    ret.m_mantissa.resize(x.bigit_count());
    ret.m_mantissa[0] = u.x << 9;
    ret.m_exponent = (x.m_exponent - tweak) / 3 + (u.x >> 23) - 0x7f;
    ret.m_sign = x.m_sign;

    /* FIXME: 1+log2(bigit_count()) steps of Newton-Raphson seems to be enough
     * for convergence, but this hasn’t been checked seriously. */
    real third = inverse(real::R_3());
    for (int i = 1; i <= x.bigit_count(); i *= 2)
    {
        ret = third * (x / (ret * ret) + (ret * 2));
    }

    return ret;
}

template<> inline real pow(real const &x, real const &y)
{
    /* Shortcuts for degenerate cases */
    if (!y)
        return real::R_1();
    if (!x)
        return real::R_0();

    /* Small integer exponent: use exponentiation by squaring */
    int64_t int_y = (int64_t)y;
    if (y == (real)int_y)
    {
        real ret = real::R_1();
        real x_n = int_y > 0 ? x : inverse(x);

        while (int_y) /* Can be > 0 or < 0 */
        {
            if (int_y & 1)
                ret *= x_n;
            x_n *= x_n;
            int_y /= 2;
        }

        return ret;
    }

    /* If x is positive, nothing special to do. */
    if (x > real::R_0())
        return exp(y * log(x));

    /* XXX: manpage for pow() says “If x is a finite value less than 0,
     * and y is a finite noninteger, a domain error occurs, and a NaN is
     * returned”. We check whether y is closer to an even number or to
     * an odd number and return something reasonable. */
    real round_y = round(y);
    bool is_odd = round_y / 2 == round(round_y / 2);
    return is_odd ? exp(y * log(-x)) : -exp(y * log(-x));
}

/* A fast factorial implementation for small numbers. An optional
 * step argument allows to compute double factorials (i.e. with
 * only the odd or the even terms. */
static real fast_fact(int x, int step = 1)
{
    if (x < step)
        return 1;

    if (x == step)
        return x;

    unsigned int start = (x + step - 1) % step + 1;
    real ret(start);
    uint64_t multiplier = 1;

    for (int i = start, exponent = 0;;)
    {
        if (i >= x)
            return ldexp(ret * multiplier, exponent);

        i += step;

        /* Accumulate the power of two part in the exponent */
        unsigned int tmp = i;
        while ((tmp & 1) == 0)
        {
            tmp >>= 1;
            exponent++;
        }

        /* Accumulate the other factors in the multiplier */
        if (multiplier * tmp / tmp != multiplier)
        {
            ret *= multiplier;
            multiplier = 1;
        }
        multiplier *= tmp;
    }
}

template<> inline real gamma(real const &x)
{
    static float pi = acosf(-1.f);

    /* We use Spouge’s formula. FIXME: precision is far from acceptable,
     * especially with large values. We need to compute this with higher
     * precision values in order to attain the desired accuracy. It might
     * also be useful to sort the ck values by decreasing absolute value
     * and do the addition in this order. */
    int a = (int)ceilf(logf(2) / logf(2 * pi) * x.total_bits());

    real ret = sqrt(real::R_PI() * 2);
    real fact_k_1 = real::R_1();

    for (int k = 1; k < a; k++)
    {
        real a_k = (real)(a - k);
        real ck = pow(a_k, (real)((float)k - 0.5)) * exp(a_k)
                / (fact_k_1 * (x + (real)(k - 1)));
        ret += ck;
        fact_k_1 *= (real)-k;
    }

    ret *= pow(x + (real)(a - 1), x - (real::R_1() / 2));
    ret *= exp(-x - (real)(a - 1));

    return ret;
}

template<> inline real fabs(real const &x)
{
    real ret = x;
    ret.m_sign = false;
    return ret;
}

template<> inline real abs(real const &x)
{
    return fabs(x);
}

template<> inline real fract(real const &x)
{
    return x - floor(x);
}

template<> inline real degrees(real const &x)
{
    /* FIXME: need to recompute this for different mantissa sizes */
    static real mul = real(180) * real::R_1_PI();

    return x * mul;
}

template<> inline real radians(real const &x)
{
    /* FIXME: need to recompute this for different mantissa sizes */
    static real mul = real::R_PI() / real(180);

    return x * mul;
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
    real z = (y - real::R_1()) / (y + real::R_1()), z2 = z * z, zn = z2;
    real sum = real::R_1();

    for (int i = 3; ; i += 2)
    {
        real newsum = sum + zn / (real)i;
        if (newsum == sum)
            break;
        sum = newsum;
        zn *= z2;
    }

    return z * sum * 4;
}

template<> inline real log(real const &x)
{
    /* Strategy for log(x): if x = 2^E*M then log(x) = E log(2) + log(M),
     * with the property that M is in [1..2[, so fast_log() applies here. */
    if (x.is_negative() || x.is_zero())
        return real::R_NAN();

    real tmp(x);
    tmp.m_exponent = 0;
    return real(x.m_exponent) * real::R_LN2() + fast_log(tmp);
}

template<> inline real log2(real const &x)
{
    /* Strategy for log2(x): see log(x). */
    if (x.is_negative() || x.is_zero())
        return real::R_NAN();

    real tmp(x);
    tmp.m_exponent = 0;
    return real(x.m_exponent) + fast_log(tmp) * real::R_LOG2E();
}

template<> inline real log10(real const &x)
{
    return log(x) * real::R_LOG10E();
}

static real fast_exp_sub(real const &x, real const &y)
{
    /* This fast exp method is tuned to work on the [-1..1] range and
     * no effort whatsoever was made to improve convergence outside this
     * domain of validity. The argument y is used for cases where we
     * don’t want the leading 1 in the Taylor series. */
    real ret = real::R_1() - y, xn = x;
    int i = 1;

    for (;;)
    {
        real newret = ret + xn;
        if (newret == ret)
            break;
        ret = newret * ++i;
        xn *= x;
    }

    return ret / fast_fact(i);
}

template<> inline real exp(real const &x)
{
    /* Strategy for exp(x): the Taylor series does not converge very fast
     * with large positive or negative values.
     *
     * However, since the result is going to be in the form M*2^E, we first
     * try to predict a value for E, which is approximately:
     *  E ≈ log2(exp(x)) = x / log(2)
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
    real::exponent_t e0 = x / real::R_LN2();
    real x0 = x - (real)e0 * real::R_LN2();
    real x1 = fast_exp_sub(x0, real::R_0());
    x1.m_exponent += e0;
    return x1;
}

template<> inline real exp2(real const &x)
{
    /* Strategy for exp2(x): see strategy in exp(). */
    real::exponent_t e0 = x;
    real x0 = x - (real)e0;
    real x1 = fast_exp_sub(x0 * real::R_LN2(), real::R_0());
    x1.m_exponent += e0;
    return x1;
}

template<> inline real erf(real const &x)
{
    /* Strategy for erf(x):
     *  - if x<0, erf(x) = -erf(-x)
     *  - if x<7, erf(x) = sum((-1)^n·x^(2n+1)/((2n+1)·n!))/sqrt(π/4)
     *  - if x≥7, erf(x) = 1+exp(-x²)/(x·sqrt(π))·sum((-1)^n·(2n-1)!!/(2x²)^n
     *
     * FIXME: do not compute factorials at each iteration, accumulate
     * them instead (see fast_exp_sub).
     * FIXME: For a potentially faster implementation, see “Expanding the
     * Error Function erf(z)” at:
     *  http://www.mathematica-journal.com/2014/11/on-burmanns-theorem-and-its-application-to-problems-of-linear-and-nonlinear-heat-transfer-and-diffusion/#more-39602/
     */
    if (x.is_negative())
        return -erf(-x);

    real sum = real::R_0();
    real x2 = x * x;

    /* FIXME: this test is inefficient; the series converges slowly for x≥1 */
    if (x < real(7))
    {
        real xn = x, xmul = x2;
        for (int n = 0;; ++n, xn *= xmul)
        {
            real tmp = xn / (fast_fact(n) * (2 * n + 1));
            real newsum = (n & 1) ? sum - tmp : sum + tmp;
            if (newsum == sum)
                break;
            sum = newsum;
        }
        return sum * real::R_2_SQRTPI();
    }
    else
    {
        real xn = real::R_1(), xmul = inverse(x2 + x2);
        /* FIXME: this does not converge well! We need to stop at 30
         * iterations and sacrifice some accuracy. */
        for (int n = 0; n < 30; ++n, xn *= xmul)
        {
            real tmp = xn * fast_fact(n * 2 - 1, 2);
            real newsum = (n & 1) ? sum - tmp : sum + tmp;
            if (newsum == sum)
                break;
            sum = newsum;
        }

        return real::R_1() - exp(-x2) / (x * sqrt(real::R_PI())) * sum;
    }
}

template<> inline real sinh(real const &x)
{
    /* We cannot always use (exp(x)-exp(-x))/2 because we'll lose
     * accuracy near zero. We only use this identity for |x|>0.5. If
     * |x|<=0.5, we compute exp(x)-1 and exp(-x)-1 instead. */
    bool near_zero = (fabs(x) < real::R_1() / 2);
    real x1 = near_zero ? fast_exp_sub(x, real::R_1()) : exp(x);
    real x2 = near_zero ? fast_exp_sub(-x, real::R_1()) : exp(-x);
    return (x1 - x2) / 2;
}

template<> inline real tanh(real const &x)
{
    /* See sinh() for the strategy here */
    bool near_zero = (fabs(x) < real::R_1() / 2);
    real x1 = near_zero ? fast_exp_sub(x, real::R_1()) : exp(x);
    real x2 = near_zero ? fast_exp_sub(-x, real::R_1()) : exp(-x);
    real x3 = near_zero ? x1 + x2 + real::R_2() : x1 + x2;
    return (x1 - x2) / x3;
}

template<> inline real cosh(real const &x)
{
    /* No need to worry about accuracy here; maybe the last bit is slightly
     * off, but that's about it. */
    return (exp(x) + exp(-x)) / 2;
}

template<> inline real frexp(real const &x, real::exponent_t *exp)
{
    if (!x)
    {
        *exp = 0;
        return x;
    }

    /* FIXME: check that this works */
    *exp = x.m_exponent;

    real ret = x;
    ret.m_exponent = 0;
    return ret;
}

template<> inline real ldexp(real const &x, real::exponent_t exp)
{
    real ret = x;
    if (ret) /* Only do something if non-zero */
        ret.m_exponent += exp;
    return ret;
}

template<> inline real modf(real const &x, real *iptr)
{
    real absx = fabs(x);
    real tmp = floor(absx);

    *iptr = copysign(tmp, x);
    return copysign(absx - tmp, x);
}

template<> inline real nextafter(real const &x, real const &y)
{
    /* Linux manpage: “If x equals y, the functions return y.” */
    if (x == y)
        return y;

    /* Ensure x is positive. */
    if (x.is_negative())
        return -nextafter(-x, -y);

    /* FIXME: broken for now */
    real ulp = ldexp(x, -x.total_bits());
    return x < y ? x + ulp : x - ulp;
}

template<> inline real copysign(real const &x, real const &y)
{
    real ret = x;
    ret.m_sign = y.m_sign;
    return ret;
}

template<> inline real floor(real const &x)
{
    /* Strategy for floor(x):
     *  - if negative, return -ceil(-x)
     *  - if zero or negative zero, return x
     *  - if less than one, return zero
     *  - otherwise, if e is the exponent, clear all bits except the
     *    first e. */
    if (x < -real::R_0())
        return -ceil(-x);
    if (!x)
        return x;
    if (x < real::R_1())
        return real::R_0();

    real ret = x;
    real::exponent_t exponent = x.m_exponent;

    for (int i = 0; i < x.bigit_count(); ++i)
    {
        if (exponent <= 0)
            ret.m_mantissa[i] = 0;
        else if (exponent < real::bigit_bits())
            ret.m_mantissa[i] &= ~((1 << (real::bigit_bits() - exponent)) - 1);

        exponent -= real::bigit_bits();
    }

    return ret;
}

template<> inline real ceil(real const &x)
{
    /* Strategy for ceil(x):
     *  - if negative, return -floor(-x)
     *  - if x == floor(x), return x
     *  - otherwise, return floor(x) + 1 */
    if (x < -real::R_0())
        return -floor(-x);
    real ret = floor(x);
    if (ret < x)
        ret += real::R_1();
    return ret;
}

template<> inline real round(real const &x)
{
    if (x < real::R_0())
        return -round(-x);

    return floor(x + (real::R_1() / 2));
}

template<> inline real fmod(real const &x, real const &y)
{
    if (!y)
        return real::R_0(); /* FIXME: return NaN */

    if (!x)
        return x;

    real tmp = round(x / y);
    return x - tmp * y;
}

template<> inline real sin(real const &x)
{
    bool switch_sign = x.is_negative();

    real absx = fmod(fabs(x), real::R_PI() * 2);
    if (absx > real::R_PI())
    {
        absx -= real::R_PI();
        switch_sign = !switch_sign;
    }

    if (absx > real::R_PI_2())
        absx = real::R_PI() - absx;

    real ret = real::R_0(), fact = real::R_1(), xn = absx, mx2 = -absx * absx;
    int i = 1;
    for (;;)
    {
        real newret = ret + xn;
        if (newret == ret)
            break;
        ret = newret * ((i + 1) * (i + 2));
        xn *= mx2;
        i += 2;
    }
    ret /= fast_fact(i);

    /* Propagate sign */
    ret.m_sign ^= switch_sign;
    return ret;
}

template<> inline real cos(real const &x)
{
    return sin(real::R_PI_2() - x);
}

template<> inline real tan(real const &x)
{
    /* Constrain input to [-π,π] */
    real y = fmod(x, real::R_PI());

    /* Constrain input to [-π/2,π/2] */
    if (y < -real::R_PI_2())
        y += real::R_PI();
    else if (y > real::R_PI_2())
        y -= real::R_PI();

    /* In [-π/4,π/4] return sin/cos */
    if (fabs(y) <= real::R_PI_4())
        return sin(y) / cos(y);

    /* Otherwise, return cos/sin */
    if (y > real::R_0())
        y = real::R_PI_2() - y;
    else
        y = -real::R_PI_2() - y;

    return cos(y) / sin(y);
}

static inline real asinacos(real const &x, int is_asin)
{
    /* Strategy for asin(): in [-0.5..0.5], use a Taylor series around
     * zero. In [0.5..1], use asin(x) = π/2 - 2*asin(sqrt((1-x)/2)), and
     * in [-1..-0.5] just revert the sign.
     * Strategy for acos(): use acos(x) = π/2 - asin(x) and try not to
     * lose the precision around x=1. */
    real absx = fabs(x);
    int around_zero = (absx < (real::R_1() / 2));

    if (!around_zero)
        absx = sqrt((real::R_1() - absx) / 2);

    real ret = absx, xn = absx, x2 = absx * absx, fact1 = 2, fact2 = 1;
    for (int i = 1; ; ++i)
    {
        xn *= x2;
        real mul = (real)(2 * i + 1);
        real newret = ret + ldexp(fact1 * xn / (mul * fact2), -2 * i);
        if (newret == ret)
            break;
        ret = newret;
        fact1 *= (real)((2 * i + 1) * (2 * i + 2));
        fact2 *= (real)((i + 1) * (i + 1));
    }

    if (x.is_negative())
        ret = -ret;

    if (around_zero)
        ret = is_asin ? ret : real::R_PI_2() - ret;
    else
    {
        real adjust = x.is_negative() ? real::R_PI() : real::R_0();
        if (is_asin)
            ret = real::R_PI_2() - adjust - ret * 2;
        else
            ret = adjust + ret * 2;
    }

    return ret;
}

template<> inline real asin(real const &x)
{
    return asinacos(x, 1);
}

template<> inline real acos(real const &x)
{
    return asinacos(x, 0);
}

template<> inline real atan(real const &x)
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

    if (absx < (real::R_1() / 2))
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

    if (absx < (real::R_3() / 2))
    {
        real y = real::R_1() - absx;
        real yn = y, my2 = -y * y;
        for (int i = 0; ; i += 2)
        {
            real newret = ret + ldexp(yn / (real)(2 * i + 1), -i - 1);
            yn *= y;
            newret += ldexp(yn / (real)(2 * i + 2), -i - 1);
            yn *= y;
            newret += ldexp(yn / (real)(2 * i + 3), -i - 2);
            if (newret == ret)
                break;
            ret = newret;
            yn *= my2;
        }
        ret = real::R_PI_4() - ret;
    }
    else if (absx < real::R_2())
    {
        real y = (absx - real::R_SQRT3()) / 2;
        real yn = y, my2 = -y * y;
        for (int i = 1; ; i += 6)
        {
            real newret = ret + ((yn / (real)i) / 2);
            yn *= y;
            newret -= (real::R_SQRT3() / 2) * yn / (real)(i + 1);
            yn *= y;
            newret += yn / (real)(i + 2);
            yn *= y;
            newret -= (real::R_SQRT3() / 2) * yn / (real)(i + 3);
            yn *= y;
            newret += (yn / (real)(i + 4)) / 2;
            if (newret == ret)
                break;
            ret = newret;
            yn *= my2;
        }
        ret = real::R_PI_3() + ret;
    }
    else
    {
        real y = inverse(absx);
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
        ret = real::R_PI_2() - ret;
    }

    /* Propagate sign */
    ret.m_sign = x.m_sign;
    return ret;
}

template<> inline real atan2(real const &y, real const &x)
{
    if (!y)
    {
        if (!x.is_negative())
            return y;
        return y.is_negative() ? -real::R_PI() : real::R_PI();
    }

    if (!x)
    {
        return y.is_negative() ? -real::R_PI() : real::R_PI();
    }

    /* FIXME: handle the Inf and NaN cases */
    real z = y / x;
    real ret = atan(z);
    if (x < real::R_0())
        ret += (y > real::R_0()) ? real::R_PI() : -real::R_PI();
    return ret;
}

/* Franke’s function, used as a test for interpolation methods */
template<> inline real franke(real const &x, real const &y)
{
    /* Compute 9x and 9y */
    real nx = x + x; nx += nx; nx += nx + x;
    real ny = y + y; ny += ny; ny += ny + y;

    /* Temporary variables for the formula */
    real a = nx - real::R_2();
    real b = ny - real::R_2();
    real c = nx + real::R_1();
    real d = ny + real::R_1();
    real e = nx - real(7);
    real f = ny - real::R_3();
    real g = nx - real(4);
    real h = ny - real(7);

    return exp(-(a * a + b * b) * real(0.25)) * real(0.75)
         + exp(-(c * c / real(49) + d * d / real::R_10())) * real(0.75)
         + exp(-(e * e + f * f) * real(0.25)) * real(0.5)
         - exp(-(g * g + h * h)) / real(5);
}

/* The Peaks example function from Matlab */
template<> inline real peaks(real const &x, real const &y)
{
    real x2 = x * x;
    real y2 = y * y;
    /* 3 * (1-x)^2 * exp(-x^2 - (y+1)^2) */
    real ret = real::R_3()
             * (x2 - x - x + real::R_1())
             * exp(- x2 - y2 - y - y - real::R_1());
    /* -10 * (x/5 - x^3 - y^5) * exp(-x^2 - y^2) */
    ret -= (x + x - real::R_10() * (x2 * x + y2 * y2 * y)) * exp(-x2 - y2);
    /* -1/3 * exp(-(x+1)^2 - y^2) */
    ret -= exp(-x2 - x - x - real::R_1() - y2) / real::R_3();
    return ret;
}

template<> inline
std::ostream& operator <<(std::ostream &s, real const &x)
{
    bool hex = (s.flags() & std::ios_base::basefield) == std::ios_base::hex;
    s << (hex ? x.xstr() : x.str((int)s.precision()));
    return s;
}

template<> inline std::string real::str(int ndigits) const
{
    std::stringstream ss;
    real x = *this;

    if (x.is_negative())
    {
        ss << '-';
        x = -x;
    }

    if (!x)
    {
        ss << '0';
        return ss.str();
    }

    // Normalise x so that mantissa is in [1..9.999]
    // FIXME: better use int64_t when the cast is implemented
    // FIXME: does not work with R_MAX and probably R_MIN
    int exponent = ceil(log10(x));
    x *= pow(R_10(), -(real)exponent);

    if (ndigits < 1)
        ndigits = 1;

    // Add a bias to simulate some naive rounding
    x += real(4.99f) * pow(R_10(), -(real)(ndigits + 1));

    if (x < R_1())
    {
        x *= R_10();
        exponent--;
    }

    // Print digits
    for (int i = 0; i < ndigits; ++i)
    {
        int digit = (int)floor(x);
        ss << (char)('0' + digit);
        if (i == 0)
            ss << '.';
        x -= real(digit);
        x *= R_10();
    }

    // Remove trailing zeroes
    std::string ret = ss.str();
    ss.str("");
    size_t end = ret.find_last_not_of('0');
    if (end != std::string::npos)
        ss << ret.substr(0, end + 1);

    // Print exponent information
    if (exponent)
        ss << 'e' << (exponent >= 0 ? '+' : '-') << std::abs(exponent);

    return ss.str();
}

template<> inline std::string real::xstr() const
{
    std::stringstream ss;
    if (is_negative())
        ss << '-';
    ss << "0x1." << std::hex << std::setfill('0');
    for (int i = 0; i < bigit_count(); ++i)
        ss << std::setw(8) << m_mantissa[i];
    ss << std::dec;

    // Remove trailing zeroes
    std::string ret = ss.str();
    ss.str("");
    size_t end = ret.find_last_not_of('0');
    if (end != std::string::npos)
        ss << ret.substr(0, end + 1);

    ss << 'p' << m_exponent;

    return ss.str();
}

static real load_min()
{
    real ret = 1;
    return ldexp(ret, std::numeric_limits<real::exponent_t>::min());
}

static real load_max()
{
    /* FIXME: the last bits of the mantissa are not properly handled in this
     * code! So we fallback to a slow but exact method. */
#if 0
    real ret = 1;
    ret = ldexp(ret, real::TOTAL_BITS - 1) - ret;
    return ldexp(ret, real::EXPONENT_BIAS + 2 - real::TOTAL_BITS);
#endif
    /* Generates 0x1.ffff..ffffp18446744073709551615 */
    char str[160];
    std::sprintf(str, "0x1.%llx%llx%llx%llx%llx%llx%llx%llxp%lld",
                 -1ll, -1ll, -1ll, -1ll, -1ll, -1ll, -1ll, -1ll,
                 (long long int)std::numeric_limits<int64_t>::max());
    return real(str);
}

static real load_pi()
{
    /* Approximate π using Machin’s formula: 16*atan(1/5)-4*atan(1/239) */
    real ret = 0, x0 = 5, x1 = 239;
    real const m0 = -x0 * x0, m1 = -x1 * x1, r16 = 16, r4 = 4;

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

} /* namespace lol */

