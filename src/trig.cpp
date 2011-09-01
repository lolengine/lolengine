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

#if defined HAVE_FASTMATH_H
#   include <fastmath.h>
#endif

#include "core.h"

using namespace std;

namespace lol
{

static const double PI     = 3.14159265358979323846;
static const double PI_2   = PI / 2.0;
static const double PI_4   = PI / 4.0;
static const double INV_PI = 1.0 / PI;
static const double ROOT3  = 1.732050807568877293527;

static const double ZERO    = 0.0;
static const double ONE     = 1.0;
static const double NEG_ONE = -1.0;
static const double HALF    = 0.5;
static const double TWO     = 2.0;
static const double VERY_SMALL_NUMBER = 0x1.0p-128;
static const double VERY_LARGE_NUMBER = 4503599627370496.0;

/** sin Taylor series coefficients. */
static const double SC[]    =
{
    -1.6666666666666666666667e-01, // 1/3!
    +8.3333333333333333333333e-03, // 1/5!
    -1.9841269841269841269841e-04, // 1/7!
    +2.7557319223985890652557e-06, // 1/9!
    -2.5052108385441718775052e-08, // 1/11!
    +1.6059043836821614599392e-10, // 1/13!
    -7.6471637318198164759011e-13, // 1/15!
    +2.8114572543455207631989e-15, // 1/17!
};

/* Custom intrinsics */
#define INLINEATTR __attribute__((always_inline))

#if defined __CELLOS_LV2__
static inline double lol_fctid(double x) INLINEATTR;
static inline double lol_fctidz(double x) INLINEATTR;
static inline double lol_fcfid(double x) INLINEATTR;
static inline double lol_frsqrte(double x) INLINEATTR;
#endif
static inline double lol_fsel(double c, double gte, double lt) INLINEATTR;
static inline double lol_fabs(double x) INLINEATTR;
static inline double lol_round(double x) INLINEATTR;
static inline double lol_trunc(double x) INLINEATTR;
static inline double lol_round(double x) INLINEATTR;
static inline double lol_trunc(double x) INLINEATTR;

#if defined __CELLOS_LV2__
static inline double lol_fctid(double x)
{
    double r;
#if defined __SNC__
    r = __builtin_fctid(x);
#else
    __asm__ ("fctid %0, %1"
             : "=f"(r) : "f"(x));
#endif
    return r;
}

static double lol_fctidz(double x)
{
    double r;
#if defined __SNC__
    r = __builtin_fctidz(x);
#else
    __asm__ ("fctidz %0, %1"
             : "=f"(r) : "f"(x));
#endif
    return r;
}

static double lol_fcfid(double x)
{
    double r;
#if defined __SNC__
    r = __builtin_fcfid(x);
#else
    __asm__ ("fcfid %0, %1"
             : "=f"(r) : "f"(x));
#endif
    return r;
}

static double lol_frsqrte(double x)
{
#if defined __SNC__
    return __builtin_frsqrte(x);
#else
    double r;
    __asm__ ("frsqrte %0, %1"
             : "=f"(r) : "f"(x));
    return r;
#endif
}
#endif /* __CELLOS_LV2__ */

static inline double lol_fsel(double c, double gte, double lt)
{
#if defined __CELLOS_LV2__ && defined __SNC__
    return __fsel(c, gte, lt);
#elif defined __CELLOS_LV2__
    double r;
    __asm__ ("fsel %0, %1, %2, %3"
             : "=f"(r) : "f"(c), "f"(gte), "f"(lt));
    return r;
#else
    if (c >= 0) return gte; return lt;
#endif
}

static inline double lol_fabs(double x)
{
#if defined __CELLOS_LV2__ && defined __SNC__
    return __fabs(x);
#elif defined __CELLOS_LV2__
    double r;
    __asm__ ("fabs %0, %1"
             : "=f"(r) : "f"(x));
    return r;
#else
    return __builtin_fabs(x);
#endif
}

static inline double lol_round(double x)
{
#if defined __CELLOS_LV2__
    return lol_fcfid(lol_fctid(x));
#else
    return __builtin_round(x);
#endif
}

static inline double lol_trunc(double x)
{
#if defined __CELLOS_LV2__
    return lol_fcfid(lol_fctidz(x));
#else
    return __builtin_trunc(x);
#endif
}

double lol_sin(double x)
{
    double absx = lol_fabs(x);
#if defined __CELLOS_LV2__
    double num_cycles = lol_round(absx * INV_PI);
    double is_even = lol_trunc(num_cycles * HALF) - (num_cycles * HALF);
#else
    double num_cycles = absx * INV_PI;
    num_cycles += VERY_LARGE_NUMBER;
    __asm__("" : "+m" (num_cycles));
    num_cycles -= VERY_LARGE_NUMBER;
    double is_even = num_cycles * HALF - HALF;
    is_even += VERY_LARGE_NUMBER;
    __asm__("" : "+m" (is_even));
    is_even -= VERY_LARGE_NUMBER;
    is_even -= num_cycles * HALF;
#endif
    double norm_x = absx - PI * num_cycles;
    double y = norm_x * norm_x;
    double taylor = ((((((((SC[7] * y + SC[6]) * y + SC[5])
                                  * y + SC[4]) * y + SC[3])
                                  * y + SC[2]) * y + SC[1])
                                  * y + SC[0]) * y);
    double sign = lol_fsel(is_even * x, ONE, NEG_ONE);
    double result = norm_x + norm_x * taylor;
    return result * sign;
}

} /* namespace lol */

