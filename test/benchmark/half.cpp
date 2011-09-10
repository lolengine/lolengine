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

#include <cstdio>

#include "core.h"

using namespace std;
using namespace lol;

static size_t const HALF_TABLE_SIZE = 1024 * 1024;
static size_t const HALF_RUNS = 50;

void bench_half(int mode)
{
    float result[10] = { 0.0f };
    Timer timer;

    /* Set up tables */
    float *pf = new float[HALF_TABLE_SIZE + 1];
    half *ph = new half[HALF_TABLE_SIZE + 1];

    for (size_t run = 0; run < HALF_RUNS; run++)
    {
        switch (mode)
        {
        case 1:
            for (size_t i = 0; i < HALF_TABLE_SIZE + 1; i++)
                ph[i] = half::makebits(rand());
            break;
        case 2:
            for (size_t i = 0; i < HALF_TABLE_SIZE + 1; i++)
                ph[i] = RandF(-2.0f, 2.0f);
            break;
        }

        /* Copy float */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            pf[i] = pf[i + 1];
        result[0] += timer.GetMs();

        /* Convert half to float (array) */
        timer.GetMs();
        half::convert(pf, ph, HALF_TABLE_SIZE);
        result[1] += timer.GetMs();

        /* Convert half to float (fast) */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            pf[i] = (float)ph[i];
        result[2] += timer.GetMs();

        /* Add a half to every float */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            pf[i] += ph[i];
        result[3] += timer.GetMs();

        /* Copy half */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] = ph[i + 1];
        result[4] += timer.GetMs();

        /* Change sign of every half */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] = -ph[i];
        result[5] += timer.GetMs();

        /* Convert float to half (array) */
        timer.GetMs();
        half::convert(ph, pf, HALF_TABLE_SIZE);
        result[6] += timer.GetMs();

        /* Convert float to half (fast) */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] = (half)pf[i];
        result[7] += timer.GetMs();

        /* Convert float to half (accurate) */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] = half::makeaccurate(pf[i]);
        result[8] += timer.GetMs();

        /* Add a float to every half */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] += pf[i];
        result[9] += timer.GetMs();
    }

    delete[] pf;
    delete[] ph;

    for (size_t i = 0; i < sizeof(result) / sizeof(*result); i++)
        result[i] *= 1000000.0f / (HALF_TABLE_SIZE * HALF_RUNS);

    Log::Info("                          ns/elem\n");
    Log::Info("float = float            %7.3f\n", result[0]);
    Log::Info("float = half (array)     %7.3f\n", result[1]);
    Log::Info("float = half (fast)      %7.3f\n", result[2]);
    Log::Info("float += half            %7.3f\n", result[3]);
    Log::Info("half = half              %7.3f\n", result[4]);
    Log::Info("half = -half             %7.3f\n", result[5]);
    Log::Info("half = float (array)     %7.3f\n", result[6]);
    Log::Info("half = float (fast)      %7.3f\n", result[7]);
    Log::Info("half = float (accurate)  %7.3f\n", result[8]);
    Log::Info("half += float            %7.3f\n", result[9]);
}

