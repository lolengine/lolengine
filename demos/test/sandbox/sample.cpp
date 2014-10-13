//
// Lol Engine - Sandbox program
//
// Copyright: (c) 2005-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>

using namespace lol;

int main(int argc, char **argv)
{
    ivec2 size(128, 128);

    if (argc > 1)
        size.x = size.y = atoi(argv[1]);
    if (argc > 2)
        size.y = atoi(argv[2]);

    array2d<float> vac = Image::BlueNoiseKernel(size);
    for (int y = 0; y < size.y; ++y)
        for (int x = 0; x < size.x; ++x)
            printf("%d %d %f\n", x, y, vac[x][y]);

    return EXIT_SUCCESS;
}

