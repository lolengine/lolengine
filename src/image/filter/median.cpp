//
// Lol Engine
//
// Copyright: (c) 2004-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

/*
 * Median filter functions
 */

/* FIXME: this is in desperate want of optimisation. Here is what could
 * be done to improve the performance:
 *  - prefetch the neighbourhood; most neighbours are the same as the
 *    previous pixels.
 *  - use a better sort algorithm; bubble sort is ridiculous
 *  - even better, use state-of-the art median selection, ie. O(3n), for
 *    most common combinations (9, 25, 49, 81). */

namespace lol
{

static int cmpfloat(void const *i1, void const *i2)
{
    float a = *(float const *)i1;
    float b = *(float const *)i2;

    return (a > b) - (a < b);
}

Image Image::Median(ivec2 ksize) const
{
    ivec2 const size = GetSize();
    Image tmp = *this;
    Image ret(size);

    if (GetFormat() == PixelFormat::Y_8 || GetFormat() == PixelFormat::Y_F32)
    {
        ivec2 const lsize = 2 * ksize + ivec2(1);
        array2d<float> list(lsize);

        float *srcp = tmp.Lock<PixelFormat::Y_F32>();
        float *dstp = ret.Lock<PixelFormat::Y_F32>();

        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                /* Make a list of neighbours */
                for (int j = -ksize.y; j <= ksize.y; j++)
                {
                    int y2 = y + j;
                    if (y2 < 0) y2 = size.y - 1 - ((-y2 - 1) % size.y);
                    else if (y2 > 0) y2 = y2 % size.y;

                    for (int i = -ksize.x; i <= ksize.x; i++)
                    {
                        int x2 = x + i;
                        if (x2 < 0) x2 = size.x - 1 - ((-x2 - 1) % size.x);
                        else if (x2 > 0) x2 = x2 % size.x;

                        list[i + ksize.x][j + ksize.y] = srcp[y2 * size.x + x2];
                    }
                }

                /* Sort the list */
                qsort(&list[0][0], lsize.x * lsize.y, sizeof(float), cmpfloat);

                /* Store the median value */
                dstp[y * size.x + x] = *(&list[0][0] + lsize.x * lsize.y / 2);
            }
        }

        tmp.Unlock(srcp);
        ret.Unlock(dstp);
    }
    else
    {
        ivec2 const lsize = 2 * ksize + ivec2(1);
        array2d<vec3> list(lsize);

        vec4 *srcp = tmp.Lock<PixelFormat::RGBA_F32>();
        vec4 *dstp = ret.Lock<PixelFormat::RGBA_F32>();

        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                /* Make a list of neighbours */
                for (int j = -ksize.y; j <= ksize.y; j++)
                {
                    int y2 = y + j;
                    if (y2 < 0) y2 = size.y - 1 - ((-y2 - 1) % size.y);
                    else if (y2 > 0) y2 = y2 % size.y;

                    for (int i = -ksize.x; i <= ksize.x; i++)
                    {
                        int x2 = x + i;
                        if (x2 < 0) x2 = size.x - 1 - ((-x2 - 1) % size.x);
                        else if (x2 > 0) x2 = x2 % size.x;

                        list[i + ksize.x][j + ksize.y] = srcp[y2 * size.x + x2].rgb;
                    }
                }

                /* Algorithm constants, empirically chosen */
                int const N = 5;
                float const K = 1.5f;

                /* Iterate using Weiszfeld’s algorithm */
                vec3 oldmed(0.f), median(0.f);
                for (int iter = 0; ; ++iter)
                {
                    oldmed = median;
                    vec3 s1(0.f);
                    float s2 = 0.f;
                    for (int j = 0; j < lsize.y; ++j)
                        for (int i = 0; i < lsize.x; ++i)
                        {
                            float d = 1.0f /
                                      (1e-10f + distance(median, list[i][j]));
                            s1 += list[i][j] * d;
                            s2 += d;
                        }
                    median = s1 / s2;

                    if (iter > 1 && iter < N)
                    {
                        median += K * (median - oldmed);
                    }

                    if (iter > 3 && distance(oldmed, median) < 1.e-5f)
                        break;
                }

                /* Store the median value */
                dstp[y * size.x + x] = vec4(median, srcp[y * size.x + x].a);
            }
        }

        tmp.Unlock(srcp);
        ret.Unlock(dstp);
    }

    return ret;
}

Image Image::Median(array2d<float> const &kernel) const
{
    ivec2 const size = GetSize();
    Image tmp = *this;
    Image ret(size);

    /* FIXME: TODO */
#if 0
    if (GetFormat() == PixelFormat::Y_8 || GetFormat() == PixelFormat::Y_F32)
    {
    }
    else
#endif
    {
        ivec2 const ksize = (ivec2)kernel.GetSize();
        array2d<vec3> list(ksize);

        vec4 *srcp = tmp.Lock<PixelFormat::RGBA_F32>();
        vec4 *dstp = ret.Lock<PixelFormat::RGBA_F32>();

        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                /* Make a list of neighbours */
                for (int j = 0; j < ksize.y; j++)
                {
                    int y2 = y + j - ksize.y / 2;
                    if (y2 < 0) y2 = size.y - 1 - ((-y2 - 1) % size.y);
                    else if (y2 > 0) y2 = y2 % size.y;

                    for (int i = 0; i < ksize.x; i++)
                    {
                        int x2 = x + i - ksize.x / 2;
                        if (x2 < 0) x2 = size.x - 1 - ((-x2 - 1) % size.x);
                        else if (x2 > 0) x2 = x2 % size.x;

                        list[i][j] = srcp[y2 * size.x + x2].rgb;
                    }
                }

                /* Algorithm constants, empirically chosen */
                int const N = 5;
                float const K = 1.5f;

                /* Iterate using Weiszfeld’s algorithm */
                vec3 oldmed(0.f), median(0.f);
                for (int iter = 0; ; ++iter)
                {
                    oldmed = median;
                    vec3 s1(0.f);
                    float s2 = 0.f;
                    for (int j = 0; j < ksize.y; ++j)
                        for (int i = 0; i < ksize.x; ++i)
                        {
                            float d = kernel[i][j] /
                                      (1e-10f + distance(median, list[i][j]));
                            s1 += list[i][j] * d;
                            s2 += d;
                        }
                    median = s1 / s2;

                    if (iter > 1 && iter < N)
                    {
                        median += K * (median - oldmed);
                    }

                    if (iter > 3 && distance(oldmed, median) < 1.e-5f)
                        break;
                }

                /* Store the median value */
                dstp[y * size.x + x] = vec4(median, srcp[y * size.x + x].a);
            }
        }

        tmp.Unlock(srcp);
        ret.Unlock(dstp);
    }

    return ret;
}

} /* namespace lol */

