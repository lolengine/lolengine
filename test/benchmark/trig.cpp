//
// Lol Engine - Benchmark program
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

#ifdef WIN32
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
                pf[i] = RandF(-1e5f, 1e5f);
            break;
        case 2:
            for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
                pf[i] = RandF(-M_PI, M_PI);
            break;
        case 3:
            for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
                pf[i] = RandF(-1e-2f, 1e-2f);
            break;
        }

        /* Sin */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined __GNUC__ && !defined __SNC__
            pf2[i] = __builtin_sinf(pf[i]);
#else
            pf2[i] = sinf(pf[i]);
#endif
        result[0] += timer.GetMs();

        /* Fast sin */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined HAVE_FASTMATH_H
            pf2[i] = f_sinf(pf[i]);
#else
            pf2[i] = sinf(pf[i]);
#endif
        result[1] += timer.GetMs();

        /* Lol sin */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            pf2[i] = lol_sin(pf[i]);
        result[2] += timer.GetMs();

        /* Cos */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined __GNUC__ && !defined __SNC__
            pf2[i] = __builtin_cosf(pf[i]);
#else
            pf2[i] = cosf(pf[i]);
#endif
        result[3] += timer.GetMs();

        /* Fast cos */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined HAVE_FASTMATH_H
            pf2[i] = f_cosf(pf[i]);
#else
            pf2[i] = cosf(pf[i]);
#endif
        result[4] += timer.GetMs();

        /* Lol cos */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            pf2[i] = lol_cos(pf[i]);
        result[5] += timer.GetMs();

        /* Sin & cos */
        timer.GetMs();
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
        result[6] += timer.GetMs();

        /* Fast sin & cos */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
        {
#if defined HAVE_FASTMATH_H
            pf2[i] = f_sinf(pf[i]);
            pf3[i] = f_cosf(pf[i]);
#else
            pf2[i] = sinf(pf[i]);
            pf3[i] = cosf(pf[i]);
#endif
        }
        result[7] += timer.GetMs();

        /* Lol sincos */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            lol_sincos(pf[i], &pf2[i], &pf3[i]);
        result[8] += timer.GetMs();

        /* Tan */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined __GNUC__ && !defined __SNC__
            pf2[i] = __builtin_tanf(pf[i]);
#else
            pf2[i] = tanf(pf[i]);
#endif
        result[9] += timer.GetMs();

        /* Fast tan */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined HAVE_FASTMATH_H
            pf2[i] = f_tanf(pf[i]);
#else
            pf2[i] = tanf(pf[i]);
#endif
        result[10] += timer.GetMs();

        /* Lol tan */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            pf2[i] = lol_tan(pf[i]);
        result[11] += timer.GetMs();
    }

    delete[] pf;
    delete[] pf2;
    delete[] pf3;

    for (size_t i = 0; i < sizeof(result) / sizeof(*result); i++)
        result[i] *= 1000000.0f / (TRIG_TABLE_SIZE * TRIG_RUNS);

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

