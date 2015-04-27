//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2014 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdio>
#include <cstdlib>

#include <lolunit.h>

int main(int argc, char **argv)
{
    UNUSED(argc, argv);

    lol::TextTestRunner runner;
    bool success = runner.Run();
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

