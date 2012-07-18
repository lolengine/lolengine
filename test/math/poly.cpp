//
// Lol Engine - Sample math program: chek trigonometric functions
//
// Copyright: (c) 2005-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include "core.h"

using namespace lol;
using namespace std;

float adjustf(float f, int i) __attribute__((noinline));
float adjustf(float f, int i)
{
    union { float f; uint32_t x; } u = { f };
    u.x += i;
    return u.f;
}

double adjust(double f, int i) __attribute__((noinline));
double adjust(double f, int i)
{
    union { double f; uint64_t x; } u = { f };
    u.x += i;
    return u.f;
}

static void inspect(float f)
{
    union { float f; uint32_t x; } u = { f };
    printf("%08x %14.12g  --  ", u.x, u.f);
    int i = (u.x & 0x7fffffu) | 0x800000u;
    int j = 23 - ((u.x >> 23) & 0xff) + ((1 << 7) - 1);
    if (u.f <= 0)
        i = -i;
    printf("%i / 2^%i = %14.12g\n", i, j, (float)i / (1LLu << j));
}

//#define float double
#if 1
static float const a0 = 1.0;
static float const a1 = -0.1666666666663036;
static float const a2 = 0.008333333325075758;
static float const a3 = -0.0001984126372689299;
static float const a4 = 2.755533925906394e-06;
static float const a5 = -2.476042626296988e-08;
static float const a6 = 0.0;
#elif 0
static float const a0 = adjust(0.9999999999999376, 0);
static float const a1 = adjust(-0.1666666666643236, 0);
static float const a2 = adjust(0.008333333318766562, 0);
static float const a3 = adjust(-0.0001984126641174625, 0);
static float const a4 = adjust(2.755693193297308e-006, 0);
static float const a5 = adjust(-2.502951900290311e-008, 0);
static float const a6 = adjust(1.540117123154927e-010, 0);
#elif 0
static float const a0 = adjust(1.0, 0);
static float const a1 = adjust(-0.1666666666372165, 0);
static float const a2 = adjust(0.008333332748323419, 0);
static float const a3 = adjust(-0.0001984108245332497, 0);
static float const a4 = adjust(2.753619853326498e-06, 0);
static float const a5 = adjust(-2.407483949485896e-08, 0);
static float const a6 = 0.0;
#else
static float const a0 = adjust(0.9999999946887117, 0);
static float const a1 = adjust(-0.1666665668590824, 0);
static float const a2 = adjust(0.008333025160523476, 0);
static float const a3 = adjust(-0.0001980741944205014, 0);
static float const a4 = adjust(2.60190356966559e-06, 0); // -900 in floats
static float const a5 = 0.0;
static float const a6 = 0.0;
#endif

static float floatsin(float f)
{
    return lol_sin(f);
    //static float const k = (float)real::R_2_PI;

    //f *= k;
    float f2 = f * f;
    float f4 = f2 * f2;
    return f * (a0 + f4 * (a2 + f4 * (a4 + f4 * a6)) + f2 * (a1 + f4 * (a3 + f4 * a5)));
    //return f * (a0 + f2 * (a1 + f2 * (a2 + f2 * (a3 + f2 * (a4 + f2 * (a5 + f2 * a6))))));
    //return f * (a0 + a1 * f2 + a2 * f2 * f2 + a3 * f2 * f2 * f2 + a4 * f2 * f2 * f2 * f2 + a5 * f2 * f2 * f2 * f2 * f2 + a6 * f2 * f2 * f2 * f2 * f2 * f2);
#undef float
}

int main(int argc, char **argv)
{
    typedef union { float f; uint32_t x; } flint;

    int error[5] = { 0 };

    inspect(a0);
    inspect(a1);
    inspect(a2);
    inspect(a3);
    inspect(a4);
    inspect(a5);

//flint v = { 1.433971524239 };
flint v = { 1.555388212204 };
inspect(v.f);
printf("sinf: ");
flint w = { sinf(adjustf(v.f, 0)) };
inspect(w.f);
printf("lols: ");
flint z = { lol_sin(adjustf(v.f, 0)) };
inspect(z.f);

printf("-- START --\n");
    for (flint u = { (float)real::R_PI_2 }; u.f > 1e-30; u.x -= 1)
    {
        union { float f; uint32_t x; } s1 = { sinf(adjustf(u.f, 0)) };
        union { float f; uint32_t x; } s2 = { floatsin(adjustf(u.f, 0)) };
        int e = abs((int)(s1.x - s2.x));
        switch (e)
        {
        case 3:
        case 2:
        case 1:
inspect(u.f);
printf("sinf: ");
inspect(sinf(u.f));
            if (fabs((double)s1.f - (double)s2.f) > 1e-10 * fabs(s1.f))
                printf("%15.13g %08x: %15.13g %15.13g %08x %08x\n", u.f, u.x, s1.f, s2.f, s1.x, s2.x);
        case 0:
            error[e]++;
            break;
        default:
            error[4]++;
            break;
        }
    }

    printf("exact: %i  off by 1: %i  by 2: %i  by 3: %i  error: %i\n",
           error[0], error[1], error[2], error[3], error[4]);

    return EXIT_SUCCESS;
}

