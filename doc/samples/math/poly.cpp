//
//  Lol Engine — Sample math program: polynomials
//
//  Copyright © 2005—2017 Sam Hocevar <sam@hocevar.net>
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

#include <cstdio>

#include <lol/engine.h>

using namespace lol;

int main(int argc, char **argv)
{
    UNUSED(argc, argv);

    polynomial<polynomial<float>> p;

    return EXIT_SUCCESS;
}

