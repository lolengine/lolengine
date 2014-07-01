//
// Lol Engine - Benchmark program
//
// Copyright: (c) 2005-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include <lol/main.h>

using namespace std;
using namespace lol;

void bench_real(int mode);
void bench_trig(int mode);
void bench_matrix(int mode);
void bench_half(int mode);

int main(int argc, char **argv)
{
    UNUSED(argc, argv);

    Log::Info("-----------------------\n");
    Log::Info(" High precision floats\n");
    Log::Info("-----------------------\n");
    bench_real(1);

    Log::Info("--------------------------\n");
    Log::Info(" Trigonometry [-1e5, 1e5]\n");
    Log::Info("--------------------------\n");
    bench_trig(1);

    Log::Info("------------------------\n");
    Log::Info(" Trigonometry [-pi, pi]\n");
    Log::Info("------------------------\n");
    bench_trig(2);

    Log::Info("----------------------------\n");
    Log::Info(" Trigonometry [-1e-2, 1e-2]\n");
    Log::Info("----------------------------\n");
    bench_trig(3);

    Log::Info("----------------------------\n");
    Log::Info(" Float matrices [-2.0, 2.0]\n");
    Log::Info("----------------------------\n");
    bench_matrix(1);

    Log::Info("-------------------------------------\n");
    Log::Info(" Half precision floats (random bits)\n");
    Log::Info("-------------------------------------\n");
    bench_half(1);

    Log::Info("-----------------------------------\n");
    Log::Info(" Half precision floats [-2.0, 2.0]\n");
    Log::Info("-----------------------------------\n");
    bench_half(2);

#if defined _WIN32
    getchar();
#endif

    return EXIT_SUCCESS;
}

