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

#if USE_SDL && defined __APPLE__
#   include <SDL_main.h>
#endif

#include "core.h"

using namespace lol;
using namespace std;

#include "remez-matrix.h"
#include "remez-solver.h"

/* The function we want to approximate */
real myfun(real const &y)
{
real k1024 = 1024;
real klog32 = log((real)32);
return (y - k1024) / log2(log(sqrt(y))/klog32);
    real x = sqrt(y);
    return (sin(x) - x) / (x * y);
}

real myerr(real const &y)
{
return myfun(y);
return real::R_1;
    real x = sqrt(y);
    return sin(x) / (x * y);
}

int main(int argc, char **argv)
{
    RemezSolver<3> solver;
    solver.Run((real)(1024.001), (real)(1024 * 1024), myfun, myerr, 40);

    return EXIT_SUCCESS;
}

