//
// Lol Engine - Sample math program: compute Pi
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

using std::printf;
using std::sqrt;

using lol::real;

int main(int argc, char **argv)
{
    UNUSED(argc, argv);

    printf("Pi: "); real::R_PI().print();
    printf("e: "); real::R_E().print();
    printf("ln(2): "); real::R_LN2().print();
    printf("sqrt(2): "); real::R_SQRT2().print();
    printf("sqrt(1/2): "); real::R_SQRT1_2().print();

    /* Gauss-Legendre computation of Pi -- doesn't work well at all,
     * probably because we use finite precision. */
    real a = 1.0, b = sqrt((real)0.5), t = 0.25, p = 1.0;

    for (int i = 0; i < 3; i++)
    {
        real a2 = (a + b) * (real)0.5;
        real b2 = sqrt(a * b);
        real tmp = a - a2;
        real t2 = t - p * tmp * tmp;
        real p2 = p + p;
        a = a2; b = b2; t = t2; p = p2;
    }

    real sum = a + b;
    sum = sum * sum / ((real)4 * t);
    sum.print();

    return EXIT_SUCCESS;
}

