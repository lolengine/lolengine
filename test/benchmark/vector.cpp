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

static size_t const MATRIX_TABLE_SIZE = 64 * 1024;
static size_t const MATRIX_RUNS = 100;

void bench_matrix(int mode)
{
    float result[5] = { 0.0f };
    Timer timer;

    /* Set up tables */
    mat4 *pm = new mat4[MATRIX_TABLE_SIZE + 1];
    float *pf = new float[MATRIX_TABLE_SIZE];

    for (size_t run = 0; run < MATRIX_RUNS; run++)
    {
        switch (mode)
        {
        case 1:
            for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
                for (int j = 0; j < 4; j++)
                    for (int k = 0; k < 4; k++)
                        pm[i][j][k] = RandF(-2.0f, 2.0f);
            break;
        }

        /* Copy matrices */
        timer.GetMs();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pm[i] = pm[i + 1];
        result[0] += timer.GetMs();

        /* Determinant */
        timer.GetMs();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pf[i] = determinant(pm[i]);
        result[1] += timer.GetMs();

        /* Multiply matrices */
        timer.GetMs();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pm[i] *= pm[i + 1];
        result[2] += timer.GetMs();

        /* Add matrices */
        timer.GetMs();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pm[i] += pm[i + 1];
        result[3] += timer.GetMs();

        /* Invert matrix */
        timer.GetMs();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pm[i] = inverse(pm[i]);
        result[4] += timer.GetMs();
    }

    delete[] pm;
    delete[] pf;

    for (size_t i = 0; i < sizeof(result) / sizeof(*result); i++)
        result[i] *= 1000000.0f / (MATRIX_TABLE_SIZE * MATRIX_RUNS);

    Log::Info("                          ns/elem\n");
    Log::Info("mat4 = mat4              %7.3f\n", result[0]);
    Log::Info("float = mat4.det()       %7.3f\n", result[1]);
    Log::Info("mat4 *= mat4             %7.3f\n", result[2]);
    Log::Info("mat4 += mat4             %7.3f\n", result[3]);
    Log::Info("mat4 = mat4.invert()     %7.3f\n", result[4]);
}

