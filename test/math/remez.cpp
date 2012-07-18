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

#include "lol/math/real.h"
#include "lol/math/remez.h"

using lol::real;
using lol::RemezSolver;

/* See the tutorial at http://lol.zoy.org/wiki/doc/maths/remez */
real f(real const &x) { return exp(x); }
real g(real const &x) { return exp(x); }

int main(int argc, char **argv)
{
    RemezSolver<4, real> solver;
    solver.Run(-1, 1, f, g, 40);
    return 0;
}

