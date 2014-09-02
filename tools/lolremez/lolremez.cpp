//
// LolRemez - Remez algorithm implementation
//
// Copyright: (c) 2005-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>

#include <lol/math/real.h>

#include "solver.h"

using lol::real;

/* See the tutorial at http://lolengine.net/wiki/doc/maths/remez */
real f(real const &x)
{
    return exp(x);
}

real g(real const &x)
{
    return exp(x);
}

int main(int argc, char **argv)
{
    UNUSED(argc, argv);

    RemezSolver solver(4, 40);
    solver.Run(-1, 1, f, g);

    return 0;
}

