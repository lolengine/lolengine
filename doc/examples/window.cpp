//
//  Lol Engine — Window example
//
//  Copyright © 2012–2024 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine/sys>

int main(int argc, char **argv)
{
    lol::sys::init(argc, argv, "window", 640, 480);
    lol::sys::run();
    return EXIT_SUCCESS;
}
