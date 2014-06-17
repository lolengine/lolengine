//
// Lol Engine
//
// Copyright: (c) 2004-2013 Sam Hocevar <sam@hocevar.net>
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
 * Stock images
 */

namespace lol
{

bool Image::Stock(char const *name)
{
    /* Generate a Bayer dithering pattern. */
    if (!strncmp(name, "bayer:", 6))
    {
        ivec2 size(0);
        size.x = atoi(name + 6);
        name = strchr(name + 6, 'x');
        if (name)
            size.y = atoi(name + 1);
        if (!size.y)
            size.y = size.x;

        return RenderBayer(size);
    }

    /* Generate a clustered dithering pattern. */
    if (!strncmp(name, "halftone:", 9))
    {
        ivec2 size(0);
        size.x = atoi(name + 9);
        name = strchr(name + 9, 'x');
        if (name)
            size.y = atoi(name + 1);
        if (!size.y)
            size.y = size.x;

        return RenderHalftone(size);
    }

    /* Generate an error diffusion matrix. */
    if (!strncmp(name, "ediff:", 6))
    {
        float const *ker = nullptr;
        ivec2 size(0);

        if (!strcmp(name + 6, "fs"))
        {
            float const myker[] =
            {
                   0.,     1.,  7./16,
                3./16,  5./16,  1./16,
            };
            ker = myker; size = ivec2(3, 2);
        }
        else if (!strcmp(name + 6, "jajuni"))
        {
            float const myker[] =
            {
                   0.,     0.,     1.,  7./48,  5./48,
                3./48,  5./48,  7./48,  5./48,  3./48,
                1./48,  3./48,  5./48,  3./48,  1./48,
            };
            ker = myker; size = ivec2(5, 3);
        }
        else if (!strcmp(name + 6, "atkinson"))
        {
            float const myker[] =
            {
                  0.,    1.,  1./8,  1./8,
                1./8,  1./8,  1./8,    0.,
                  0.,  1./8,    0.,    0.,
            };
            ker = myker; size = ivec2(4, 3);
        }
        else if (!strcmp(name + 6, "fan"))
        {
            float const myker[] =
            {
                   0.,     0.,     1.,  7./16,
                1./16,  3./16,  5./16,     0.,
            };
            ker = myker; size = ivec2(4, 2);
        }
        else if (!strcmp(name + 6, "shiaufan"))
        {
            float const myker[] =
            {
                  0.,    0.,    1.,  1./2,
                1./8,  1./8,  1./4,    0.,
            };
            ker = myker; size = ivec2(4, 2);
        }
        else if (!strcmp(name + 6, "shiaufan2"))
        {
            float const myker[] =
            {
                   0.,     0.,    0.,    1.,  1./2,
                1./16,  1./16,  1./8,  1./4,    0.,
            };
            ker = myker; size = ivec2(5, 2);
        }
        else if (!strcmp(name + 6, "stucki"))
        {
            float const myker[] =
            {
                   0.,     0.,     1.,  8./42,  4./42,
                2./42,  4./42,  8./42,  4./42,  2./42,
                1./42,  2./42,  4./42,  2./42,  1./42,
            };
            ker = myker; size = ivec2(5, 3);
        }
        else if (!strcmp(name + 6, "burkes"))
        {
            float const myker[] =
            {
                   0.,     0.,     1.,  4./16,  2./16,
                1./16,  2./16,  4./16,  2./16,  1./16,
            };
            ker = myker; size = ivec2(5, 2);
        }
        else if (!strcmp(name + 6, "sierra"))
        {
            float const myker[] =
            {
                   0.,     0.,     1.,  5./32,  3./32,
                2./32,  4./32,  5./32,  4./32,  2./32,
                   0.,  2./32,  3./32,  2./32,     0.,
            };
            ker = myker; size = ivec2(5, 3);
        }
        else if (!strcmp(name + 6, "sierra2"))
        {
            float const myker[] =
            {
                   0.,     0.,     1.,  4./16,  3./16,
                1./16,  2./16,  3./16,  2./16,  1./16,
            };
            ker = myker; size = ivec2(5, 2);
        }
        else if (!strcmp(name + 6, "lite"))
        {
            float const myker[] =
            {
                  0.,    1.,  1./2,
                1./4,  1./4,    0.,
            };
            ker = myker; size = ivec2(3, 2);
        }

        SetSize(size);
        float *pixels = Lock<PixelFormat::Y_F32>();
        memcpy(pixels, ker, size.x * size.y * sizeof(float));
        Unlock(pixels);

        return true;
    }

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

} /* namespace lol */

