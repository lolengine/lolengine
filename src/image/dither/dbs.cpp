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
 * Direct Binary Search dithering
 */

#define CELL 16

#define N 7
#define NN ((N * 2 + 1))

namespace lol
{

/* FIXME: though the algorithm is supposed to stop, we do not have a real,
 * guaranteed stop condition here. */
Image Image::DitherDbs() const
{
    ivec2 size = GetSize();

    /* Build our human visual system kernel. */
    Array2D<float> kernel;
    kernel.SetSize(ivec2(NN, NN));
    float t = 0.f;
    for (int j = 0; j < NN; j++)
        for (int i = 0; i < NN; i++)
        {
            vec2 v = vec2(i - N, j - N);
            kernel[i][j] = exp(-sqlength(v / 1.6f) / 2.f)
                         + exp(-sqlength(v / 0.6f) / 2.f);
            t += kernel[i][j];
        }

    for (int j = 0; j < NN; j++)
        for (int i = 0; i < NN; i++)
            kernel[i][j] /= t;

    /* A list of cells in our picture. If no change is done to a cell
     * for two iterations, we stop considering changes to it. */
    ivec2 csize = (size + ivec2(CELL - 1)) / CELL;
    Array2D<int> changelist;
    changelist.SetSize(csize);
    memset(changelist.Data(), 0, changelist.Bytes());

    Image dst = *this;
    dst.SetFormat(PixelFormat::Y_F32);

    Image tmp1 = dst.Convolution(kernel);
    float *tmp1data = tmp1.Lock<PixelFormat::Y_F32>();

    dst = dst.DitherRandom();
    float *dstdata = dst.Lock<PixelFormat::Y_F32>();

    Image tmp2 = dst.Convolution(kernel);
    float *tmp2data = tmp2.Lock<PixelFormat::Y_F32>();

    for (;;)
    {
        int allchanges = 0;

        for (int cy = 0; cy < csize.y; ++cy)
        for (int cx = 0; cx < csize.x; ++cx)
        {
            int changes = 0;

            if (changelist[cx][cy] >= 2)
                continue;

            for (int y = cy * CELL; y < (cy + 1) * CELL; ++y)
            for (int x = cx * CELL; x < (cx + 1) * CELL; ++x)
            {
                int opx = -1, opy = -1;

                float d = dstdata[y * size.x + x];
                float d2;

                /* Compute the effect of a toggle */
                float e = 0.f, best = 0.f;
                for (int j = -N; j < N + 1; j++)
                {
                    if (y + j < 0 || y + j >= size.y)
                        continue;

                    for (int i = -N; i < N + 1; i++)
                    {
                        if (x + i < 0 || x + i >= size.x)
                            continue;

                        float m = kernel[i + N][j + N];
                        float p = tmp1data[(y + j) * size.x + x + i];
                        float q1 = tmp2data[(y + j) * size.x + x + i];
                        float q2 = q1 - m * d + m * (1. - d);
                        e += (q1 - p) * (q1 - p) - (q2 - p) * (q2 - p);
                    }
                }

                if (e > best)
                {
                    best = e;
                    opx = opy = 0;
                }

                /* Compute the effect of swaps */
                for (int n = 0; n < 8; n++)
                {
                    static int const step[] =
                      { 0, 1, 0, -1, -1, 0, 1, 0, -1, -1, -1, 1, 1, -1, 1, 1 };
                    int idx = step[n * 2], idy = step[n * 2 + 1];
                    if (y + idy < 0 || y + idy >= size.y
                         || x + idx < 0 || x + idx >= size.x)
                        continue;
                    d2 = dstdata[(y + idy) * size.x + x + idx];
                    if (d2 == d)
                        continue;
                    e = 0.;
                    for (int j = -N; j < N + 1; j++)
                    {
                        if (y + j < 0 || y + j >= size.y)
                            continue;
                        if (j - idy + N < 0 || j - idy + N >= NN)
                            continue;
                        for (int i = -N; i < N + 1; i++)
                        {
                            if (x + i < 0 || x + i >= size.x)
                                continue;
                            if (i - idx + N < 0 || i - idx + N >= NN)
                                continue;
                            float ma = kernel[i + N][j + N];
                            float mb = kernel[i - idx + N][j - idy + N];
                            float p = tmp1data[(y + j) * size.x + x + i];
                            float q1 = tmp2data[(y + j) * size.x + x + i];
                            float q2 = q1 - ma * d + ma * d2 - mb * d2 + mb * d;
                            e += (q1 - p) * (q1 - p) - (q2 - p) * (q2 - p);
                        }
                    }

                    if (e > best)
                    {
                        best = e;
                        opx = idx;
                        opy = idy;
                    }
                }

                /* Apply the change if interesting */
                if (best <= 0.f)
                    continue;

                if (opx || opy)
                {
                    d2 = dstdata[(y + opy) * size.x + x + opx];
                    dstdata[(y + opy) * size.x + x + opx] = d;
                }
                else
                    d2 = 1. - d;
                dstdata[y * size.x + x] = d2;

                for (int j = -N; j < N + 1; j++)
                for (int i = -N; i < N + 1; i++)
                {
                    float m = kernel[i + N][j + N];
                    if (y + j >= 0 && y + j < size.y
                         && x + i >= 0 && x + i < size.x)
                    {
                        t = tmp2data[(y + j) * size.x + x + i];
                        tmp2data[(y + j) * size.x + x + i] = t + m * (d2 - d);
                    }
                    if ((opx || opy) && y + opy + j >= 0 && y + opy + j < size.y
                                    && x + opx + i >= 0 && x + opx + i < size.x)
                    {
                        t = tmp2data[(y + opy + j) * size.x + x + opx + i];
                        tmp2data[(y + opy + j) * size.x + x + opx + i]
                                = t + m * (d - d2);
                    }
                }

                changes++;
            }

            if (changes == 0)
                ++changelist[cx][cy];

            allchanges += changes;
        }

        if (allchanges == 0)
            break;
    }

    tmp1.Unlock(tmp1data);
    tmp2.Unlock(tmp2data);
    dst.Unlock(dstdata);

    return dst;
}

} /* namespace lol */

