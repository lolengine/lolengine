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

#include <kinc/graphics4/graphics.h>
#include <lol/engine/sys>
#include <cmath>

int main(int argc, char **argv)
{
    int frame = 0;

    // Open a simple window
    lol::sys::init(argc, argv, "window", 640, 480);

    // Update routine: clear the background color
    lol::sys::add_callback([&frame]() {
        uint32_t alpha = uint32_t(127.0f + 127.0f * std::cos(++frame * 0.1f));
        kinc_g4_begin(0);
        kinc_g4_clear(KINC_G4_CLEAR_COLOR, 0x800000 | alpha * 0x0101, 0.f, 0);
        kinc_g4_end(0);
        kinc_g4_swap_buffers();
        return true;
    });

    lol::sys::run();
    return EXIT_SUCCESS;
}
