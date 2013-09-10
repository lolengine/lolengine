//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cstdlib>

#include "core.h"

#include <lol/unit.h>

int main(int argc, char **argv)
{
    UNUSED(argc, argv);

    lol::TextTestRunner runner;
    bool success = runner.Run();
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
