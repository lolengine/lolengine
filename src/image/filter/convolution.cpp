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
 * Generic convolution functions
 */

namespace lol
{

static Image SepConv(Image &src, Array<float> const &hvec,
                     Array<float> const &vvec);
static Image NonSepConv(Image &src, Array2D<float> const &kernel);

Image Image::Convolution(Array2D<float> const &kernel)
{
    /* Find the cell with the largest value */
    ivec2 ksize = kernel.GetSize();
    int besti = -1, bestj = -1;
    float tmp = 0.f;
    for (int j = 0; j < ksize.y; ++j)
        for (int i = 0; i < ksize.x; ++i)
            if (lol::sq(kernel[i][j] > tmp))
            {
                tmp = sq(kernel[i][j]);
                besti = i;
                bestj = j;
            }

    /* If the kernel is empty, return a copy of the picture */
    if (tmp == 0.f)
        return *this;

    /* Check whether the matrix rank is 1 */
    bool separable = true;
    for (int j = 0; j < ksize.y && separable; ++j)
    {
        if (j == bestj)
            continue;

        for (int i = 0; i < ksize.x && separable; ++i)
        {
            if (i == besti)
                continue;

            float p = kernel[i][j] * kernel[besti][bestj];
            float q = kernel[i][bestj] * kernel[besti][j];

            if (lol::abs(p - q) > 1.0e-8f)
                separable = false;
        }
    }

    if (separable)
    {
        /* Matrix rank is 1! Separate the filter. */
        Array<float> hvec, vvec;

        float norm = 1.0f / lol::sqrt(lol::abs(kernel[besti][bestj]));
        for (int i = 0; i < ksize.x; i++)
            hvec << norm * kernel[i][bestj];
        for (int j = 0; j < ksize.y; j++)
            vvec << norm * kernel[besti][j];

        return SepConv(*this, hvec, vvec);
    }
    else
    {
        return NonSepConv(*this, kernel);
    }
}

static Image NonSepConv(Image &src, Array2D<float> const &kernel)
{
    ivec2 const size = src.GetSize();
    ivec2 const ksize = kernel.GetSize();
    Image dst(size);

    bool const wrap_x = src.GetWrapX() == WrapMode::Repeat;
    bool const wrap_y = src.GetWrapY() == WrapMode::Repeat;

    if (src.GetFormat() == PixelFormat::Y_8
         || src.GetFormat() == PixelFormat::Y_F32)
    {
        float const *srcp = src.Lock<PixelFormat::Y_F32>();
        float *dstp = dst.Lock<PixelFormat::Y_F32>();

        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                float pixel = 0.f;

                for (int j = 0; j < ksize.y; j++)
                {
                    int y2 = y + j - ksize.y / 2;
                    if (y2 < 0)
                        y2 = wrap_y ? size.y - 1 - ((-y2 - 1) % size.y) : 0;
                    else if (y2 >= size.y)
                        y2 = wrap_y ? y2 % size.y : size.y - 1;

                    for (int i = 0; i < ksize.x; i++)
                    {
                        float f = kernel[i][j];

                        int x2 = x + i - ksize.x / 2;
                        if (x2 < 0)
                            x2 = wrap_x ? size.x - 1 - ((-x2 - 1) % size.x) : 0;
                        else if (x2 >= size.x)
                            x2 = wrap_x ? x2 % size.x : size.x - 1;

                        pixel += f * srcp[y2 * size.x + x2];
                    }
                }

                dstp[y * size.x + x] = lol::clamp(pixel, 0.0f, 1.0f);
            }
        }

        src.Unlock(srcp);
        dst.Unlock(dstp);
    }
    else
    {
        vec4 const *srcp = src.Lock<PixelFormat::RGBA_F32>();
        vec4 *dstp = dst.Lock<PixelFormat::RGBA_F32>();

        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                vec4 pixel(0.f);

                for (int j = 0; j < ksize.y; j++)
                {
                    int y2 = y + j - ksize.y / 2;
                    if (y2 < 0)
                        y2 = wrap_y ? size.y - 1 - ((-y2 - 1) % size.y) : 0;
                    else if (y2 >= size.y)
                        y2 = wrap_y ? y2 % size.y : size.y - 1;

                    for (int i = 0; i < ksize.x; i++)
                    {
                        float f = kernel[i][j];

                        int x2 = x + i - ksize.x / 2;
                        if (x2 < 0)
                            x2 = wrap_x ? size.x - 1 - ((-x2 - 1) % size.x) : 0;
                        else if (x2 >= size.x)
                            x2 = wrap_x ? x2 % size.x : size.x - 1;

                        pixel += f * srcp[y2 * size.x + x2];
                    }
                }

                dstp[y * size.x + x] = lol::clamp(pixel, 0.0f, 1.0f);
            }
        }

        src.Unlock(srcp);
        dst.Unlock(dstp);
    }

    return dst;
}

static Image SepConv(Image &src, Array<float> const &hvec,
                     Array<float> const &vvec)
{
    ivec2 const size = src.GetSize();
    ivec2 const ksize = ivec2(hvec.Count(), vvec.Count());
    Image dst(size);

    bool const wrap_x = src.GetWrapX() == WrapMode::Repeat;
    bool const wrap_y = src.GetWrapY() == WrapMode::Repeat;

    if (src.GetFormat() == PixelFormat::Y_8
         || src.GetFormat() == PixelFormat::Y_F32)
    {
        float const *srcp = src.Lock<PixelFormat::Y_F32>();
        float *dstp = dst.Lock<PixelFormat::Y_F32>();
        /* TODO: compare performances with Array2D here */
        float *tmp = new float[size.x * size.y];

        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                float pixel = 0.f;

                for (int i = 0; i < ksize.x; i++)
                {
                    float f = hvec[i];

                    int x2 = x + i - ksize.x / 2;
                    if (x2 < 0)
                        x2 = wrap_x ? size.x - 1 - ((-x2 - 1) % size.x) : 0;
                    else if (x2 >= size.x)
                        x2 = wrap_x ? x2 % size.x : size.x - 1;

                    pixel += f * srcp[y * size.x + x2];
                }

                tmp[y * size.x + x] = pixel;
            }
        }

        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                float pixel = 0.f;

                for (int j = 0; j < ksize.y; j++)
                {
                    double f = vvec[j];

                    int y2 = y + j - ksize.y / 2;
                    if (y2 < 0)
                        y2 = wrap_y ? size.y - 1 - ((-y2 - 1) % size.y) : 0;
                    else if (y2 >= size.y)
                        y2 = wrap_y ? y2 % size.y : size.y - 1;

                    pixel += f * tmp[y2 * size.x + x];
                }

                dstp[y * size.x + x] = lol::clamp(pixel, 0.0f, 1.0f);
            }
        }

        src.Unlock(srcp);
        dst.Unlock(dstp);
    }
    else
    {
        vec4 const *srcp = src.Lock<PixelFormat::RGBA_F32>();
        vec4 *dstp = dst.Lock<PixelFormat::RGBA_F32>();
        /* TODO: compare performances with Array2D here */
        vec4 *tmp = new vec4[size.x * size.y];

        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                vec4 pixel(0.f);

                for (int i = 0; i < ksize.x; i++)
                {
                    int x2 = x + i - ksize.x / 2;
                    if (x2 < 0)
                        x2 = wrap_x ? size.x - 1 - ((-x2 - 1) % size.x) : 0;
                    else if (x2 >= size.x)
                        x2 = wrap_x ? x2 % size.x : size.x - 1;

                    pixel += hvec[i] * srcp[y * size.x + x2];
                }

                tmp[y * size.x + x] = pixel;
            }
        }

        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                vec4 pixel(0.f);

                for (int j = 0; j < ksize.y; j++)
                {
                    int y2 = y + j - ksize.y / 2;
                    if (y2 < 0)
                        y2 = wrap_y ? size.y - 1 - ((-y2 - 1) % size.y) : 0;
                    else if (y2 >= size.y)
                        y2 = wrap_y ? y2 % size.y : size.y - 1;

                    pixel += vvec[j] * tmp[y2 * size.x + x];
                }

                dstp[y * size.x + x] = lol::clamp(pixel, 0.0f, 1.0f);
            }
        }

        src.Unlock(srcp);
        dst.Unlock(dstp);
    }
    return dst;
}

} /* namespace lol */

