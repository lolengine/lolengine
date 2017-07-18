//
//  bluenoise — create a N×N blue noise kernel
//
//  Copyright © 2016—2017 Sam Hocevar <sam@hocevar.net>
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

int main(int argc, char **argv)
{
    UNUSED(argc, argv);

    ivec2 const size(64);
    auto const &kernel = image::kernel::blue_noise(size, ivec2(8));

    image im(size.xy);
    array2d<vec4> &data = im.lock2d<PixelFormat::RGBA_F32>();

    for (int j = 0; j < size.y; ++j)
    for (int i = 0; i < size.x; ++i)
        data[i][j] = vec4(vec3(kernel[i][j]), 1.0f);
    im.unlock2d(data);
    im.save("bluenoise.png");

    return 0;
}

