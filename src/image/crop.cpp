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
#include "image-private.h"

/*
 * Image cropping functions
 */

namespace lol
{

Image Image::Crop(ibox2 box) const
{
    ivec2 const srcsize = GetSize();
    ivec2 const dstsize = box.B - box.A;

    Image dst(dstsize);
    PixelFormat format = GetFormat();

    if (format != PixelFormat::Unknown)
    {
        dst.SetFormat(format);
        uint8_t const *srcp = (uint8_t const *)m_data->m_pixels[(int)format];
        uint8_t *dstp = (uint8_t *)dst.m_data->m_pixels[(int)format];
        uint8_t bpp = BytesPerPixel(format);

        int len = dstsize.x;

        if (box.A.x < 0)
        {
            len += box.A.x;
            box.A.x = 0;
        }

        if (box.A.x + len > srcsize.x)
            len = srcsize.x - box.A.x;

        if (len > 0)
        {
            for (int y = 0; y < dstsize.y; y++)
            {
                if (y + box.A.y < 0 || y + box.A.y >= srcsize.y)
                    continue;

                memcpy(dstp + y * dstsize.x * bpp,
                       srcp + ((y + box.A.y) * srcsize.x + box.A.x) * bpp,
                       len * bpp);
            }
        }
    }

    return dst;
}

} /* namespace lol */

