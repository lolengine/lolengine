//
// Lol Engine - Benchmark program
//
// Copyright: (c) 2005-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined _XBOX
#   define _USE_MATH_DEFINES /* for M_PI */
#   include <xtl.h>
#elif defined _WIN32
#   define _USE_MATH_DEFINES /* for M_PI */
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#include <cstdio>

#if defined HAVE_FASTMATH_H
#   include <fastmath.h>
#endif

#include "core.h"

using namespace std;
using namespace lol;

static size_t const TRIG_TABLE_SIZE = 128 * 1024;
static size_t const TRIG_RUNS = 50;

void bench_trig(int mode)
{
    float result[12] = { 0.0f };
    Timer timer;

    /* Set up tables */
    float *pf = new float[TRIG_TABLE_SIZE];
    float *pf2 = new float[TRIG_TABLE_SIZE];
    float *pf3 = new float[TRIG_TABLE_SIZE];

    for (size_t run = 0; run < TRIG_RUNS; run++)
    {
        switch (mode)
        {
        case 1:
            for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
                pf[i] = rand(-1e5f, 1e5f);
            break;
        case 2:
            for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
                pf[i] = rand(-M_PI, M_PI);
            break;
        case 3:
            for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
                pf[i] = rand(-1e-2f, 1e-2f);
            break;
        }

        /* Sin */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined __GNUC__ && !defined __SNC__
            pf2[i] = __builtin_sinf(pf[i]);
#else
            pf2[i] = sinf(pf[i]);
#endif
        result[0] += timer.Get();

        /* Fast sin */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined HAVE_FASTMATH_H && !defined __native_client__
            pf2[i] = f_sinf(pf[i]);
#else
            pf2[i] = sinf(pf[i]);
#endif
        result[1] += timer.Get();

        /* Lol sin */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            pf2[i] = lol_sin(pf[i]);
        result[2] += timer.Get();

        /* Cos */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined __GNUC__ && !defined __SNC__
            pf2[i] = __builtin_cosf(pf[i]);
#else
            pf2[i] = cosf(pf[i]);
#endif
        result[3] += timer.Get();

        /* Fast cos */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined HAVE_FASTMATH_H && !defined __native_client__
            pf2[i] = f_cosf(pf[i]);
#else
            pf2[i] = cosf(pf[i]);
#endif
        result[4] += timer.Get();

        /* Lol cos */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            pf2[i] = lol_cos(pf[i]);
        result[5] += timer.Get();

        /* Sin & cos */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
        {
#if defined __GNUC__ && !defined __SNC__
            pf2[i] = __builtin_sinf(pf[i]);
            pf3[i] = __builtin_cosf(pf[i]);
#else
            pf2[i] = sinf(pf[i]);
            pf3[i] = cosf(pf[i]);
#endif
        }
        result[6] += timer.Get();

        /* Fast sin & cos */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
        {
#if defined HAVE_FASTMATH_H && !defined __native_client__
            pf2[i] = f_sinf(pf[i]);
            pf3[i] = f_cosf(pf[i]);
#else
            pf2[i] = sinf(pf[i]);
            pf3[i] = cosf(pf[i]);
#endif
        }
        result[7] += timer.Get();

        /* Lol sincos */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            lol_sincos(pf[i], &pf2[i], &pf3[i]);
        result[8] += timer.Get();

        /* Tan */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined __GNUC__ && !defined __SNC__
            pf2[i] = __builtin_tanf(pf[i]);
#else
            pf2[i] = tanf(pf[i]);
#endif
        result[9] += timer.Get();

        /* Fast tan */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined HAVE_FASTMATH_H && !defined __native_client__
            pf2[i] = f_tanf(pf[i]);
#else
            pf2[i] = tanf(pf[i]);
#endif
        result[10] += timer.Get();

        /* Lol tan */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            pf2[i] = lol_tan(pf[i]);
        result[11] += timer.Get();
    }

    delete[] pf;
    delete[] pf2;
    delete[] pf3;

    for (size_t i = 0; i < sizeof(result) / sizeof(*result); i++)
        result[i] *= 1e9f / (TRIG_TABLE_SIZE * TRIG_RUNS);

    Log::Info("                              ns/elem\n");
    Log::Info("float = sinf(float)          %7.3f\n", result[0]);
    Log::Info("float = f_sinf(float)        %7.3f\n", result[1]);
    Log::Info("float = lol_sin(float)       %7.3f\n", result[2]);
    Log::Info("float = cosf(float)          %7.3f\n", result[3]);
    Log::Info("float = f_cosf(float)        %7.3f\n", result[4]);
    Log::Info("float = lol_cos(float)       %7.3f\n", result[5]);
    Log::Info("float = sinf,cosf(float)     %7.3f\n", result[6]);
    Log::Info("float = f_sinf,f_cosf(float) %7.3f\n", result[7]);
    Log::Info("float = lol_sincos(float)    %7.3f\n", result[8]);
    Log::Info("float = tanf(float)          %7.3f\n", result[9]);
    Log::Info("float = f_tanf(float)        %7.3f\n", result[10]);
    Log::Info("float = lol_tanf(float)      %7.3f\n", result[11]);
}

