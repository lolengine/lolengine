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
#include <lol/real>

using namespace lol;

static size_t const REAL_TABLE_SIZE = 10000;
static size_t const REAL_RUNS = 50;

void bench_real(int mode)
{
    float result[12] = { 0.0f };
    lol::timer timer;

    for (size_t run = 0; run < REAL_RUNS; run++)
    {
        switch (mode)
        {
        case 1:
            break;
        }

        real fib1 = 1.0, fib2 = 1.0;
        timer.get();
        for (size_t i = 0; i < REAL_TABLE_SIZE; i++)
        {
            real tmp = fib1 + fib2;
            fib1 = fib2;
            fib2 = tmp;
        }
        result[0] += timer.get();

        real fact = 1.0;
        timer.get();
        for (size_t i = 0; i < REAL_TABLE_SIZE; i++)
            fact = fact * real(1.0 + i);
        result[1] += timer.get();

        real invfact = 1.0;
        timer.get();
        for (size_t i = 0; i < REAL_TABLE_SIZE; i++)
            invfact = invfact / real(1.0 + i);
        result[2] += timer.get();

        timer.get();
        for (size_t i = 0; i < REAL_TABLE_SIZE / 128; i++)
            (void)sin(real(0.01 * i));
        result[3] += timer.get() * 128;

        timer.get();
        for (size_t i = 0; i < REAL_TABLE_SIZE / 128; i++)
            (void)exp((real)(int)(i - REAL_TABLE_SIZE / 256));
        result[4] += timer.get() * 128;
    }

    for (size_t i = 0; i < sizeof(result) / sizeof(*result); i++)
        result[i] *= 1e9f / (REAL_TABLE_SIZE * REAL_RUNS);

    msg::info("                              ns/elem\n");
    msg::info("real = real + real           %7.3f\n", result[0]);
    msg::info("real = real * real           %7.3f\n", result[1]);
    msg::info("real = real / real           %7.3f\n", result[2]);
    msg::info("real = sin(real)             %7.3f\n", result[3]);
    msg::info("real = exp(real)             %7.3f\n", result[4]);
}

