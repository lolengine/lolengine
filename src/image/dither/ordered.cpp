//
// Lol Engine
//
// Copyright: (c) 2004-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

/*
 * Bayer ordered dithering functions
 */

namespace lol
{

static Image DitherHelper(Image const &image, array2d<float> const &kernel,
                          float scale, float angle);

Image Image::DitherOrdered(array2d<float> const &kernel) const
{
    return DitherHelper(*this, kernel, 1.0f, 0.0f);
}

Image Image::DitherHalftone(float radius, float angle) const
{
    /* Increasing the precision is necessary or the rotation will look
     * like crap. So we create a kernel PRECISION times larger, and ask
     * the ditherer to scale it by 1/PRECISION. */
    float const PRECISION = 4.f;
    int k = (radius * PRECISION * lol::sqrt(2.f) + 0.5f);
    array2d<float> kernel = Image::HalftoneKernel(ivec2(k, k));

    return DitherHelper(*this, kernel, 1.f / PRECISION, angle + F_PI / 4.f);
}

static Image DitherHelper(Image const &image, array2d<float> const &kernel,
                          float scale, float angle)
{
    ivec2 size = image.GetSize();
    ivec2 ksize = kernel.GetSize();

    float cost = lol::cos(angle);
    float sint = lol::sin(angle);

    Image ret = image;
    float *dstp = ret.Lock<PixelFormat::Y_F32>();

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            int kx = (int)((cost * x - sint * y + 2 * size.x * size.y) / scale) % ksize.x;
            int ky = (int)((cost * y + sint * x + 2 * size.x * size.y) / scale) % ksize.y;

            float p = dstp[y * size.x + x];
            dstp[y * size.x + x] = (p > kernel[kx][ky]) ? 1.f : 0.f;
        }
    }

    ret.Unlock(dstp);

    return ret;
}

} /* namespace lol */

