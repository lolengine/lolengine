//
// Lol Engine - Sample math program: compute Pi
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

#include "core.h"

using namespace lol;

int main(int argc, char **argv)
{
    /* Approximate Pi using Machin's formula: 16*atan(1/5)-4*atan(1/239) */
    real sum = 0.0, x0 = 5.0, x1 = 239.0;
    real const m0 = -x0 * x0, m1 = -x1 * x1, r16 = 16.0, r4 = 4.0;

    /* Degree 240 is required for 512-bit mantissa precision */
    for (int i = 1; i < 240; i += 2)
    {
        sum += r16 / (x0 * (real)i) - r4 / (x1 * (real)i);
        x0 *= m0;
        x1 *= m1;
    }

    sum.print();

    return EXIT_SUCCESS;
}

