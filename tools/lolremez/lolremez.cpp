//
//  LolRemez - Remez algorithm implementation
//
//  Copyright © 2005—2016 Sam Hocevar <sam@hocevar.net>
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

using lol::array;
using lol::real;
using lol::String;

static void version(void)
{
    printf("lolremez %s\n", PACKAGE_VERSION);
    printf("Copyright © 2005—2016 Sam Hocevar <sam@hocevar.net>\n");
    printf("This program is free software. It comes without any warranty, to the extent\n");
    printf("permitted by applicable law. You can redistribute it and/or modify it under\n");
    printf("the terms of the Do What the Fuck You Want to Public License, Version 2, as\n");
    printf("published by the WTFPL Task Force. See http://www.wtfpl.net/ for more details.\n");
    printf("\n");
    printf("Written by Sam Hocevar. Report bugs to <sam@hocevar.net>.\n");
}

static void usage()
{
    printf("Usage: lolremez [-d degree] [-r xmin:xmax] x-expression [x-error]\n");
    printf("       lolremez -h | --help\n");
    printf("       lolremez -V | --version\n"); 
    printf("Find a polynomial approximation for x-expression.\n");
    printf("\n");
    printf("Mandatory arguments to long options are mandatory for short options too.\n");
    printf("  -d, --degree <degree>      degree of final polynomial\n");
    printf("  -r, --range <xmin>:<xmax>  range over which to approximate\n");
    printf("  -h, --help                 display this help and exit\n");
    printf("  -V, --version              output version information and exit\n");
    printf("\n");
    printf("Examples:\n");
    printf("  lolremez -d 4 -r -1:1 \"atan(exp(1+x))\"\n");
    printf("  lolremez -d 4 -r -1:1 \"atan(exp(1+x))\" \"exp(1+x)\"\n");
    printf("\n");
    printf("Written by Sam Hocevar. Report bugs to <sam@hocevar.net>.\n");
}

static void FAIL(char const *message)
{
    printf("Error: %s\n", message);
    printf("\n");
    usage();
    exit(EXIT_FAILURE);
}

/* See the tutorial at http://lolengine.net/wiki/doc/maths/remez */
int main(int argc, char **argv)
{
    String xmin("-1"), xmax("1");
    char const *f = nullptr, *g = nullptr;
    int degree = 4;

    lol::getopt opt(argc, argv);
    opt.add_arg('d', "degree",  true);
    opt.add_arg('r', "range",   true);
    opt.add_arg('h', "help",    false);
    opt.add_arg('v', "version", false);

    for (;;)
    {
        int c = opt.parse();
        if (c == -1)
            break;

        switch (c)
        {
        case 'd': /* --degree */
            degree = atoi(opt.arg);
            break;
        case 'r': { /* --range */
            array<String> arg = String(opt.arg).split(':');
            if (arg.count() != 2)
                FAIL("invalid range");
            xmin = arg[0];
            xmax = arg[1];
          } break;
        case 'h': /* --help */
            usage();
            return EXIT_SUCCESS;
        case 'v': /* --version */
            version();
            return EXIT_SUCCESS;
        default:
            return EXIT_FAILURE;
        }
    }

    if (opt.index < argc)
        f = argv[opt.index++];

    if (opt.index < argc)
        g = argv[opt.index++];

    if (!f)
        FAIL("no function specified");
    else if (opt.index < argc)
        FAIL("too many arguments");

    if (real(xmin.C()) >= real(xmax.C()))
        FAIL("invalid range");

    remez_solver solver(degree, 20);
    solver.run(xmin.C(), xmax.C(), f, g);

    return 0;
}

