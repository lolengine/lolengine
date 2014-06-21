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
 * Stock images and kernels
 */

namespace lol
{

bool Image::Stock(char const *name)
{
    /* Generate a completely random image. */
    if (!strncmp(name, "random:", 7))
    {
        ivec2 size(0);

        size.x = atoi(name + 7);
        name = strchr(name + 7, 'x');
        if (name)
            size.y = atoi(name + 1);
        if (!size.y)
            size.y = size.x;
        if (size.x <= 0 || size.y <= 0)
            return false;

        return RenderRandom(size);
    }

    return false;
}

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

    float epsilon = 1.f / (size.x * size.y + 1);
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

} /* namespace lol */

