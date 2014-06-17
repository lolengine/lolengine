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
Image Image::DitherEdiff(Image &kernel, ScanMode scan) const
{
    Image dst = *this;

    float *pixels = dst.Lock<PixelFormat::Y_F32>();
    int w = dst.GetSize().x;
    int h = dst.GetSize().y;

    float *kerdata = kernel.Lock<PixelFormat::Y_F32>();
    int kw = kernel.GetSize().x;
    int kh = kernel.GetSize().y;

    int kx;
    for (kx = 0; kx < kw; kx++)
        if (kerdata[kx] > 0.f)
            break;

    for (int y = 0; y < h; y++)
    {
        bool reverse = (y & 1) && (scan == ScanMode::Serpentine);

        for (int x = 0; x < w; x++)
        {
            int x2 = reverse ? w - 1 - x : x;
            int s = reverse ? -1 : 1;

            float p = pixels[y * w + x2];
            float q = p < 0.5f ? 0.f : 1.f;
            pixels[y * w + x2] = q;

            float e = (p - q);

            for (int j = 0; j < kh && y < h - j; j++)
                for (int i = 0; i < kw; i++)
                {
                    if (j == 0 && i <= kx)
                        continue;

                    if (x + i - kx < 0 || x + i - kx >= w)
                        continue;

                    pixels[(y + j) * w + x2 + (i - kx) * s]
                       += e * kerdata[j * kw + i];
                }
        }
    }

    kernel.Unlock(kerdata);
    dst.Unlock(pixels);

    return dst;
}

} /* namespace lol */

