//
//  Simplex Noise Test Program
//
//  Copyright (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//            (c) 2013-2014 Guillaume Bittoun <guillaume.bittoun@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>

using namespace lol;

int main(int argc, char **argv)
{
    UNUSED(argc, argv);

    float const zoom = 0.03f;
    int const octaves = 10;

    /* Create an image */
    ivec2 const size(1280, 720);
    Image img(size);
    array2d<vec4> &data = img.Lock2D<PixelFormat::RGBA_F32>();

    /* Fill image with simplex noise */
    simplex_interpolator<2> s2;
    simplex_interpolator<3> s3;
    simplex_interpolator<4> s4;

    for (int j = 0; j < size.y; ++j)
    for (int i = 0; i < size.x; ++i)
    {
        float sum = 0.f;
        int maxoctave = (j < size.y / 2) ? 1 : (1 << octaves);

        for (int k = 1; k <= maxoctave; k *= 2)
        {
            if (i < size.x / 3)
                sum += 0.5f / k * s2.Interp(zoom * k * vec2(i, j));
            else if (i < size.x * 2 / 3)
                sum += 0.5f / k * s3.Interp(zoom * k * vec3(i, j, 0.0f));
            else
                sum += 0.5f / k * s4.Interp(zoom * k * vec4(i, j, 0.0f, 0.0f));
        }

        float c = saturate(0.5f + sum);
        data[i][j] = vec4(c, c, c, 1.f);
    }

    /* Save image */
    img.Unlock2D(data);
    img.Save("simplex.png");
}

