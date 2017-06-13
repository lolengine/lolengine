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
 * Dilate and erode functions
 */

/* FIXME: these functions are almost the same, try to merge them
 * somewhat efficiently. */
/* TODO: - dilate by k (Manhattan distance)
 *       - dilate by r (euclidian distance, with non-integer r) */

namespace lol
{

image image::Dilate()
{
    ivec2 isize = size();
    image ret(isize);

    if (format() == PixelFormat::Y_8 || format() == PixelFormat::Y_F32)
    {
        float const *srcp = lock<PixelFormat::Y_F32>();
        float *dstp = ret.lock<PixelFormat::Y_F32>();

        for (int y = 0; y < isize.y; ++y)
            for (int x = 0; x < isize.x; ++x)
            {
                int y2 = lol::max(y - 1, 0);
                int x2 = lol::max(x - 1, 0);
                int y3 = lol::min(y + 1, isize.y - 1);
                int x3 = lol::min(x + 1, isize.x - 1);

                float t = srcp[y * isize.x + x];
                t = lol::max(t, srcp[y * isize.x + x2]);
                t = lol::max(t, srcp[y * isize.x + x3]);
                t = lol::max(t, srcp[y2 * isize.x + x]);
                t = lol::max(t, srcp[y3 * isize.x + x]);
                dstp[y * isize.x + x] = t;
            }

        unlock(srcp);
        ret.unlock(dstp);
    }
    else
    {
        vec4 const *srcp = lock<PixelFormat::RGBA_F32>();
        vec4 *dstp = ret.lock<PixelFormat::RGBA_F32>();

        for (int y = 0; y < isize.y; ++y)
            for (int x = 0; x < isize.x; ++x)
            {
                int y2 = lol::max(y - 1, 0);
                int x2 = lol::max(x - 1, 0);
                int y3 = lol::min(y + 1, isize.y - 1);
                int x3 = lol::min(x + 1, isize.x - 1);

                vec3 t = srcp[y * isize.x + x].rgb;
                t = lol::max(t, srcp[y * isize.x + x2].rgb);
                t = lol::max(t, srcp[y * isize.x + x3].rgb);
                t = lol::max(t, srcp[y2 * isize.x + x].rgb);
                t = lol::max(t, srcp[y3 * isize.x + x].rgb);
                dstp[y * isize.x + x] = vec4(t, srcp[y * isize.x + x].a);
            }

        unlock(srcp);
        ret.unlock(dstp);
    }

    return ret;
}

image image::Erode()
{
    ivec2 isize = size();
    image ret(isize);

    if (format() == PixelFormat::Y_8 || format() == PixelFormat::Y_F32)
    {
        float const *srcp = lock<PixelFormat::Y_F32>();
        float *dstp = ret.lock<PixelFormat::Y_F32>();

        for (int y = 0; y < isize.y; ++y)
            for (int x = 0; x < isize.x; ++x)
            {
                int y2 = lol::max(y - 1, 0);
                int x2 = lol::max(x - 1, 0);
                int y3 = lol::min(y + 1, isize.y - 1);
                int x3 = lol::min(x + 1, isize.x - 1);

                float t = srcp[y * isize.x + x];
                t = lol::max(t, srcp[y * isize.x + x2]);
                t = lol::max(t, srcp[y * isize.x + x3]);
                t = lol::max(t, srcp[y2 * isize.x + x]);
                t = lol::max(t, srcp[y3 * isize.x + x]);
                dstp[y * isize.x + x] = t;
            }

        unlock(srcp);
        ret.unlock(dstp);
    }
    else
    {
        vec4 const *srcp = lock<PixelFormat::RGBA_F32>();
        vec4 *dstp = ret.lock<PixelFormat::RGBA_F32>();

        for (int y = 0; y < isize.y; ++y)
            for (int x = 0; x < isize.x; ++x)
            {
                int y2 = lol::max(y - 1, 0);
                int x2 = lol::max(x - 1, 0);
                int y3 = lol::min(y + 1, isize.y - 1);
                int x3 = lol::min(x + 1, isize.x - 1);

                vec3 t = srcp[y * isize.x + x].rgb;
                t = lol::min(t, srcp[y * isize.x + x2].rgb);
                t = lol::min(t, srcp[y * isize.x + x3].rgb);
                t = lol::min(t, srcp[y2 * isize.x + x].rgb);
                t = lol::min(t, srcp[y3 * isize.x + x].rgb);
                dstp[y * isize.x + x] = vec4(t, srcp[y * isize.x + x].a);
            }

        unlock(srcp);
        ret.unlock(dstp);
    }

    return ret;
}

} /* namespace lol */

