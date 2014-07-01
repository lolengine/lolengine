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
 * Dilate and erode functions
 */

/* FIXME: these functions are almost the same, try to merge them
 * somewhat efficiently. */
/* TODO: - dilate by k (Manhattan distance)
 *       - dilate by r (euclidian distance, with non-integer r) */

namespace lol
{

Image Image::Dilate()
{
    ivec2 const size = GetSize();
    Image ret(size);

    if (GetFormat() == PixelFormat::Y_8 || GetFormat() == PixelFormat::Y_F32)
    {
        float const *srcp = Lock<PixelFormat::Y_F32>();
        float *dstp = ret.Lock<PixelFormat::Y_F32>();

        for (int y = 0; y < size.y; ++y)
            for (int x = 0; x < size.x; ++x)
            {
                int y2 = lol::max(y - 1, 0);
                int x2 = lol::max(x - 1, 0);
                int y3 = lol::min(y + 1, size.y - 1);
                int x3 = lol::min(x + 1, size.x - 1);

                float t = srcp[y * size.x + x];
                t = lol::max(t, srcp[y * size.x + x2]);
                t = lol::max(t, srcp[y * size.x + x3]);
                t = lol::max(t, srcp[y2 * size.x + x]);
                t = lol::max(t, srcp[y3 * size.x + x]);
                dstp[y * size.x + x] = t;
            }

        Unlock(srcp);
        ret.Unlock(dstp);
    }
    else
    {
        vec4 const *srcp = Lock<PixelFormat::RGBA_F32>();
        vec4 *dstp = ret.Lock<PixelFormat::RGBA_F32>();

        for (int y = 0; y < size.y; ++y)
            for (int x = 0; x < size.x; ++x)
            {
                int y2 = lol::max(y - 1, 0);
                int x2 = lol::max(x - 1, 0);
                int y3 = lol::min(y + 1, size.y - 1);
                int x3 = lol::min(x + 1, size.x - 1);

                vec3 t = srcp[y * size.x + x].rgb;
                t = lol::max(t, srcp[y * size.x + x2].rgb);
                t = lol::max(t, srcp[y * size.x + x3].rgb);
                t = lol::max(t, srcp[y2 * size.x + x].rgb);
                t = lol::max(t, srcp[y3 * size.x + x].rgb);
                dstp[y * size.x + x] = vec4(t, srcp[y * size.x + x].a);
            }

        Unlock(srcp);
        ret.Unlock(dstp);
    }

    return ret;
}

Image Image::Erode()
{
    ivec2 const size = GetSize();
    Image ret(size);

    if (GetFormat() == PixelFormat::Y_8 || GetFormat() == PixelFormat::Y_F32)
    {
        float const *srcp = Lock<PixelFormat::Y_F32>();
        float *dstp = ret.Lock<PixelFormat::Y_F32>();

        for (int y = 0; y < size.y; ++y)
            for (int x = 0; x < size.x; ++x)
            {
                int y2 = lol::max(y - 1, 0);
                int x2 = lol::max(x - 1, 0);
                int y3 = lol::min(y + 1, size.y - 1);
                int x3 = lol::min(x + 1, size.x - 1);

                float t = srcp[y * size.x + x];
                t = lol::max(t, srcp[y * size.x + x2]);
                t = lol::max(t, srcp[y * size.x + x3]);
                t = lol::max(t, srcp[y2 * size.x + x]);
                t = lol::max(t, srcp[y3 * size.x + x]);
                dstp[y * size.x + x] = t;
            }

        Unlock(srcp);
        ret.Unlock(dstp);
    }
    else
    {
        vec4 const *srcp = Lock<PixelFormat::RGBA_F32>();
        vec4 *dstp = ret.Lock<PixelFormat::RGBA_F32>();

        for (int y = 0; y < size.y; ++y)
            for (int x = 0; x < size.x; ++x)
            {
                int y2 = lol::max(y - 1, 0);
                int x2 = lol::max(x - 1, 0);
                int y3 = lol::min(y + 1, size.y - 1);
                int x3 = lol::min(x + 1, size.x - 1);

                vec3 t = srcp[y * size.x + x].rgb;
                t = lol::min(t, srcp[y * size.x + x2].rgb);
                t = lol::min(t, srcp[y * size.x + x3].rgb);
                t = lol::min(t, srcp[y2 * size.x + x].rgb);
                t = lol::min(t, srcp[y3 * size.x + x].rgb);
                dstp[y * size.x + x] = vec4(t, srcp[y * size.x + x].a);
            }

        Unlock(srcp);
        ret.Unlock(dstp);
    }

    return ret;
}

} /* namespace lol */

