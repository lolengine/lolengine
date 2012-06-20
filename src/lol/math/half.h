//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Half class
// --------------
//

#if !defined __LOL_HALF_H__
#define __LOL_HALF_H__

#include <cstdio>
#include <stdint.h>

namespace lol
{

/* This is OUR namespace. Don't let Windows headers fuck with it. */
#undef min
#undef max

class half
{
public:
    /* Constructors. Always inline so that the code can work in registers
     * instead of calling routines with the hidden "this" parameter. */
    inline half() { }
    inline half(int f) { *this = makefast((float)f); }
    inline half(float f) { *this = makefast(f); }
    inline half(double f) { *this = makefast((float)f); }
    inline half(long double f) { *this = makefast((float)f); }

    inline int is_nan() const
    {
        return ((bits & 0x7c00u) == 0x7c00u) && (bits & 0x03ffu);
    }

    inline int is_finite() const
    {
        return (bits & 0x7c00u) != 0x7c00u;
    }

    inline int is_inf() const
    {
        return (uint16_t)(bits << 1) == (0x7c00u << 1);
    }

    inline int is_normal() const
    {
        return (is_finite() && (bits & 0x7c00u)) || ((bits & 0x7fffu) == 0);
    }

    /* Cast to other types -- always inline, see constructors */
    inline half &operator =(int f) { return *this = makefast((float)f); }
    inline half &operator =(float f) { return *this = makefast(f); }
    inline half &operator =(double f) { return *this = makefast((float)f); }
    inline half &operator =(long double f) { return *this = makefast((float)f); }
    inline operator int8_t() const { return (int8_t)(float)*this; }
    inline operator uint8_t() const { return (uint8_t)(float)*this; }
    inline operator int16_t() const { return (int16_t)(float)*this; }
    inline operator uint16_t() const { return (uint16_t)(float)*this; }
    inline operator int32_t() const { return (int32_t)(float)*this; }
    inline operator uint32_t() const { return (uint32_t)(float)*this; }
    inline operator int64_t() const { return (int64_t)(float)*this; }
    inline operator uint64_t() const { return (uint64_t)(float)*this; }

    inline operator float() const { return tofloat(*this); }
    inline operator double() const { return tofloat(*this); }
    inline operator long double() const { return tofloat(*this); }

    static float tofloat(half h);

    /* Array conversions */
    static size_t convert(half *dst, float const *src, size_t nelem);
    static size_t convert(float *dst, half const *src, size_t nelem);

    /* Operations */
    bool operator ==(half x) const { return (float)*this == (float)x; }
    bool operator !=(half x) const { return (float)*this != (float)x; }
    bool operator <(half x) const { return (float)*this < (float)x; }
    bool operator >(half x) const { return (float)*this > (float)x; }
    bool operator <=(half x) const { return (float)*this <= (float)x; }
    bool operator >=(half x) const { return (float)*this >= (float)x; }

    bool operator !() const { return !(bits & 0x7fffu); }
    operator bool() const { return !!*this; }

    inline half operator -() const { return makebits(bits ^ 0x8000u); }
    inline half operator +() const { return *this; }
    inline half &operator +=(half h) { return (*this = (half)(*this + h)); }
    inline half &operator -=(half h) { return (*this = (half)(*this - h)); }
    inline half &operator *=(half h) { return (*this = (half)(*this * h)); }
    inline half &operator /=(half h) { return (*this = (half)(*this / h)); }

    inline float operator +(half h) const { return (float)*this + (float)h; }
    inline float operator -(half h) const { return (float)*this - (float)h; }
    inline float operator *(half h) const { return (float)*this * (float)h; }
    inline float operator /(half h) const { return (float)*this / (float)h; }

    /* Factories */
    static half makefast(float f);
    static half makeaccurate(float f);
    static inline half makebits(uint16_t x)
    {
        half ret;
        ret.bits = x;
        return ret;
    }

    /* Internal representation */
    uint16_t bits;
};

static inline half min(half a, half b) { return a < b ? a : b; }
static inline half max(half a, half b) { return a > b ? a : b; }

static inline half clamp(half x, half a, half b)
{
    return (x < a) ? a : (x > b) ? b : x;
}


#define DECLARE_COERCE_HALF_NUMERIC_OPS(op, type, ret, x2, h2) \
    inline ret operator op(type x, half h) { return x2 op h2; } \
    inline ret operator op(half h, type x) { return h2 op x2; } \
    inline type &operator op##=(type &x, half h) { return x = x op h2; } \
    inline half &operator op##=(half &h, type x) { return h = h op x2; }

#define DECLARE_COERCE_HALF_BOOL_OPS(op, type, x2, h2) \
    inline bool operator op(type x, half h) { return x2 op h2; } \
    inline bool operator op(half h, type x) { return h2 op x2; }

#define DECLARE_COERCE_HALF_OPS(type, ret, x2, h2) \
    DECLARE_COERCE_HALF_NUMERIC_OPS(+, type, ret, x2, h2) \
    DECLARE_COERCE_HALF_NUMERIC_OPS(-, type, ret, x2, h2) \
    DECLARE_COERCE_HALF_NUMERIC_OPS(*, type, ret, x2, h2) \
    DECLARE_COERCE_HALF_NUMERIC_OPS(/, type, ret, x2, h2) \
    \
    DECLARE_COERCE_HALF_BOOL_OPS(==, type, x2, h2) \
    DECLARE_COERCE_HALF_BOOL_OPS(!=, type, x2, h2) \
    DECLARE_COERCE_HALF_BOOL_OPS(>=, type, x2, h2) \
    DECLARE_COERCE_HALF_BOOL_OPS(<=, type, x2, h2) \
    DECLARE_COERCE_HALF_BOOL_OPS(>, type, x2, h2) \
    DECLARE_COERCE_HALF_BOOL_OPS(<, type, x2, h2)

#define DECLARE_COERCE_TO_HALF_OPS(type) \
    DECLARE_COERCE_HALF_OPS(type, half, (half)(int)x, h)

#define DECLARE_COERCE_FROM_HALF_OPS(type) \
    DECLARE_COERCE_HALF_OPS(type, type, x, (type)h)

/* Only provide coercion rules above int32_t, since the standard says
 * all smaller base types are coerced to int. */
DECLARE_COERCE_TO_HALF_OPS(int32_t)
DECLARE_COERCE_TO_HALF_OPS(uint32_t)
DECLARE_COERCE_TO_HALF_OPS(int64_t)
DECLARE_COERCE_TO_HALF_OPS(uint64_t)

DECLARE_COERCE_FROM_HALF_OPS(float)
DECLARE_COERCE_FROM_HALF_OPS(double)
DECLARE_COERCE_FROM_HALF_OPS(long double)

#undef DECLARE_COERCE_HALF_NUMERIC_OPS
#undef DECLARE_COERCE_HALF_OPS
#undef DECLARE_COERCE_TO_HALF_OPS
#undef DECLARE_COERCE_FROM_HALF_OPS

} /* namespace lol */

#endif // __LOL_HALF_H__

