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

image image::Median(ivec2 ksize) const
{
    ivec2 const isize = size();
    image tmp = *this;
    image ret(isize);

    if (format() == PixelFormat::Y_8 || format() == PixelFormat::Y_F32)
    {
        ivec2 const lsize = 2 * ksize + ivec2(1);
        array2d<float> list(lsize);

        float *srcp = tmp.lock<PixelFormat::Y_F32>();
        float *dstp = ret.lock<PixelFormat::Y_F32>();

        for (int y = 0; y < isize.y; y++)
        {
            for (int x = 0; x < isize.x; x++)
            {
                /* Make a list of neighbours */
                for (int j = -ksize.y; j <= ksize.y; j++)
                {
                    int y2 = y + j;
                    if (y2 < 0) y2 = isize.y - 1 - ((-y2 - 1) % isize.y);
                    else if (y2 > 0) y2 = y2 % isize.y;

                    for (int i = -ksize.x; i <= ksize.x; i++)
                    {
                        int x2 = x + i;
                        if (x2 < 0) x2 = isize.x - 1 - ((-x2 - 1) % isize.x);
                        else if (x2 > 0) x2 = x2 % isize.x;

                        list[i + ksize.x][j + ksize.y] = srcp[y2 * isize.x + x2];
                    }
                }

                /* Sort the list */
                qsort(&list[0][0], lsize.x * lsize.y, sizeof(float), cmpfloat);

                /* Store the median value */
                dstp[y * isize.x + x] = *(&list[0][0] + lsize.x * lsize.y / 2);
            }
        }

        tmp.unlock(srcp);
        ret.unlock(dstp);
    }
    else
    {
        ivec2 const lsize = 2 * ksize + ivec2(1);
        array2d<vec3> list(lsize);

        vec4 *srcp = tmp.lock<PixelFormat::RGBA_F32>();
        vec4 *dstp = ret.lock<PixelFormat::RGBA_F32>();

        for (int y = 0; y < isize.y; y++)
        {
            for (int x = 0; x < isize.x; x++)
            {
                /* Make a list of neighbours */
                for (int j = -ksize.y; j <= ksize.y; j++)
                {
                    int y2 = y + j;
                    if (y2 < 0) y2 = isize.y - 1 - ((-y2 - 1) % isize.y);
                    else if (y2 > 0) y2 = y2 % isize.y;

                    for (int i = -ksize.x; i <= ksize.x; i++)
                    {
                        int x2 = x + i;
                        if (x2 < 0) x2 = isize.x - 1 - ((-x2 - 1) % isize.x);
                        else if (x2 > 0) x2 = x2 % isize.x;

                        list[i + ksize.x][j + ksize.y] = srcp[y2 * isize.x + x2].rgb;
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
                dstp[y * isize.x + x] = vec4(median, srcp[y * isize.x + x].a);
            }
        }

        tmp.unlock(srcp);
        ret.unlock(dstp);
    }

    return ret;
}

image image::Median(array2d<float> const &ker) const
{
    ivec2 const isize = size();
    image tmp = *this;
    image ret(isize);

    /* FIXME: TODO */
#if 0
    if (format() == PixelFormat::Y_8 || format() == PixelFormat::Y_F32)
    {
    }
    else
#endif
    {
        ivec2 const ksize = ker.sizes();
        array2d<vec3> list(ksize);

        vec4 *srcp = tmp.lock<PixelFormat::RGBA_F32>();
        vec4 *dstp = ret.lock<PixelFormat::RGBA_F32>();

        for (int y = 0; y < isize.y; y++)
        {
            for (int x = 0; x < isize.x; x++)
            {
                /* Make a list of neighbours */
                for (int j = 0; j < ksize.y; j++)
                {
                    int y2 = y + j - ksize.y / 2;
                    if (y2 < 0) y2 = isize.y - 1 - ((-y2 - 1) % isize.y);
                    else if (y2 > 0) y2 = y2 % isize.y;

                    for (int i = 0; i < ksize.x; i++)
                    {
                        int x2 = x + i - ksize.x / 2;
                        if (x2 < 0) x2 = isize.x - 1 - ((-x2 - 1) % isize.x);
                        else if (x2 > 0) x2 = x2 % isize.x;

                        list[i][j] = srcp[y2 * isize.x + x2].rgb;
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
                            float d = ker[i][j] /
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
                dstp[y * isize.x + x] = vec4(median, srcp[y * isize.x + x].a);
            }
        }

        tmp.unlock(srcp);
        ret.unlock(dstp);
    }

    return ret;
}

} /* namespace lol */

