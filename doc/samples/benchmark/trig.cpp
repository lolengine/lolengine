//
//  Lol Engine — Benchmark program
//
//  Copyright © 2005—2015 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#if HAVE_FASTMATH_H
#   include <fastmath.h>
#endif

#include <lol/engine.h>

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
                pf[i] = rand(-F_PI, F_PI);
            break;
        case 3:
            for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
                pf[i] = rand(-1e-2f, 1e-2f);
            break;
        }

        /* Sin */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if __GNUC__ && !__SNC__
            pf2[i] = __builtin_sinf(pf[i]);
#else
            pf2[i] = sinf(pf[i]);
#endif
        result[0] += timer.Get();

        /* Fast sin */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if HAVE_FASTMATH_H && !__native_client__ && !EMSCRIPTEN
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
#if __GNUC__ && !__SNC__
            pf2[i] = __builtin_cosf(pf[i]);
#else
            pf2[i] = cosf(pf[i]);
#endif
        result[3] += timer.Get();

        /* Fast cos */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if HAVE_FASTMATH_H && !__native_client__ && !EMSCRIPTEN
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
#if __GNUC__ && !__SNC__
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
#if HAVE_FASTMATH_H && !__native_client__ && !EMSCRIPTEN
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
#if __GNUC__ && !__SNC__
            pf2[i] = __builtin_tanf(pf[i]);
#else
            pf2[i] = tanf(pf[i]);
#endif
        result[9] += timer.Get();

        /* Fast tan */
        timer.Get();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if HAVE_FASTMATH_H && !__native_client__ && !EMSCRIPTEN
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

    msg::info("                              ns/elem\n");
    msg::info("float = sinf(float)          %7.3f\n", result[0]);
    msg::info("float = f_sinf(float)        %7.3f\n", result[1]);
    msg::info("float = lol_sin(float)       %7.3f\n", result[2]);
    msg::info("float = cosf(float)          %7.3f\n", result[3]);
    msg::info("float = f_cosf(float)        %7.3f\n", result[4]);
    msg::info("float = lol_cos(float)       %7.3f\n", result[5]);
    msg::info("float = sinf,cosf(float)     %7.3f\n", result[6]);
    msg::info("float = f_sinf,f_cosf(float) %7.3f\n", result[7]);
    msg::info("float = lol_sincos(float)    %7.3f\n", result[8]);
    msg::info("float = tanf(float)          %7.3f\n", result[9]);
    msg::info("float = f_tanf(float)        %7.3f\n", result[10]);
    msg::info("float = lol_tanf(float)      %7.3f\n", result[11]);
}

