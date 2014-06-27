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

Array2D<float> Image::BayerKernel(ivec2 size)
{
    Array2D<float> ret(size);

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

Array2D<float> Image::HalftoneKernel(ivec2 size)
{
    Array2D<float> ret(size);

    for (int y = 0; y < size.y; y++)
        for (int x = 0; x < size.x; x++)
        {
            float dx = 2.f * x / size.x - 0.5f;
            float dy = 2.f * (y + 0.07f) / size.y - 0.5f;
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

Array2D<float> Image::BlueNoiseKernel(ivec2 size)
{
    /* FIXME: this function could be faster if we didn't do the convolution
     * each time and instead work on the convoluted matrix. */
    Array2D<float> ret(size);
    float const epsilon = 1.f / (size.x * size.y + 1);

    /* Create a small Gaussian kernel for filtering */
    ivec2 const gsize = ivec2(9, 9);
    Array2D<float> gaussian(gsize);
    for (int j = 0; j < gsize.y; ++j)
    for (int i = 0; i < gsize.x; ++i)
    {
        ivec2 const distance = gsize / 2 - ivec2(i, j);
        gaussian[i][j] = lol::exp(-lol::sqlength(distance)
                                    / (0.05f * gsize.x * gsize.y));
    }

    /* Helper function to find voids and clusters */
    auto best = [&] (Array2D<float> const &p, float val, float mul) -> ivec2
    {
        float maxval = -size.x * size.y;
        ivec2 coord(0, 0);
        for (int y = 0; y < size.y; ++y)
        for (int x = 0; x < size.x; ++x)
        {
            if (p[x][y] != val)
                continue;

            float total = 0.0f;
            for (int j = 0; j < gsize.y; ++j)
            for (int i = 0; i < gsize.x; ++i)
                total += gaussian[i][j] *
                         p[(x + i - gsize.x / 2 + size.x) % size.x]
                          [(y + j - gsize.y / 2 + size.y) % size.y];
            if (total * mul > maxval)
            {
                maxval = total * mul;
                coord = ivec2(x, y);
            }
        }

        return coord;
    };

    /* Generate an array with about 10% random dots */
    Array2D<float> dots(size);
    int const ndots = (size.x * size.y + 9) / 10;
    memset(dots.Data(), 0, dots.Bytes());
    for (int n = 0; n < ndots; )
    {
        int x = lol::rand(size.x);
        int y = lol::rand(size.y);
        if (dots[x][y])
            continue;
        dots[x][y] = 1.0f;
        ++n;
    }

    /* Rearrange 1s so that they occupy the largest voids */
    for (;;)
    {
        ivec2 bestcluster = best(dots, 1.0f, 1.0f);
        dots[bestcluster] = 0.0f;
        ivec2 bestvoid = best(dots, 0.0f, -1.0f);
        dots[bestvoid] = 1.0f;
        if (bestcluster == bestvoid)
            break;
    }

    /* Reorder all 1s and replace them with 0.0001 */
    for (int n = ndots; n--; )
    {
        ivec2 bestcluster = best(dots, 1.0f, 1.0f);
        ret[bestcluster] = (n + 1.0f) * epsilon;
        dots[bestcluster] = 0.0001f;
    }

    /* Reorder all 0s and replace them with 0.0001 */
    for (int n = ndots; n < size.x * size.y; ++n)
    {
        ivec2 bestvoid = best(dots, 0.0f, -1.0f);
        ret[bestvoid] = (n + 1.0f) * epsilon;
        dots[bestvoid] = 0.0001f;
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

Array2D<float> Image::NormalizeKernel(Array2D<float> const &kernel)
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

    Array2D<float> dst(size);

    float const epsilon = 1.f / (size.x * size.y + 1);
    for (int n = 0; n < size.x * size.y; n++)
    {
        int x = tmp[n].x;
        int y = tmp[n].y;
        dst[x][y] = (float)(n + 1) * epsilon;
    }

    return dst;
}

Array2D<float> Image::EdiffKernel(EdiffAlgorithm algorithm)
{
    Array2D<float> ret;

    switch(algorithm)
    {
    case EdiffAlgorithm::FloydSteinberg:
        ret.SetSize(ivec2(3, 2));
        {
            static float const myker[] =
            {
                   0.,     1.,  7./16,
                3./16,  5./16,  1./16,
            };
            memcpy(&ret[0][0], myker, sizeof(myker));
        }
        return ret;
    case EdiffAlgorithm::JaJuNi:
        ret.SetSize(ivec2(5, 3));
        {
            static float const myker[] =
            {
                   0.,     0.,     1.,  7./48,  5./48,
                3./48,  5./48,  7./48,  5./48,  3./48,
                1./48,  3./48,  5./48,  3./48,  1./48,
            };
            memcpy(&ret[0][0], myker, sizeof(myker));
        }
        return ret;
    case EdiffAlgorithm::Atkinson:
        ret.SetSize(ivec2(4, 3));
        {
            static float const myker[] =
            {
                  0.,    1.,  1./8,  1./8,
                1./8,  1./8,  1./8,    0.,
                  0.,  1./8,    0.,    0.,
            };
            memcpy(&ret[0][0], myker, sizeof(myker));
        }
        return ret;
    case EdiffAlgorithm::Fan:
        ret.SetSize(ivec2(4, 2));
        {
            static float const myker[] =
            {
                   0.,     0.,     1.,  7./16,
                1./16,  3./16,  5./16,     0.,
            };
            memcpy(&ret[0][0], myker, sizeof(myker));
        }
        return ret;
    case EdiffAlgorithm::ShiauFan:
        ret.SetSize(ivec2(4, 2));
        {
            static float const myker[] =
            {
                  0.,    0.,    1.,  1./2,
                1./8,  1./8,  1./4,    0.,
            };
            memcpy(&ret[0][0], myker, sizeof(myker));
        }
        return ret;
    case EdiffAlgorithm::ShiauFan2:
        ret.SetSize(ivec2(5, 2));
        {
            static float const myker[] =
            {
                   0.,     0.,    0.,    1.,  1./2,
                1./16,  1./16,  1./8,  1./4,    0.,
            };
            memcpy(&ret[0][0], myker, sizeof(myker));
        }
        return ret;
    case EdiffAlgorithm::Stucki:
        ret.SetSize(ivec2(5, 3));
        {
            static float const myker[] =
            {
                   0.,     0.,     1.,  8./42,  4./42,
                2./42,  4./42,  8./42,  4./42,  2./42,
                1./42,  2./42,  4./42,  2./42,  1./42,
            };
            memcpy(&ret[0][0], myker, sizeof(myker));
        }
        return ret;
    case EdiffAlgorithm::Burkes:
        ret.SetSize(ivec2(5, 2));
        {
            static float const myker[] =
            {
                   0.,     0.,     1.,  4./16,  2./16,
                1./16,  2./16,  4./16,  2./16,  1./16,
            };
            memcpy(&ret[0][0], myker, sizeof(myker));
        }
        return ret;
    case EdiffAlgorithm::Sierra:
        ret.SetSize(ivec2(5, 3));
        {
            static float const myker[] =
            {
                   0.,     0.,     1.,  5./32,  3./32,
                2./32,  4./32,  5./32,  4./32,  2./32,
                   0.,  2./32,  3./32,  2./32,     0.,
            };
            memcpy(&ret[0][0], myker, sizeof(myker));
        }
        return ret;
    case EdiffAlgorithm::Sierra2:
        ret.SetSize(ivec2(5, 2));
        {
            static float const myker[] =
            {
                   0.,     0.,     1.,  4./16,  3./16,
                1./16,  2./16,  3./16,  2./16,  1./16,
            };
            memcpy(&ret[0][0], myker, sizeof(myker));
        }
        return ret;
    case EdiffAlgorithm::Lite:
        ret.SetSize(ivec2(3, 2));
        {
            static float const myker[] =
            {
                  0.,    1.,  1./2,
                1./4,  1./4,    0.,
            };
            memcpy(&ret[0][0], myker, sizeof(myker));
        }
        return ret;
    }

    return ret;
}

/* Any standard deviation below this value will be rounded up, in order
 * to avoid ridiculously low values. exp(-1/(2*0.2*0.2)) is < 10^-5 so
 * there is little chance that any value below 0.2 will be useful. */
#define BLUR_EPSILON 0.2f

Array2D<float> Image::GaussianKernel(vec2 radius, float angle, vec2 delta)
{
    Array2D<float> kernel;

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

