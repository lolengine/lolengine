//
// Lol Engine
//
// Copyright: (c) 2004-2015 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

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
    array2d<float> kernel;
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
    ivec2 const csize = (size + ivec2(CELL - 1)) / CELL;
    array2d<int> changelist(csize);
    memset(changelist.Data(), 0, changelist.Bytes());

    Image dst = *this;
    dst.SetFormat(PixelFormat::Y_F32);

    Image tmp1 = dst.Convolution(kernel);
    array2d<float> &tmp1data = tmp1.Lock2D<PixelFormat::Y_F32>();

    dst = dst.DitherRandom();
    array2d<float> &dstdata = dst.Lock2D<PixelFormat::Y_F32>();

    Image tmp2 = dst.Convolution(kernel);
    array2d<float> &tmp2data = tmp2.Lock2D<PixelFormat::Y_F32>();

    for (int run = 0, last_change = 0; ; ++run)
    {
        int const cell = run % (csize.x * csize.y);
        int const cx = cell % csize.x;
        int const cy = cell / csize.x;

        /* Bail out if no change was done for the last full image run */
        if (run > last_change + csize.x * csize.y)
            break;

        /* Skip cell if it was already ignored twice */
        if (changelist[cx][cy] >= 2)
            continue;

        int changes = 0;

        for (int pixel = 0; pixel < CELL * CELL; ++pixel)
        {
            ivec2 const pos(cx * CELL + pixel % CELL,
                            cy * CELL + pixel / CELL);

            if (!(pos >= ivec2(0)) || !(pos < size))
                continue;

            /* The best operation we can do */
            ivec2 best_op(0);
            float best_error = 0.f;

            float d = dstdata[pos];

            /* Compute the effect of all possible toggle and swaps */
            static ivec2 const op_list[] =
            {
                { 0, 0 },
                { 0, 1 },   { 0, -1 }, { -1, 0 }, { 1, 0 },
                { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 },
            };

            for (ivec2 const op : op_list)
            {
                if (!(pos + op >= ivec2(0)) || !(pos + op < size))
                    continue;

                bool flip = (op == ivec2(0));

                float d2 = flip ? 1 - d : dstdata[pos + op];

                if (!flip && d2 == d)
                    continue;

                /* TODO: implement min/max for 3+ arguments */
                int imin = max(max(-N, op.x - N), -pos.x);
                int imax = min(min(N + 1, op.x + NN - N), size.x - pos.x);
                int jmin = max(max(-N, op.y - N), -pos.y);
                int jmax = min(min(N + 1, op.y + NN - N), size.y - pos.y);

                float error = 0.f;
                for (int j = jmin; j < jmax; j++)
                for (int i = imin; i < imax; i++)
                {
                    ivec2 pos2 = pos + ivec2(i, j);

                    float m = kernel[i + N][j + N];
                    if (!flip)
                        m -= kernel[i - op.x + N][j - op.y + N];
                    float p = tmp1data[pos2];
                    float q1 = tmp2data[pos2];
                    float q2 = q1 + m * (d2 - d);
                    error += sq(q1 - p) - sq(q2 - p);
                }

                if (error > best_error)
                {
                    best_error = error;
                    best_op = op;
                }
            }

            /* Only apply the change if interesting */
            if (best_error > 0.f)
            {
                bool flip = (best_op == ivec2(0));

                float d2 = flip ? 1 - d : dstdata[pos + best_op];
                dstdata[pos + best_op] = d;
                dstdata[pos] = d2;

                for (int j = -N; j <= N; j++)
                for (int i = -N; i <= N; i++)
                {
                    ivec2 off(i, j);
                    float delta = (d2 - d) * kernel[i + N][j + N];

                    if (pos + off >= ivec2(0) && pos + off < size)
                        tmp2data[pos + off] += delta;

                    if (!flip && pos + off + best_op >= ivec2(0)
                         && pos + off + best_op < size)
                        tmp2data[pos + off + best_op] -= delta;
                }

                ++changes;
                last_change = run;
            }
        }

        if (changes == 0)
            ++changelist[cx][cy];
    }

    tmp1.Unlock2D(tmp1data);
    tmp2.Unlock2D(tmp2data);
    dst.Unlock2D(dstdata);

    return dst;
}

} /* namespace lol */

