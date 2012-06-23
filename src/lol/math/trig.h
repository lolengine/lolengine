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
// Trigonometry functions
// ----------------------
//

#if !defined __LOL_MATH_TRIG_H__
#define __LOL_MATH_TRIG_H__

#include <cstdio>
#include <stdint.h>

namespace lol
{

/* Standard cmath functions */
static inline double sin(double const &x) { return std::sin(x); }
static inline double cos(double const &x) { return std::cos(x); }
static inline double tan(double const &x) { return std::tan(x); }
static inline double asin(double const &x) { return std::asin(x); }
static inline double acos(double const &x) { return std::acos(x); }
static inline double atan(double const &x) { return std::atan(x); }

static inline float sin(float const &x) { return std::sin(x); }
static inline float cos(float const &x) { return std::cos(x); }
static inline float tan(float const &x) { return std::tan(x); }
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

} /* namespace lol */

#endif // __LOL_MATH_TRIG_H__

