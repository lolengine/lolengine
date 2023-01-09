//
//  Lol Engine
//
//  Copyright © 2004–2023 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
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
old_image old_image::dither_dbs() const
{
    ivec2 isize = size();

    /* Build our human visual system kernel. */
    old_array2d<float> ker;
    ker.resize(ivec2(NN, NN));
    float t = 0.f;
    for (int j = 0; j < NN; j++)
        for (int i = 0; i < NN; i++)
        {
            vec2 v = vec2((float)(i - N), (float)(j - N));
            ker[i][j] = exp(-sqlength(v / 1.6f) / 2.f)
                      + exp(-sqlength(v / 0.6f) / 2.f);
            t += ker[i][j];
        }

    for (int j = 0; j < NN; j++)
        for (int i = 0; i < NN; i++)
            ker[i][j] /= t;

    /* A list of cells in our picture. If no change is done to a cell
     * for two iterations, we stop considering changes to it. */
    ivec2 const csize = (isize + ivec2(CELL - 1)) / CELL;
    old_array2d<int> changelist(csize);
    memset(changelist.data(), 0, changelist.bytes());

    old_image dst = *this;
    dst.set_format(PixelFormat::Y_F32);

    old_image tmp1 = dst.Convolution(ker);
    old_array2d<float> &tmp1data = tmp1.lock2d<PixelFormat::Y_F32>();

    dst = dst.dither_random();
    old_array2d<float> &dstdata = dst.lock2d<PixelFormat::Y_F32>();

    old_image tmp2 = dst.Convolution(ker);
    old_array2d<float> &tmp2data = tmp2.lock2d<PixelFormat::Y_F32>();

    for (int run = 0, last_change = 0; ; ++run)
    {
        int const cell = run % (csize.x * csize.y);
        int const cx = cell % csize.x;
        int const cy = cell / csize.x;

        /* Bail out if no change was done for the last full old_image run */
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

            if (!(pos >= ivec2(0)) || !(pos < isize))
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

            for (ivec2 const &op : op_list)
            {
                if (!(pos + op >= ivec2(0)) || !(pos + op < isize))
                    continue;

                bool flip = (op == ivec2(0));

                float d2 = flip ? 1 - d : dstdata[pos + op];

                if (!flip && d2 == d)
                    continue;

                /* TODO: implement min/max for 3+ arguments */
                int imin = max(max(-N, op.x - N), -pos.x);
                int imax = min(min(N + 1, op.x + NN - N), isize.x - pos.x);
                int jmin = max(max(-N, op.y - N), -pos.y);
                int jmax = min(min(N + 1, op.y + NN - N), isize.y - pos.y);

                float error = 0.f;
                for (int j = jmin; j < jmax; j++)
                for (int i = imin; i < imax; i++)
                {
                    ivec2 pos2 = pos + ivec2(i, j);

                    float m = ker[i + N][j + N];
                    if (!flip)
                        m -= ker[i - op.x + N][j - op.y + N];
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
                    float delta = (d2 - d) * ker[i + N][j + N];

                    if (pos + off >= ivec2(0) && pos + off < isize)
                        tmp2data[pos + off] += delta;

                    if (!flip && pos + off + best_op >= ivec2(0)
                         && pos + off + best_op < isize)
                        tmp2data[pos + off + best_op] -= delta;
                }

                ++changes;
                last_change = run;
            }
        }

        if (changes == 0)
            ++changelist[cx][cy];
    }

    tmp1.unlock2d(tmp1data);
    tmp2.unlock2d(tmp2data);
    dst.unlock2d(dstdata);

    return dst;
}

} /* namespace lol */

