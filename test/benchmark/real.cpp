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

static size_t const REAL_TABLE_SIZE = 10000;
static size_t const REAL_RUNS = 50;

void bench_real(int mode)
{
    float result[12] = { 0.0f };
    Timer timer;

    for (size_t run = 0; run < REAL_RUNS; run++)
    {
        switch (mode)
        {
        case 1:
            break;
        }

        real fib1 = 1.0, fib2 = 1.0;
        timer.GetMs();
        for (size_t i = 0; i < REAL_TABLE_SIZE; i++)
        {
            real tmp = fib1 + fib2;
            fib1 = fib2;
            fib2 = tmp;
        }
        result[0] += timer.GetMs();

        real fact = 1.0;
        timer.GetMs();
        for (size_t i = 0; i < REAL_TABLE_SIZE; i++)
            fact = fact * real(1.0 + i);
        result[1] += timer.GetMs();

        real invfact = 1.0;
        timer.GetMs();
        for (size_t i = 0; i < REAL_TABLE_SIZE; i++)
            invfact = invfact / real(1.0 + i);
        result[2] += timer.GetMs();

        timer.GetMs();
        for (size_t i = 0; i < REAL_TABLE_SIZE / 128; i++)
            sin(real(0.01 * i));
        result[3] += timer.GetMs() * 128;

        timer.GetMs();
        for (size_t i = 0; i < REAL_TABLE_SIZE / 128; i++)
            exp((real)(int)(i - REAL_TABLE_SIZE / 256));
        result[4] += timer.GetMs() * 128;
    }

    for (size_t i = 0; i < sizeof(result) / sizeof(*result); i++)
        result[i] *= 1000000.0f / (REAL_TABLE_SIZE * REAL_RUNS);

    Log::Info("                              ns/elem\n");
    Log::Info("real = real + real           %7.3f\n", result[0]);
    Log::Info("real = real * real           %7.3f\n", result[1]);
    Log::Info("real = real / real           %7.3f\n", result[2]);
    Log::Info("real = sin(real)             %7.3f\n", result[3]);
    Log::Info("real = exp(real)             %7.3f\n", result[4]);
}

