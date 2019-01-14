//
//  Lol Engine — GIF encoding sample
//
//  Copyright © 2016—2019 Sam Hocevar <sam@hocevar.net>
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

#include <lol/engine.h>
#include "loldebug.h"

int main(int argc, char **argv)
{
    UNUSED(argc, argv);

    lol::ivec2 size(256, 256);

    lol::movie movie(size);
    if (!movie.open_file("16_movie.gif"))
        return EXIT_FAILURE;

    // Use 4D Perlin noise
    lol::perlin_noise<4> noise;

    for (int i = 0; i < 256; ++i)
    {
        lol::image im(size);

        lol::array2d<lol::u8vec3> &data = im.lock2d<lol::PixelFormat::RGB_8>();
        for (int y = 0; y < size.y; ++y)
        for (int x = 0; x < size.x; ++x)
        {
            float alpha = lol::F_TAU * i / 256;
            float beta = lol::F_TAU * i / 256;
            lol::vec4 p(2.f * x / (float)size.x + cos(alpha),
                        2.f * x / (float)size.x + sin(alpha),
                        2.f * y / (float)size.y + cos(beta),
                        2.f * y / (float)size.y + sin(beta));

            data[x][y].r = 128 * (noise.eval(p) + 1 + lol::rand(0.1f));
            data[x][y].g = 128 * (noise.eval(p.zyxw) + 1 + lol::rand(0.1f));
            data[x][y].b = 128 * (noise.eval(p.ywxz) + 1 + lol::rand(0.1f));
        }
        im.unlock2d(data);

        if (!movie.push_image(im))
            break;
    }

    movie.close();

    return 0;
}

