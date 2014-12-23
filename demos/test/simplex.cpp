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

    int const period = 10;
    float const zoom = 0.02f;

    /* Create some gradients */
    array2d<vec2> gradients(vec_t<ptrdiff_t, 2>({period, period}));
    for (int i = 0 ; i < period ; ++i)
        for (int j = 0 ; j < period ; ++j)
            gradients[i][j] = normalize(vec2(rand(-1.f, 1.f), rand(-1.f, 1.f)));
    simplex_interpolator<2> s;
    s.SetGradients(gradients);

    /* Create an image */
    ivec2 const size(800, 600);
    Image img(size);
    array2d<vec4> &data = img.Lock2D<PixelFormat::RGBA_F32>();

    /* Fill image with simplex noise */
    for (int j = 0; j < size.y; ++j)
    for (int i = 0; i < size.x; ++i)
    {
        float p = 0.5f * s.Interp(zoom * vec2(i, j));
#if 0
        for (int k = 2; k < 128; k *= 2)
            p += 0.5f / k * s.Interp(zoom * k * vec2(i, j));
#endif
        data[i][j] = vec4(saturate(0.5f + p), 0.f, 0.f, 1.f);
    }

    /* Mark simplex vertices */
    vec2 diagonal = normalize(vec2(1.f));
    vec2 dx = mat2::rotate(60.f) * diagonal;
    vec2 dy = mat2::rotate(-60.f) * diagonal;
    for (int j = -100; j < 100; ++j)
    for (int i = -100; i < 100; ++i)
    {
        ivec2 coord = ivec2(i / zoom * dx + j / zoom * dy);
        if (coord.x >= 0 && coord.x < size.x - 1
             && coord.y >= 0 && coord.y < size.y - 1)
        {
            data[coord.x][coord.y] = vec4(1.f, 0.f, 1.f, 1.f);
            data[coord.x + 1][coord.y] = vec4(1.f, 0.f, 1.f, 1.f);
            data[coord.x][coord.y + 1] = vec4(1.f, 0.f, 1.f, 1.f);
            data[coord.x + 1][coord.y + 1] = vec4(1.f, 0.f, 1.f, 1.f);
        }
    }

    /* Save image */
    img.Unlock2D(data);
    img.Save("simplex.png");
}

