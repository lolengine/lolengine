//
//  Lol Engine — Sample math program: compute Pi
//
//  Copyright © 2005—2019 Sam Hocevar <sam@hocevar.net>
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

#include <iostream>

#include <lol/engine.h>

using lol::real;

int main(int argc, char **argv)
{
    UNUSED(argc, argv);

    std::cout << "      0: " << real::R_0().str() << '\n';
    std::cout << "      1: " << real::R_1().str() << '\n';
    std::cout << "sqrt(2): " << real::R_SQRT2().str() << '\n';
    std::cout << "sqrt(½): " << real::R_SQRT1_2().str() << '\n';
    std::cout << "  ln(2): " << real::R_LN2().str() << '\n';
    std::cout << "      e: " << real::R_E().str() << '\n';
    std::cout << "      π: " << real::R_PI().str() << '\n';

    // Gauss-Legendre computation of Pi — six iterations are enough for 150 digits
    real a = 1.0, b = real::R_SQRT1_2(), t = 0.25, p = 1.0;

    for (int i = 0; i < 6; i++)
    {
        real tmp = (a - b) * (real)0.5;
        b = sqrt(a * b);
        a -= tmp;
        t -= p * tmp * tmp;
        p += p;
    }

    real sum = a + b;
    sum = sum * sum / ((real)4 * t);
    std::cout << "         " << sum.str() << '\n';

    return EXIT_SUCCESS;
}

