//
// Lol Engine
//
// Copyright: (c) 2004-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "image-private.h"

/*
 * Image cropping functions
 */

namespace lol
{

Image Image::Crop(ibox2 box) const
{
    ivec2 const srcsize = GetSize();
    ivec2 const dstsize = box.extent();

    Image dst(dstsize);
    PixelFormat format = GetFormat();

    if (format != PixelFormat::Unknown)
    {
        dst.SetFormat(format);
        uint8_t const *srcp = (uint8_t const *)m_data->m_pixels[(int)format];
        uint8_t *dstp = (uint8_t *)dst.m_data->m_pixels[(int)format];
        uint8_t bpp = BytesPerPixel(format);

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

