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

void bench_real(int mode);
void bench_matrix(int mode);
void bench_half(int mode);

int main(int, char **)
{
    msg::info("-----------------------\n");
    msg::info(" High precision floats\n");
    msg::info("-----------------------\n");
    bench_real(1);

    msg::info("----------------------------\n");
    msg::info(" Float matrices [-2.0, 2.0]\n");
    msg::info("----------------------------\n");
    bench_matrix(1);

    msg::info("-------------------------------------\n");
    msg::info(" Half precision floats (random bits)\n");
    msg::info("-------------------------------------\n");
    bench_half(1);

    msg::info("-----------------------------------\n");
    msg::info(" Half precision floats [-2.0, 2.0]\n");
    msg::info("-----------------------------------\n");
    bench_half(2);

#if defined _WIN32
    getchar();
#endif

    return EXIT_SUCCESS;
}

