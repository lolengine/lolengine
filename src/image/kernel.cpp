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
 * Stock kernels
 */

namespace lol
{

array2d<float> Image::BayerKernel(ivec2 size)
{
    array2d<float> ret(size);

    int n = 1;
    while (n < size.x || n < size.y)
        n *= 2;

    for (int j = 0; j < size.y; j++)
        for (int i = 0; i < size.x; i++)
        {
            int x = 0;

            for (int k = 1, l = n * n / 4; k < n; k *= 2, l /= 4)
            {
                if ((i & k) && (j & k))
                    x += l;
                else if (i & k)
                    x += 3 * l;
                else if (j & k)
                    x += 2 * l;
            }

            ret[i][j] = (float)(x + 1) / (n * n + 1);
        }

    return ret;
}

array2d<float> Image::HalftoneKernel(ivec2 size)
{
    array2d<float> ret(size);

    for (int y = 0; y < size.y; y++)
        for (int x = 0; x < size.x; x++)
        {
            float dx = 2.f * (x + 0.02f) / size.x - 0.5f;
            float dy = 2.f * (y + 0.03f) / size.y - 0.5f;
            bool flip = false;
            if (dx > 0.5f)
            {
                flip = !flip;
                dx -= 1.0f;
            }
            if (dy > 0.5f)
            {
                flip = !flip;
                dy -= 1.0f;
            }
            /* Using dx²+dy² here creates another interesting halftone. */
            float r = - lol::cos(F_PI * (dx - dy)) - lol::cos(F_PI * (dx + dy));

            ret[x][y] = flip ? 10.f - r : r;
        }

    return NormalizeKernel(ret);
}

array2d<float> Image::BlueNoiseKernel(ivec2 size, ivec2 gsize)
{
    float const epsilon = 1.f / (size.x * size.y + 1);
    gsize = lol::min(size, gsize);

    array2d<float> ret(size);
    array2d<vec2> dots(size);

    /* Create a small Gaussian kernel for filtering */
    array2d<float> gaussian(gsize);
    for (int j = 0; j < gsize.y; ++j)
    for (int i = 0; i < gsize.x; ++i)
    {
        ivec2 const distance = gsize / 2 - ivec2(i, j);
        gaussian[i][j] = lol::exp(-lol::sqlength(distance)
                                    / (0.05f * gsize.x * gsize.y));
    }

    /* Helper function to find voids and clusters */
    auto setdot = [&] (ivec2 pos, float val)
    {
        float const delta = val - dots[pos][0];
        dots[pos][0] = val;

        for (int j = 0; j < gsize.y; ++j)
        for (int i = 0; i < gsize.x; ++i)
            dots[(pos.x + i - gsize.x / 2 + size.x) % size.x]
                [(pos.y + j - gsize.y / 2 + size.y) % size.y]
                [1] += gaussian[i][j] * delta;
    };

    auto best = [&] (float val, float mul) -> ivec2
    {
        float maxval = -size.x * size.y;
        ivec2 coord(0, 0);
        for (int y = 0; y < size.y; ++y)
        for (int x = 0; x < size.x; ++x)
        {
            if (dots[x][y][0] != val)
                continue;

            float total = dots[x][y][1];
            if (total * mul > maxval)
            {
                maxval = total * mul;
                coord = ivec2(x, y);
            }
        }

        return coord;
    };

    /* Generate an array with about 10% random dots */
    int const ndots = (size.x * size.y + 9) / 10;
    memset(dots.Data(), 0, dots.Bytes());
    for (int n = 0; n < ndots; )
    {
        ivec2 pos(lol::rand(size.x), lol::rand(size.y));
        if (dots[pos][0])
            continue;
        setdot(ivec2(pos), 1.0f);
        ++n;
    }

    /* Rearrange 1s so that they occupy the largest voids */
    for (;;)
    {
        ivec2 bestcluster = best(1.0f, 1.0f);
        setdot(bestcluster, 0.0f);
        ivec2 bestvoid = best(0.0f, -1.0f);
        setdot(bestvoid, 1.0f);
        if (bestcluster == bestvoid)
            break;
    }

    /* Reorder all 1s and replace them with 0.0001 */
    for (int n = ndots; n--; )
    {
        ivec2 bestcluster = best(1.0f, 1.0f);
        ret[bestcluster] = (n + 1.0f) * epsilon;
        setdot(bestcluster, 0.0001f);
    }

    /* Reorder all 0s and replace them with 0.0001 */
    for (int n = ndots; n < size.x * size.y; ++n)
    {
        ivec2 bestvoid = best(0.0f, -1.0f);
        ret[bestvoid] = (n + 1.0f) * epsilon;
        setdot(bestvoid, 0.0001f);
    }

    return ret;
}

struct Dot
{
    int x, y;
    float val;
};

static int cmpdot(const void *p1, const void *p2)
{
    return ((Dot const *)p1)->val > ((Dot const *)p2)->val;
}

array2d<float> Image::NormalizeKernel(array2d<float> const &kernel)
{
    ivec2 size = kernel.GetSize();

    Array<Dot> tmp;
    tmp.Resize(size.x * size.y);

    for (int y = 0; y < size.y; y++)
        for (int x = 0; x < size.x; x++)
        {
            tmp[y * size.x + x].x = x;
            tmp[y * size.x + x].y = y;
            tmp[y * size.x + x].val = kernel[x][y];
        }
    std::qsort(tmp.Data(), size.x * size.y, sizeof(Dot), cmpdot);

    array2d<float> dst(size);

    float const epsilon = 1.f / (size.x * size.y + 1);
    for (int n = 0; n < size.x * size.y; n++)
    {
        int x = tmp[n].x;
        int y = tmp[n].y;
        dst[x][y] = (n + 1.f) * epsilon;
    }

    return dst;
}

array2d<float> Image::EdiffKernel(EdiffAlgorithm algorithm)
{
    switch (algorithm)
    {
    case EdiffAlgorithm::FloydSteinberg:
        return { {    0.f,     1.f,  7.f/16, },
                 { 3.f/16,  5.f/16,  1.f/16, }, };

    case EdiffAlgorithm::JaJuNi:
        return { {    0.f,     0.f,     1.f,  7.f/48,  5.f/48, },
                 { 3.f/48,  5.f/48,  7.f/48,  5.f/48,  3.f/48, },
                 { 1.f/48,  3.f/48,  5.f/48,  3.f/48,  1.f/48, }, };

    case EdiffAlgorithm::Atkinson:
        return { {   0.f,    1.f,  1.f/8,  1.f/8, },
                 { 1.f/8,  1.f/8,  1.f/8,    0.f, },
                 {   0.f,  1.f/8,    0.f,    0.f, }, };

    case EdiffAlgorithm::Fan:
        return { {    0.f,     0.f,     1.f,  7.f/16, },
                 { 1.f/16,  3.f/16,  5.f/16,     0.f, }, };

    case EdiffAlgorithm::ShiauFan:
        return { {   0.f,    0.f,    1.f,  1.f/2, },
                 { 1.f/8,  1.f/8,  1.f/4,    0.f, }, };

    case EdiffAlgorithm::ShiauFan2:
        return { {    0.f,     0.f,    0.f,    1.f,  1.f/2, },
                 { 1.f/16,  1.f/16,  1.f/8,  1.f/4,    0.f, }, };

    case EdiffAlgorithm::Stucki:
        return { {    0.f,     0.f,     1.f,  8.f/42,  4.f/42, },
                 { 2.f/42,  4.f/42,  8.f/42,  4.f/42,  2.f/42, },
                 { 1.f/42,  2.f/42,  4.f/42,  2.f/42,  1.f/42, }, };

    case EdiffAlgorithm::Burkes:
        return { {    0.f,     0.f,     1.f,  4.f/16,  2.f/16, },
                 { 1.f/16,  2.f/16,  4.f/16,  2.f/16,  1.f/16, }, };

    case EdiffAlgorithm::Sierra:
        return { {    0.f,     0.f,     1.f,  5.f/32,  3.f/32, },
                 { 2.f/32,  4.f/32,  5.f/32,  4.f/32,  2.f/32, },
                 {    0.f,  2.f/32,  3.f/32,  2.f/32,     0.f, }, };

    case EdiffAlgorithm::Sierra2:
        return { {    0.f,     0.f,     1.f,  4.f/16,  3.f/16, },
                 { 1.f/16,  2.f/16,  3.f/16,  2.f/16,  1.f/16, }, };

    case EdiffAlgorithm::Lite:
        return { {   0.f,    1.f,  1.f/2, },
                 { 1.f/4,  1.f/4,    0.f, }, };
    }

    return { { 1.f } };
}

/* Any standard deviation below this value will be rounded up, in order
 * to avoid ridiculously low values. exp(-1/(2*0.2*0.2)) is < 10^-5 so
 * there is little chance that any value below 0.2 will be useful. */
#define BLUR_EPSILON 0.2f

array2d<float> Image::GaussianKernel(vec2 radius, float angle, vec2 delta)
{
    array2d<float> kernel;

    if (radius.x < BLUR_EPSILON)
        radius.x = BLUR_EPSILON;
    if (radius.y < BLUR_EPSILON)
        radius.y = BLUR_EPSILON;

    float const sint = lol::sin(angle);
    float const cost = lol::cos(angle);

    /* Compute the final ellipse's bounding box */
    float const bbx = lol::sqrt(sq(radius.x * cost) + sq(radius.y * sint));
    float const bby = lol::sqrt(sq(radius.y * cost) + sq(radius.x * sint));

    /* FIXME: the kernel becomes far too big with large values of dx, because
     * we grow both left and right. Fix the growing direction. */
    int const krx = (int)(3.f * bbx + .99999f + lol::ceil(lol::abs(delta.x)));
    int const kry = (int)(3.f * bby + .99999f + lol::ceil(lol::abs(delta.y)));
    ivec2 size(2 * krx + 1, 2 * kry + 1);
    float const Kx = -1.f / (2.f * radius.x * radius.x);
    float const Ky = -1.f / (2.f * radius.y * radius.y);

    kernel.SetSize(size);

    float t = 0.f;

    for (int j = -kry; j <= kry; j++)
    {
        for (int i = -krx; i <= krx; i++)
        {
            /* FIXME: this level of interpolation sucks. We should
             * interpolate on the full NxN grid for better quality. */
            static vec3 const samples[] =
            {
                vec3( 0.0f,  0.0f, 1.0f),
                vec3(-0.4f, -0.4f, 0.8f),
                vec3(-0.3f,  0.0f, 0.9f),
                vec3(-0.4f,  0.4f, 0.8f),
                vec3( 0.0f,  0.3f, 0.9f),
                vec3( 0.4f,  0.4f, 0.8f),
                vec3( 0.3f,  0.0f, 0.9f),
                vec3( 0.4f, -0.4f, 0.8f),
                vec3( 0.0f, -0.3f, 0.9f),
            };

            float d = 0.f;

            for (auto p : samples)
            {
                float u = (i + p.x) * cost - (j + p.y) * sint + delta.x;
                float v = (i + p.x) * sint + (j + p.y) * cost + delta.y;
                float ex = Kx * u * u;
                float ey = Ky * v * v;
                d += p.z * lol::exp(ex + ey);

                /* Do not interpolate if this is a standard gaussian. */
                if (!delta.x && !delta.y && !angle)
                    break;
            }

            kernel[i + krx][j + kry] = d;
            t += d;
        }
    }

    for (int j = 0; j < size.y; j++)
        for (int i = 0; i < size.x; i++)
            kernel[i][j] *= (1.f / t);

    return kernel;
}

} /* namespace lol */

