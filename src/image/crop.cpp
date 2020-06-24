//
//  Lol Engine
//
//  Copyright © 2004—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "image-private.h"

/*
 * Image cropping functions
 */

namespace lol
{

old_image old_image::Crop(ibox2 box) const
{
    ivec2 const srcsize = size();
    ivec2 const dstsize = box.extent();

    old_image dst(dstsize);
    PixelFormat fmt = format();

    if (fmt != PixelFormat::Unknown)
    {
        dst.set_format(fmt);
        uint8_t const *srcp = (uint8_t const *)m_data->m_pixels[(int)fmt];
        uint8_t *dstp = (uint8_t *)dst.m_data->m_pixels[(int)fmt];
        uint8_t bpp = BytesPerPixel(fmt);

        int len = dstsize.x;

        if (box.aa.x < 0)
        {
            len += box.aa.x;
            box.aa.x = 0;
        }

        if (box.aa.x + len > srcsize.x)
            len = srcsize.x - box.aa.x;

        if (len > 0)
        {
            for (int y = 0; y < dstsize.y; y++)
            {
                if (y + box.aa.y < 0 || y + box.aa.y >= srcsize.y)
                    continue;

                memcpy(dstp + y * dstsize.x * bpp,
                       srcp + ((y + box.aa.y) * srcsize.x + box.aa.x) * bpp,
                       len * bpp);
            }
        }
    }

    return dst;
}

} /* namespace lol */

