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

#pragma once

//
// Various maths functions
// -----------------------
//

#include <lol/math/constants.h>

#include <cmath>
#include <cstdio>
#include <algorithm>
#include <type_traits>

#include <stdint.h>

namespace lol
{

// This is OUR namespace. Don't let Windows headers mess with it.
#undef min
#undef max

// Macros for type traits
#define LOL_T_ARITHMETIC typename std::enable_if<std::is_arithmetic<T>::value, T>::type
#define LOL_T_SIGNED typename std::enable_if<std::is_signed<T>::value, T>::type
#define LOL_T_UNSIGNED typename std::enable_if<std::is_arithmetic<T>::value && \
                                               !std::is_signed<T>::value, T>::type
#define LOL_T_INTEGRAL typename std::enable_if<std::is_integral<T>::value, T>::type
#define LOL_T_FLOATING_POINT typename std::enable_if<std::is_floating_point<T>::value, T>::type

// Mechanism to import standard cmath functions
#define LOL_FORWARD_FP_1_ARG(f) \
    template<typename T, typename T2 = T, typename DUMMY = LOL_T_FLOATING_POINT> \
    lol_attr_nodiscard static inline T2 f(T x) { return std::f(x); }

#define LOL_FORWARD_ARITH_2_ARGS(f) \
    template<typename T, typename T2 = T, typename DUMMY = LOL_T_ARITHMETIC> \
    lol_attr_nodiscard static inline T2 f(T x, T y) { return std::f(x, y); }

#define LOL_FORWARD_FP_2_ARGS(f) \
    template<typename T, typename T2 = T, typename DUMMY = LOL_T_FLOATING_POINT> \
    lol_attr_nodiscard static inline T2 f(T x, T y) { return std::f(x, y); }

LOL_FORWARD_FP_1_ARG(sqrt)
LOL_FORWARD_FP_1_ARG(cbrt)

LOL_FORWARD_FP_1_ARG(exp)
LOL_FORWARD_FP_2_ARGS(pow)

LOL_FORWARD_FP_1_ARG(sin)
LOL_FORWARD_FP_1_ARG(cos)
LOL_FORWARD_FP_1_ARG(tan)
LOL_FORWARD_FP_1_ARG(asin)
LOL_FORWARD_FP_1_ARG(acos)
LOL_FORWARD_FP_1_ARG(atan)
LOL_FORWARD_FP_2_ARGS(atan2)

LOL_FORWARD_ARITH_2_ARGS(min)
LOL_FORWARD_ARITH_2_ARGS(max)

LOL_FORWARD_FP_2_ARGS(fmod)
LOL_FORWARD_FP_1_ARG(floor)
LOL_FORWARD_FP_1_ARG(ceil)
LOL_FORWARD_FP_1_ARG(round)

// Our extensions
template<typename T, typename T2 = LOL_T_FLOATING_POINT>
lol_attr_nodiscard static inline T2 sincos(T x, T *s, T *c)
{
    *s = std::sin(x);
    *c = std::cos(x);
}

// Inherited from GLSL
lol_attr_nodiscard static inline float degrees(float radians)
{
    return radians * (180.0f / F_PI);
}

lol_attr_nodiscard static inline double degrees(double radians)
{
    return radians * (180.0 / D_PI);
}

lol_attr_nodiscard static inline ldouble degrees(ldouble radians)
{
    return radians * (180.0L / LD_PI);
}

lol_attr_nodiscard static inline float radians(float degrees)
{
    return degrees * (F_PI / 180.0f);
}

lol_attr_nodiscard static inline double radians(double degrees)
{
    return degrees * (D_PI / 180.0);
}

lol_attr_nodiscard static inline ldouble radians(ldouble degrees)
{
    return degrees * (LD_PI / 180.0L);
}

// The integer versions return floating point values. This avoids nasty
// surprises when calling radians(180) instead of radians(180.0).
lol_attr_nodiscard static inline float   degrees(int8_t x)   { return degrees(float(x)); }
lol_attr_nodiscard static inline float   degrees(uint8_t x)  { return degrees(float(x)); }
lol_attr_nodiscard static inline float   degrees(int16_t x)  { return degrees(float(x)); }
lol_attr_nodiscard static inline float   degrees(uint16_t x) { return degrees(float(x)); }
lol_attr_nodiscard static inline double  degrees(int32_t x)  { return degrees(double(x)); }
lol_attr_nodiscard static inline double  degrees(uint32_t x) { return degrees(double(x)); }
lol_attr_nodiscard static inline ldouble degrees(int64_t x)  { return degrees(ldouble(x)); }
lol_attr_nodiscard static inline ldouble degrees(uint64_t x) { return degrees(ldouble(x)); }

lol_attr_nodiscard static inline float   radians(int8_t x)   { return radians(float(x)); }
lol_attr_nodiscard static inline float   radians(uint8_t x)  { return radians(float(x)); }
lol_attr_nodiscard static inline float   radians(int16_t x)  { return radians(float(x)); }
lol_attr_nodiscard static inline float   radians(uint16_t x) { return radians(float(x)); }
lol_attr_nodiscard static inline double  radians(int32_t x)  { return radians(double(x)); }
lol_attr_nodiscard static inline double  radians(uint32_t x) { return radians(double(x)); }
lol_attr_nodiscard static inline ldouble radians(int64_t x)  { return radians(ldouble(x)); }
lol_attr_nodiscard static inline ldouble radians(uint64_t x) { return radians(ldouble(x)); }

template<typename T, typename T2 = LOL_T_FLOATING_POINT>
lol_attr_nodiscard static inline T2 mix(T a, T b, T x)
{
    return a + (b - a) * x;
}

// Inherited from HLSL
template<typename T, typename T2 = LOL_T_FLOATING_POINT>
lol_attr_nodiscard static inline T2 lerp(T a, T b, T x)
{
    return mix(a, b, x);
}

// C++ doesn't define abs() or fmod() for all types; we add these for
// convenience to avoid adding complexity to vector.h.
template<typename T, typename T2 = LOL_T_SIGNED>
lol_attr_nodiscard static inline T2 abs(T x) { return std::abs(x); }
template<typename T, typename T2 = T, typename DUMMY = LOL_T_UNSIGNED>
lol_attr_nodiscard static inline T2 abs(T x) { return x; }

template<typename T, typename T2 = LOL_T_INTEGRAL>
lol_attr_nodiscard static inline T2 fmod(T x, T y) { return x % y; }
template<typename T, typename T2 = LOL_T_INTEGRAL>
lol_attr_nodiscard static inline T2 floor(T x) { return x; }
template<typename T, typename T2 = LOL_T_INTEGRAL>
lol_attr_nodiscard static inline T2 ceil(T x) { return x; }
template<typename T, typename T2 = LOL_T_INTEGRAL>
lol_attr_nodiscard static inline T2 round(T x) { return x; }

template<typename T, typename T2 = LOL_T_ARITHMETIC>
lol_attr_nodiscard static inline T2 sq(T x) { return x * x; }
template<typename T, typename T2 = LOL_T_ARITHMETIC>
lol_attr_nodiscard static inline T2 fract(T x) { return x - lol::floor(x); }

template<typename T, typename T2 = LOL_T_ARITHMETIC>
lol_attr_nodiscard static inline T2 clamp(T x, T y, T z) { return min(max(x, y), z); }
template<typename T, typename T2 = LOL_T_ARITHMETIC>
lol_attr_nodiscard static inline T2 saturate(T x) { return clamp(x, (T)0, (T)1); }
template<typename T, typename T2 = LOL_T_ARITHMETIC>
lol_attr_nodiscard static inline T2 gcd(T x, T y) { return y == (T)0 ? lol::abs(x) : lol::gcd(y, lol::fmod(x, y)); }

template<typename T, typename T2 = LOL_T_SIGNED>
lol_attr_nodiscard static inline T2 sign(T x) { return (T)(((T)0 < x) - (x < (T)0)); }
template<typename T, typename T2 = T, typename DUMMY = LOL_T_UNSIGNED>
lol_attr_nodiscard static inline T2 sign(T x) { return (T)((T)0 < x); }

} /* namespace lol */

