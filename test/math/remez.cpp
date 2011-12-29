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

#include <cstdio>
#include <cstdlib>

#if USE_SDL && defined __APPLE__
#   include <SDL_main.h>
#endif

#include "lol/math/real.h"
#include "lol/math/matrix.h"
#include "lol/math/remez.h"

using lol::real;
using lol::RemezSolver;

/* The function we want to approximate */
real myfun(real const &y)
{
    real x = sqrt(y);
    return (sin(x) - x) / (x * y);
}

real myerr(real const &y)
{
    real x = sqrt(y);
    return sin(x) / (x * y);
}

int main(int argc, char **argv)
{
    RemezSolver<2, real> solver;
    solver.Run(real::R_1 >> 400, real::R_PI_2 * real::R_PI_2, myfun, myerr, 40);

    return EXIT_SUCCESS;
}

