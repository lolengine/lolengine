//
//  Lol Engine — Benchmark program
//
//  Copyright © 2005—2018 Sam Hocevar <sam@hocevar.net>
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

#include <lol/engine.h>

using namespace lol;

static size_t const HALF_TABLE_SIZE = 1024 * 1024;
static size_t const HALF_RUNS = 50;

void bench_half(int mode)
{
    float result[7] = { 0.0f };
    lol::timer timer;

    /* Set up tables */
    float *pf = new float[HALF_TABLE_SIZE + 1];
    half *ph = new half[HALF_TABLE_SIZE + 1];

    for (size_t run = 0; run < HALF_RUNS; run++)
    {
        switch (mode)
        {
        case 1:
            for (size_t i = 0; i < HALF_TABLE_SIZE + 1; i++)
                ph[i] = half::frombits(rand<uint16_t>());
            break;
        case 2:
        default:
            for (size_t i = 0; i < HALF_TABLE_SIZE + 1; i++)
                ph[i] = rand(-2.0f, 2.0f);
            break;
        }

        /* Convert half to float */
        timer.get();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            pf[i] = (float)ph[i];
        result[0] += timer.get();

        /* Copy float */
        timer.get();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            pf[i] = pf[i + 1];
        result[1] += timer.get();

        /* Add a half to every float */
        timer.get();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            pf[i] += ph[i];
        result[2] += timer.get();

        /* Copy half */
        timer.get();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] = ph[i + 1];
        result[3] += timer.get();

        /* Change sign of every half */
        timer.get();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] = -ph[i];
        result[4] += timer.get();

        /* Convert float to half (accurate) */
        timer.get();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] = half(pf[i]);
        result[5] += timer.get();

        /* Add a float to every half */
        timer.get();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] += pf[i];
        result[6] += timer.get();
    }

    delete[] pf;
    delete[] ph;

    for (size_t i = 0; i < sizeof(result) / sizeof(*result); i++)
        result[i] *= 1e9f / (HALF_TABLE_SIZE * HALF_RUNS);

    msg::info("                          ns/elem\n");
    msg::info("float = half     %7.3f\n", result[0]);
    msg::info("float = float    %7.3f\n", result[1]);
    msg::info("float += half    %7.3f\n", result[2]);
    msg::info("half = half      %7.3f\n", result[3]);
    msg::info("half = -half     %7.3f\n", result[4]);
    msg::info("half = float     %7.3f\n", result[5]);
    msg::info("half += float    %7.3f\n", result[6]);
}

