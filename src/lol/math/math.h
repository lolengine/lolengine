//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// Various maths functions
// -----------------------
//

#if !defined __LOL_MATH_MATH_H__
#define __LOL_MATH_MATH_H__

#include <cmath>
#include <cstdio>
#include <algorithm>

#include <stdint.h>

namespace lol
{

/* This is OUR namespace. Don't let Windows headers fuck with it. */
#undef min
#undef max

/* There are many reasons for wanting single-word type names, the most
 * important one being compilation speedups in our vector.h: we can hide
 * some global methods in namespaces that contain the name of the type,
 * but namespaces cannot have spaces in their names. */
typedef long double ldouble;

/* Standard cmath functions */
static inline double sqrt(double const &x) { return std::sqrt(x); }
static inline float sqrt(float const &x) { return std::sqrt(x); }

static inline double sin(double const &x) { return std::sin(x); }
static inline double cos(double const &x) { return std::cos(x); }
static inline double tan(double const &x) { return std::tan(x); }
static inline float sin(float const &x) { return std::sin(x); }
static inline float cos(float const &x) { return std::cos(x); }
static inline float tan(float const &x) { return std::tan(x); }

static inline double asin(double const &x) { return std::asin(x); }
static inline double acos(double const &x) { return std::acos(x); }
static inline double atan(double const &x) { return std::atan(x); }
static inline float asin(float const &x) { return std::asin(x); }
static inline float acos(float const &x) { return std::acos(x); }
static inline float atan(float const &x) { return std::atan(x); }

static inline double atan2(double const &y, double const &x)
{
    return std::atan2(y, x);
}

static inline float atan2(float const &y, float const &x)
{
    return std::atan2(y, x);
}

/* Our extensions */
static inline void sincos(double const &x, double *s, double *c)
{
    *s = std::sin(x);
    *c = std::cos(x);
}

static inline void sincos(float const &x, float *s, float *c)
{
    *s = std::sin(x);
    *c = std::cos(x);
}

static inline float lerp(float const &a, float const &b, float const &x)
{
    return a + (b - a) * x;
}

static inline double lerp(double const &a, double const &b, double const &x)
{
    return a + (b - a) * x;
}

static inline ldouble lerp(ldouble const &a, ldouble const &b, ldouble const &x)
{
    return a + (b - a) * x;
}


/* These accelerated functions will be merged into the above, one day */
double lol_sin(double);
double lol_cos(double);
double lol_tan(double);
void lol_sincos(double, double*, double*);
void lol_sincos(float, float*, float*);
double lol_asin(double);
double lol_acos(double);
double lol_atan(double);
double lol_atan2(double, double);

/* C++ doesn't define abs() and fmod() for all types; we add these for
 * convenience to avoid adding complexity to vector.h. */
static inline int8_t abs(int8_t x) { return std::abs(x); }
static inline uint8_t abs(uint8_t x) { return x; }
static inline int16_t abs(int16_t x) { return std::abs(x); }
static inline uint16_t abs(uint16_t x) { return x; }
static inline int32_t abs(int32_t x) { return std::abs(x); }
static inline uint32_t abs(uint32_t x) { return x; }
#if defined __native_client__
/* The pepper 19 toolchain doesn't provide abs() for long long int. */
static inline int64_t abs(int64_t x) { return x > 0 ? x : -x; }
#else
static inline int64_t abs(int64_t x) { return std::abs(x); }
#endif
static inline uint64_t abs(uint64_t x) { return x; }
static inline float abs(float x) { return std::abs(x); }
static inline double abs(double x) { return std::abs(x); }
static inline ldouble abs(ldouble x) { return std::abs(x); }

static inline uint8_t fmod(uint8_t x, uint8_t y) { return x % y; }
static inline int8_t fmod(int8_t x, int8_t y) { return x % y; }
static inline uint16_t fmod(uint16_t x, uint16_t y) { return x % y; }
static inline int16_t fmod(int16_t x, int16_t y) { return x % y; }
static inline uint32_t fmod(uint32_t x, uint32_t y) { return x % y; }
static inline int32_t fmod(int32_t x, int32_t y) { return x % y; }
static inline uint64_t fmod(uint64_t x, uint64_t y) { return x % y; }
static inline int64_t fmod(int64_t x, int64_t y) { return x % y; }
static inline float fmod(float x, float y) { return std::fmod(x, y); }
static inline double fmod(double x, double y) { return std::fmod(x, y); }
static inline ldouble fmod(ldouble x, ldouble y) { return std::fmod(x, y); }

static inline uint8_t min(uint8_t x, uint8_t y) { return std::min(x, y); }
static inline int8_t min(int8_t x, int8_t y) { return std::min(x, y); }
static inline uint16_t min(uint16_t x, uint16_t y) { return std::min(x, y); }
static inline int16_t min(int16_t x, int16_t y) { return std::min(x, y); }
static inline uint32_t min(uint32_t x, uint32_t y) { return std::min(x, y); }
static inline int32_t min(int32_t x, int32_t y) { return std::min(x, y); }
static inline uint64_t min(uint64_t x, uint64_t y) { return std::min(x, y); }
static inline int64_t min(int64_t x, int64_t y) { return std::min(x, y); }
static inline float min(float x, float y) { return std::min(x, y); }
static inline double min(double x, double y) { return std::min(x, y); }
static inline ldouble min(ldouble x, ldouble y) { return std::min(x, y); }

static inline uint8_t max(uint8_t x, uint8_t y) { return std::max(x, y); }
static inline int8_t max(int8_t x, int8_t y) { return std::max(x, y); }
static inline uint16_t max(uint16_t x, uint16_t y) { return std::max(x, y); }
static inline int16_t max(int16_t x, int16_t y) { return std::max(x, y); }
static inline uint32_t max(uint32_t x, uint32_t y) { return std::max(x, y); }
static inline int32_t max(int32_t x, int32_t y) { return std::max(x, y); }
static inline uint64_t max(uint64_t x, uint64_t y) { return std::max(x, y); }
static inline int64_t max(int64_t x, int64_t y) { return std::max(x, y); }
static inline float max(float x, float y) { return std::max(x, y); }
static inline double max(double x, double y) { return std::max(x, y); }
static inline ldouble max(ldouble x, ldouble y) { return std::max(x, y); }

} /* namespace lol */

#endif // __LOL_MATH_MATH_H__

