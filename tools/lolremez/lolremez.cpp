//
//  LolRemez - Remez algorithm implementation
//
//  Copyright © 2005—2015 Sam Hocevar <sam@hocevar.net>
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

#include <lol/engine.h>

#include <lol/math/real.h>

#include "solver.h"
#include "expression.h"

using lol::real;
using lol::String;

void FAIL(char const *message)
{
    printf("Error: %s\n", message);
    printf("\n");
    printf("Usage:\n");
    printf("  lolremez [-d degree] [-i xmin xmax] x-expression [x-error]\n");
    printf("\n");
    printf("Example:\n");
    printf("  lolremez -d 4 -i -1 1 \"atan(exp(1+x))\"\n");
    printf("  lolremez -d 4 -i -1 1 \"atan(exp(1+x))\" \"exp(1+x)\"\n");
    printf("\n");

    exit(EXIT_FAILURE);
}

/* See the tutorial at http://lolengine.net/wiki/doc/maths/remez */
int main(int argc, char **argv)
{
    char const *xmin = "-1", *xmax = "1";
    char const *f = nullptr, *g = nullptr;
    int degree = 4;

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i] == String("-d"))
        {
            if (i + 1 >= argc)
                FAIL("not enough arguments for -d");

            degree = atoi(argv[++i]);
        }
        else if (argv[i] == String("-i"))
        {
            if (i + 2 >= argc)
                FAIL("not enough arguments for -i");

            xmin = argv[++i];
            xmax = argv[++i];
        }
        else if (g)
        {
            FAIL("unknown argument");
        }
        else if (f)
        {
            g = argv[i];
        }
        else
        {
            f = argv[i];
        }
    }

    if (!f)
        FAIL("no function specified");

    if (real(xmin) >= real(xmax))
        FAIL("invalid range");

    remez_solver solver(degree, 20);
    solver.run(xmin, xmax, f, g);

    return 0;
}

