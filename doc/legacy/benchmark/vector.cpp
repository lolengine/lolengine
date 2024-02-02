//
//  Lol Engine — Benchmark program
//
//  Copyright © 2005—2020 Sam Hocevar <sam@hocevar.net>
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
#include <lol/msg>

using namespace lol;

static size_t const MATRIX_TABLE_SIZE = 64 * 1024;
static size_t const MATRIX_RUNS = 100;

void bench_matrix(int mode)
{
    float result[5] = { 0.0f };
    lol::timer timer;

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
                        pm[i][j][k] = rand(-2.0f, 2.0f);
            break;
        }

        /* Copy matrices */
        timer.get();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pm[i] = pm[i + 1];
        result[0] += timer.get();

        /* Determinant */
        timer.get();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pf[i] = determinant(pm[i]);
        result[1] += timer.get();

        /* Multiply matrices */
        timer.get();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pm[i] *= pm[i + 1];
        result[2] += timer.get();

        /* Add matrices */
        timer.get();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pm[i] += pm[i + 1];
        result[3] += timer.get();

        /* Invert matrix */
        timer.get();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pm[i] = inverse(pm[i]);
        result[4] += timer.get();
    }

    delete[] pm;
    delete[] pf;

    for (size_t i = 0; i < sizeof(result) / sizeof(*result); i++)
        result[i] *= 1e9f / (MATRIX_TABLE_SIZE * MATRIX_RUNS);

    msg::info("                          ns/elem\n");
    msg::info("mat4 = mat4              %7.3f\n", result[0]);
    msg::info("float = mat4.det()       %7.3f\n", result[1]);
    msg::info("mat4 *= mat4             %7.3f\n", result[2]);
    msg::info("mat4 += mat4             %7.3f\n", result[3]);
    msg::info("mat4 = mat4.invert()     %7.3f\n", result[4]);
}

