//
//  Lol Engine
//
//  Copyright © 2004—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

/*
 * Generic error diffusion functions
 */

namespace lol
{

/* Perform a generic error diffusion dithering. The first non-zero
 * element in ker is treated as the current pixel. All other non-zero
 * elements are the error diffusion coefficients.
 * Making the matrix generic is not terribly slower: the performance
 * hit is around 4% for Floyd-Steinberg and 13% for JaJuNi, with the
 * benefit of a lot less code. */
old_image old_image::dither_ediff(old_array2d<float> const &ker, ScanMode scan) const
{
    old_image dst = *this;

    ivec2 isize = dst.size();
    ivec2 ksize = ker.sizes();

    int kx;
    for (kx = 0; kx < ksize.x; kx++)
        if (ker[kx][0] > 0.f)
            break;

    float *pixels = dst.lock<PixelFormat::Y_F32>();
    for (int y = 0; y < isize.y; y++)
    {
        bool reverse = (y & 1) && (scan == ScanMode::Serpentine);

        for (int x = 0; x < isize.x; x++)
        {
            int x2 = reverse ? isize.x - 1 - x : x;
            int s = reverse ? -1 : 1;

            float p = pixels[y * isize.x + x2];
            float q = p < 0.5f ? 0.f : 1.f;
            pixels[y * isize.x + x2] = q;

            float e = (p - q);

            for (int j = 0; j < ksize.y && y < isize.y - j; j++)
                for (int i = 0; i < ksize.x; i++)
                {
                    if (j == 0 && i <= kx)
                        continue;

                    if (x + i - kx < 0 || x + i - kx >= isize.x)
                        continue;

                    pixels[(y + j) * isize.x + x2 + (i - kx) * s]
                       += e * ker[i][j];
                }
        }
    }
    dst.unlock(pixels);

    return dst;
}

} /* namespace lol */

