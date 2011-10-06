//
// Lol Engine - Sample math program: Chebyshev polynomials
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

#include <cstring>
#include <cstdio>

#include "core.h"

using namespace lol;
using namespace std;

#include "remez-matrix.h"
#include "remez-solver.h"

/* The function we want to approximate */
static real myfun(real const &x)
{
    static real const a0 = real::R_1;
    static real const a1 = real(-11184811) >> 26;
    static real const b1 = real(-1) / real(6);
    static real const b2 = real(1) / real(120);
    real y = sqrt(x);
    return sin(y) / y;
}

static real myerr(real const &x)
{
    return real::R_1;
}

int main(int argc, char **argv)
{
    RemezSolver<6> solver;
    //solver.Run(0, 1, myfun, myfun, 15);
    solver.Run(exp((real)-100), real::R_PI * real::R_PI >> 2, myfun, myfun, 15);
    //solver.Run(-1, 1, myfun, myfun, 15);
    //solver.Run(0, real::R_PI * real::R_PI >> 4, myfun, myfun, 15);

    return EXIT_SUCCESS;
}

