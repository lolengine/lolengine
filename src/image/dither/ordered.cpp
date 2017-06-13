//
//  Lol Engine
//
//  Copyright © 2004—2017 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

/*
 * Bayer ordered dithering functions
 */

namespace lol
{

static image dither_helper(image const &img, array2d<float> const &ker,
                           float scale, float angle);

image image::dither_ordered(array2d<float> const &ker) const
{
    return dither_helper(*this, ker, 1.0f, 0.0f);
}

image image::dither_halftone(float radius, float angle) const
{
    /* Increasing the precision is necessary or the rotation will look
     * like crap. So we create a kernel PRECISION times larger, and ask
     * the ditherer to scale it by 1/PRECISION. */
    float const PRECISION = 4.f;
    int k = (radius * PRECISION * lol::sqrt(2.f) + 0.5f);
    array2d<float> ker = image::kernel::halftone(ivec2(k, k));

    return dither_helper(*this, ker, 1.f / PRECISION, angle + F_PI / 4.f);
}

static image dither_helper(image const &img, array2d<float> const &ker,
                           float scale, float angle)
{
    ivec2 isize = img.size();
    ivec2 ksize = ker.size();

    float cost = lol::cos(angle);
    float sint = lol::sin(angle);

    image ret = img;
    float *dstp = ret.lock<PixelFormat::Y_F32>();

    for (int y = 0; y < isize.y; y++)
    {
        for (int x = 0; x < isize.x; x++)
        {
            int kx = (int)((cost * x - sint * y + 2 * isize.x * isize.y) / scale) % ksize.x;
            int ky = (int)((cost * y + sint * x + 2 * isize.x * isize.y) / scale) % ksize.y;

            float p = dstp[y * isize.x + x];
            dstp[y * isize.x + x] = (p > ker[kx][ky]) ? 1.f : 0.f;
        }
    }

    ret.unlock(dstp);

    return ret;
}

} /* namespace lol */

