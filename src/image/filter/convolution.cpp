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

#include <lol/main.h>

/*
 * Generic convolution functions
 */

namespace lol
{

static Image SepConv(Image &src, array<float> const &hvec,
                     array<float> const &vvec);
static Image NonSepConv(Image &src, array2d<float> const &kernel);

Image Image::Convolution(array2d<float> const &kernel)
{
    /* Find the cell with the largest value */
    ivec2 ksize = (ivec2)kernel.GetSize();
    int bestx = -1, besty = -1;
    float tmp = 0.f;
    for (int dy = 0; dy < ksize.y; ++dy)
        for (int dx = 0; dx < ksize.x; ++dx)
            if (lol::sq(kernel[dx][dy]) > tmp)
            {
                tmp = sq(kernel[dx][dy]);
                bestx = dx;
                besty = dy;
            }

    /* If the kernel is empty, return a copy of the picture */
    if (tmp == 0.f)
        return *this;

    /* Check whether the matrix rank is 1 */
    bool separable = true;
    for (int dy = 0; dy < ksize.y && separable; ++dy)
    {
        if (dy == besty)
            continue;

        for (int dx = 0; dx < ksize.x && separable; ++dx)
        {
            if (dx == bestx)
                continue;

            float p = kernel[dx][dy] * kernel[bestx][besty];
            float q = kernel[dx][besty] * kernel[bestx][dy];

            if (lol::abs(p - q) > 1.0e-8f)
                separable = false;
        }
    }

    if (separable)
    {
        /* Matrix rank is 1! Separate the filter. */
        array<float> hvec, vvec;

        float norm = 1.0f / lol::sqrt(lol::abs(kernel[bestx][besty]));
        for (int dx = 0; dx < ksize.x; dx++)
            hvec << norm * kernel[dx][besty];
        for (int dy = 0; dy < ksize.y; dy++)
            vvec << norm * kernel[bestx][dy];

        return SepConv(*this, hvec, vvec);
    }
    else
    {
        return NonSepConv(*this, kernel);
    }
}

Image Image::Sharpen(array2d<float> const &kernel)
{
    ivec2 ksize = (ivec2)kernel.GetSize();
    array2d<float> newkernel(ksize);

    for (int dy = 0; dy < ksize.y; ++dy)
        for (int dx = 0; dx < ksize.x; ++dx)
        {
            newkernel[dx][dy] = - kernel[dx][dy];
            if (dx == ksize.x / 2 && dy == ksize.y / 2)
                newkernel[dx][dy] += 2.f;
        }

    return Convolution(newkernel);
}

template<PixelFormat FORMAT, int WRAP_X, int WRAP_Y>
static Image NonSepConv(Image &src, array2d<float> const &kernel)
{
    typedef typename PixelType<FORMAT>::type pixel_t;

    ivec2 const size = src.GetSize();
    ivec2 const ksize = (ivec2)kernel.GetSize();
    Image dst(size);

    array2d<pixel_t> const &srcp = src.Lock2D<FORMAT>();
    array2d<pixel_t> &dstp = dst.Lock2D<FORMAT>();

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            pixel_t pixel(0.f);

            for (int dy = 0; dy < ksize.y; dy++)
            {
                int y2 = y + dy - ksize.y / 2;
                if (y2 < 0)
                    y2 = WRAP_Y ? size.y - 1 - ((-y2 - 1) % size.y) : 0;
                else if (y2 >= size.y)
                    y2 = WRAP_Y ? y2 % size.y : size.y - 1;

                for (int dx = 0; dx < ksize.x; dx++)
                {
                    float f = kernel[dx][dy];

                    int x2 = x + dx - ksize.x / 2;
                    if (x2 < 0)
                        x2 = WRAP_X ? size.x - 1 - ((-x2 - 1) % size.x) : 0;
                    else if (x2 >= size.x)
                        x2 = WRAP_X ? x2 % size.x : size.x - 1;

                    pixel += f * srcp[x2][y2];
                }
            }

            dstp[x][y] = lol::clamp(pixel, 0.0f, 1.0f);
        }
    }

    src.Unlock2D(srcp);
    dst.Unlock2D(dstp);

    return dst;
}

static Image NonSepConv(Image &src, array2d<float> const &kernel)
{
    bool const wrap_x = src.GetWrapX() == WrapMode::Repeat;
    bool const wrap_y = src.GetWrapY() == WrapMode::Repeat;

    if (src.GetFormat() == PixelFormat::Y_8
         || src.GetFormat() == PixelFormat::Y_F32)
    {
        if (wrap_x)
        {
            if (wrap_y)
                return NonSepConv<PixelFormat::Y_F32, 1, 1>(src, kernel);
            else
                return NonSepConv<PixelFormat::Y_F32, 1, 0>(src, kernel);
        }
        else
        {
            if (wrap_y)
                return NonSepConv<PixelFormat::Y_F32, 0, 1>(src, kernel);
            else
                return NonSepConv<PixelFormat::Y_F32, 0, 0>(src, kernel);
        }
    }
    else
    {
        if (wrap_x)
        {
            if (wrap_y)
                return NonSepConv<PixelFormat::RGBA_F32, 1, 1>(src, kernel);
            else
                return NonSepConv<PixelFormat::RGBA_F32, 1, 0>(src, kernel);
        }
        else
        {
            if (wrap_y)
                return NonSepConv<PixelFormat::RGBA_F32, 0, 1>(src, kernel);
            else
                return NonSepConv<PixelFormat::RGBA_F32, 0, 0>(src, kernel);
        }
    }
}

template<PixelFormat FORMAT, int WRAP_X, int WRAP_Y>
static Image SepConv(Image &src, array<float> const &hvec,
                     array<float> const &vvec)
{
    typedef typename PixelType<FORMAT>::type pixel_t;

    ivec2 const size = src.GetSize();
    ivec2 const ksize(hvec.Count(), vvec.Count());
    Image dst(size);

    array2d<pixel_t> const &srcp = src.Lock2D<FORMAT>();
    array2d<pixel_t> &dstp = dst.Lock2D<FORMAT>();

    array2d<pixel_t> tmp(size);

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            pixel_t pixel(0.f);

            for (int dx = 0; dx < ksize.x; dx++)
            {
                int x2 = x + dx - ksize.x / 2;
                if (x2 < 0)
                    x2 = WRAP_X ? size.x - 1 - ((-x2 - 1) % size.x) : 0;
                else if (x2 >= size.x)
                    x2 = WRAP_X ? x2 % size.x : size.x - 1;

                pixel += hvec[dx] * srcp[x2][y];
            }

            tmp[x][y] = pixel;
        }
    }

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            pixel_t pixel(0.f);

            for (int j = 0; j < ksize.y; j++)
            {
                int y2 = y + j - ksize.y / 2;
                if (y2 < 0)
                    y2 = WRAP_Y ? size.y - 1 - ((-y2 - 1) % size.y) : 0;
                else if (y2 >= size.y)
                    y2 = WRAP_Y ? y2 % size.y : size.y - 1;

                pixel += vvec[j] * tmp[x][y2];
            }

            dstp[x][y] = lol::clamp(pixel, 0.0f, 1.0f);
        }
    }

    src.Unlock2D(srcp);
    dst.Unlock2D(dstp);

    return dst;
}

static Image SepConv(Image &src, array<float> const &hvec,
                     array<float> const &vvec)
{
    bool const wrap_x = src.GetWrapX() == WrapMode::Repeat;
    bool const wrap_y = src.GetWrapY() == WrapMode::Repeat;

    if (src.GetFormat() == PixelFormat::Y_8
         || src.GetFormat() == PixelFormat::Y_F32)
    {
        if (wrap_x)
        {
            if (wrap_y)
                return SepConv<PixelFormat::Y_F32, 1, 1>(src, hvec, vvec);
            else
                return SepConv<PixelFormat::Y_F32, 1, 0>(src, hvec, vvec);
        }
        else
        {
            if (wrap_y)
                return SepConv<PixelFormat::Y_F32, 0, 1>(src, hvec, vvec);
            else
                return SepConv<PixelFormat::Y_F32, 0, 0>(src, hvec, vvec);
        }
    }
    else
    {
        if (wrap_x)
        {
            if (wrap_y)
                return SepConv<PixelFormat::RGBA_F32, 1, 1>(src, hvec, vvec);
            else
                return SepConv<PixelFormat::RGBA_F32, 1, 0>(src, hvec, vvec);
        }
        else
        {
            if (wrap_y)
                return SepConv<PixelFormat::RGBA_F32, 0, 1>(src, hvec, vvec);
            else
                return SepConv<PixelFormat::RGBA_F32, 0, 0>(src, hvec, vvec);
        }
    }
}

} /* namespace lol */

