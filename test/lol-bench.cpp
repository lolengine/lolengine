//
// Lol Engine - Benchmark program
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
#include "loldebug.h"

using namespace std;
using namespace lol;

int main(int argc, char **argv)
{
    Timer timer;

    uint16_t total = 0;
    for (uint32_t i = 0; i < 0xffffffffu; i++)
    {
        union { float f; uint32_t x; } u;
        u.x = i;

        half h = half::makeslow(u.f);
        total ^= h.bits();
    }
    Log::Info("time for makeslow: %f (hash %04x)\n", timer.GetMs(), total);

    for (uint32_t i = 0; i < 0xffffffffu; i++)
    {
        union { float f; uint32_t x; } u;
        u.x = i;

        half h = half::makefast(u.f);
        total ^= h.bits();
    }
    Log::Info("time for makefast: %f (hash %04x)\n", timer.GetMs(), total);

    return EXIT_SUCCESS;
}

