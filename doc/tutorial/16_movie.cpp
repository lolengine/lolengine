//
//  Lol Engine — GIF encoding sample
//
//  Copyright © 2016 Sam Hocevar <sam@hocevar.net>
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

    for (int i = 0; i < 256; ++i)
    {
        lol::image im(size);

        lol::array2d<lol::u8vec3> &data = im.lock2d<lol::PixelFormat::RGB_8>();
        for (int y = 0; y < size.y; ++y)
        for (int x = 0; x < size.x; ++x)
        {
            data[x][y].r = x * i / 2;
            data[x][y].g = x / 4 * 4 * y / 16 + i;
            data[x][y].b = y + i;
        }
        im.unlock2d(data);

        if (!movie.push_image(im))
            break;
    }

    movie.close();

    return 0;
}

