//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Half class
// --------------
//

#include <lol/base/types.h>

#include <cmath>
#include <cstdio>
#include <stdint.h>

namespace lol
{

/* This is OUR namespace. Don't let Windows headers mess with it. */
#undef min
#undef max

namespace half_ops { struct base {}; }

class [[nodiscard]] half
  : half_ops::base
{
public:
    /* Constructors. Always inline so that the code can work in registers
     * instead of calling routines with the hidden "this" parameter. */
    inline half() { }
    inline half(int f) { *this = makefast((float)f); }
    inline half(float f) { *this = makefast(f); }
    inline half(double f) { *this = makefast((float)f); }
    inline half(ldouble f) { *this = makefast((float)f); }

    [[nodiscard]] inline int is_nan() const
    {
        return ((bits & 0x7c00u) == 0x7c00u) && (bits & 0x03ffu);
    }

    [[nodiscard]] inline int is_finite() const
    {
        return (bits & 0x7c00u) != 0x7c00u;
    }

    [[nodiscard]] inline int is_inf() const
    {
        return (uint16_t)(bits << 1) == (0x7c00u << 1);
    }

    [[nodiscard]] inline int is_normal() const
    {
        return (is_finite() && (bits & 0x7c00u)) || ((bits & 0x7fffu) == 0);
    }

    /* Cast to other types -- always inline, see constructors */
    inline half &operator =(int f) { return *this = makefast((float)f); }
    inline half &operator =(float f) { return *this = makefast(f); }
    inline half &operator =(double f) { return *this = makefast((float)f); }
    inline half &operator =(ldouble f) { return *this = makefast((float)f); }
    [[nodiscard]] inline operator int8_t() const { return (int8_t)(float)*this; }
    [[nodiscard]] inline operator uint8_t() const { return (uint8_t)(float)*this; }
    [[nodiscard]] inline operator int16_t() const { return (int16_t)(float)*this; }
    [[nodiscard]] inline operator uint16_t() const { return (uint16_t)(float)*this; }
    [[nodiscard]] inline operator int32_t() const { return (int32_t)(float)*this; }
    [[nodiscard]] inline operator uint32_t() const { return (uint32_t)(float)*this; }
    [[nodiscard]] inline operator int64_t() const { return (int64_t)(float)*this; }
    [[nodiscard]] inline operator uint64_t() const { return (uint64_t)(float)*this; }

    [[nodiscard]] operator float() const;
    [[nodiscard]] inline operator double() const { return (float)(*this); }
    [[nodiscard]] inline operator ldouble() const { return (float)(*this); }

    /* Array conversions */
    static void convert(half *dst, float const *src, size_t nelem);
    static void convert(float *dst, half const *src, size_t nelem);

    /* Operations */
    [[nodiscard]] bool operator ==(half x) const { return (float)*this == (float)x; }
    [[nodiscard]] bool operator !=(half x) const { return (float)*this != (float)x; }
    [[nodiscard]] bool operator <(half x) const { return (float)*this < (float)x; }
    [[nodiscard]] bool operator >(half x) const { return (float)*this > (float)x; }
    [[nodiscard]] bool operator <=(half x) const { return (float)*this <= (float)x; }
    [[nodiscard]] bool operator >=(half x) const { return (float)*this >= (float)x; }

    [[nodiscard]] bool operator !() const { return !(bits & 0x7fffu); }
    [[nodiscard]] operator bool() const { return !!*this; }

    inline half operator -() const { return makebits(bits ^ 0x8000u); }
    inline half operator +() const { return *this; }
    inline half &operator +=(half h) { return (*this = (half)(*this + h)); }
    inline half &operator -=(half h) { return (*this = (half)(*this - h)); }
    inline half &operator *=(half h) { return (*this = (half)(*this * h)); }
    inline half &operator /=(half h) { return (*this = (half)(*this / h)); }

    [[nodiscard]] inline float operator +(half h) const { return (float)*this + (float)h; }
    [[nodiscard]] inline float operator -(half h) const { return (float)*this - (float)h; }
    [[nodiscard]] inline float operator *(half h) const { return (float)*this * (float)h; }
    [[nodiscard]] inline float operator /(half h) const { return (float)*this / (float)h; }

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

static_assert(sizeof(half) == 2, "sizeof(half) == 2");

/*
 * Standard math and GLSL functions
 */

static inline half min(half a, half b) { return a < b ? a : b; }
static inline half max(half a, half b) { return a > b ? a : b; }
static inline float fmod(half a, half b)
{
    using std::fmod;
    return fmod((float)a, (float)b);
}
static inline float fract(half a) { return fract((float)a); }
static inline float degrees(half a) { return degrees((float)a); }
static inline float radians(half a) { return radians((float)a); }
static inline half abs(half a) { return half::makebits(a.bits & 0x7fffu); }

static inline half clamp(half x, half a, half b)
{
    return (x < a) ? a : (x > b) ? b : x;
}

/*
 * Standard math operators
 */

namespace half_ops
{
    /* Enumerate the types for which operations with half are valid */
    template<typename FROM, typename TO = void> struct valid {};

    template<typename TO> struct valid<uint8_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<int8_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<uint16_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<int16_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<uint32_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<int32_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<uint64_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<int64_t, TO>
        { typedef half from; typedef TO to; };

    template<typename TO> struct valid<float, TO>
        { typedef float from; typedef TO to; };
    template<typename TO> struct valid<double, TO>
        { typedef double from; typedef TO to; };
    template<typename TO> struct valid<ldouble, TO>
        { typedef ldouble from; typedef TO to; };

#define DECLARE_HALF_NUMERIC_OPS(op) \
    /* other + half */ \
    template<typename T> \
    static inline typename valid<T>::from operator op(T x, half h) \
    { return (typename valid<T>::from)x op (typename valid<T>::from)h; } \
    \
    /* half + other */ \
    template<typename T> \
    static inline typename valid<T>::from operator op(half h, T x) \
    { return (typename valid<T>::from)h op (typename valid<T>::from)x; } \
    \
    /* other += half */ \
    template<typename T> \
    static inline typename valid<T,T>::to& operator op##=(T& x, half h) \
    { return x = (typename valid<T>::from)x op (typename valid<T>::from)h; } \
    \
    /* half += other */ \
    template<typename T> \
    static inline typename valid<T,half>::to& operator op##=(half& h, T x) \
    { return h = (typename valid<T>::from)h op (typename valid<T>::from)x; }

DECLARE_HALF_NUMERIC_OPS(+)
DECLARE_HALF_NUMERIC_OPS(-)
DECLARE_HALF_NUMERIC_OPS(*)
DECLARE_HALF_NUMERIC_OPS(/)

#undef DECLARE_HALF_NUMERIC_OPS

#define DECLARE_HALF_BOOL_OPS(op) \
    /* half == other */ \
    template<typename T> \
    static inline typename valid<T,bool>::to operator op(half h, T x) \
    { return (typename valid<T>::from)h op (typename valid<T>::from)x; } \
    \
    /* other == half */ \
    template<typename T> \
    static inline typename valid<T,bool>::to operator op(T x, half h) \
    { return (typename valid<T>::from)x op (typename valid<T>::from)h; }

DECLARE_HALF_BOOL_OPS(==)
DECLARE_HALF_BOOL_OPS(!=)
DECLARE_HALF_BOOL_OPS(>)
DECLARE_HALF_BOOL_OPS(<)
DECLARE_HALF_BOOL_OPS(>=)
DECLARE_HALF_BOOL_OPS(<=)

#undef DECLARE_HALF_BOOL_OPS

} /* namespace half_ops */

} /* namespace lol */

