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
    real y = sqrt(x);
    return (sin(y) - y) / (x * y);
}

static real myerr(real const &x)
{
    real y = sqrt(x);
    return re(x * y);
}

int main(void)
{
    RemezSolver<4> solver;
    solver.Run(real::R_1 >> 400, real::R_PI_2 * real::R_PI_2, myfun, myerr, 15);

    return EXIT_SUCCESS;
}

