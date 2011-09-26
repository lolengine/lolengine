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

#include "core.h"

using namespace lol;

static int const ORDER = 12;

static int cheby[ORDER][ORDER];

static void make_table()
{
    memset(cheby[0], 0, ORDER * sizeof(int));
    cheby[0][0] = 1;
    memset(cheby[1], 0, ORDER * sizeof(int));
    cheby[1][1] = 1;

    for (int i = 2; i < ORDER; i++)
    {
        cheby[i][0] = -cheby[i - 2][0];
        for (int j = 1; j < ORDER; j++)
            cheby[i][j] = 2 * cheby[i - 1][j - 1] - cheby[i - 2][j];
    }
}

int main(int argc, char **argv)
{
    make_table();

for (int i = 0; i < ORDER; i++)
{
    for (int j = 0; j < ORDER; j++)
        printf("% 5i ", cheby[i][j]);
    printf("\n");
}

    return EXIT_SUCCESS;
}

