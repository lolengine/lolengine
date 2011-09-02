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
static const double NEG_PI = -3.14159265358979323846;
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
static const double TWO_EXP_52 = 4503599627370496.0;
static const double TWO_EXP_54 = 18014398509481984.0;

/** sin Taylor series coefficients. */
static const double SC[]    =
{
    -1.6449340668482264364724e-0, // pi^2/3!
    +8.1174242528335364363700e-1, // pi^4/5!
    -1.9075182412208421369647e-1, // pi^6/7!
    +2.6147847817654800504653e-2, // pi^8/9!
    -2.3460810354558236375089e-3, // pi^10/11!
    +1.4842879303107100368487e-4, // pi^12/13!
    -6.9758736616563804745344e-6, // pi^14/15!
    +2.5312174041370276513517e-7, // pi^16/17!
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
    return (c >= 0) ? gte : lt;
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
    double absx = lol_fabs(x * INV_PI);
    double sign = lol_fsel(x, PI, NEG_PI);

    /* To compute sin(x) we build a Taylor series for |x|/pi wrapped to
     * the range [-1, 1]. We also switch the result sign if the number
     * of cycles is odd. */
#if defined __CELLOS_LV2__
    double num_cycles = lol_round(absx);
    double is_even = lol_trunc(num_cycles * HALF) - (num_cycles * HALF);
    sign = lol_fsel(is_even, sign, -sign);
#else
    double num_cycles = absx + TWO_EXP_52;
    __asm__("" : "+m" (num_cycles)); num_cycles -= TWO_EXP_52;

    double is_even = TWO * num_cycles - ONE;
    __asm__("" : "+m" (is_even)); is_even += TWO_EXP_54;
    __asm__("" : "+m" (is_even)); is_even -= TWO_EXP_54;
    __asm__("" : "+m" (is_even));
    is_even -= TWO * num_cycles - ONE;
    sign *= is_even;
#endif
    double norm_x = absx - num_cycles;

    /* Computing x^4 is one multiplication too many we do, but it helps
     * interleave the Taylor series operations a lot better. */
    double x2 = norm_x * norm_x;
    double x4 = x2 * x2;
    double sub1 = ((SC[7] * x4 + SC[5]) * x4 + SC[3]) * x4 + SC[1];
    double sub2 = ((SC[6] * x4 + SC[4]) * x4 + SC[2]) * x4 + SC[0];
    double taylor = (sub1 * x2 + sub2) * x2 + ONE;

    double result = norm_x * taylor;
    return result * sign;
}

} /* namespace lol */

