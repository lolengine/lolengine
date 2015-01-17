//
//  Lol Engine - Sandbox program
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

using namespace lol;

int main()
{
    Timer t;

    bigint<128> x(17), y(23);
    x.print();
    y.print();

    auto z = x * y;
    z.print();

    for (int i = 0; i < 500000; ++i)
    {
        x = (bigint<128>)(x * x);
        x ^= y;
    }

    printf("%d %d\n", (int)x, (int)y);

    printf("Time: %f s\n", t.Get());
}

