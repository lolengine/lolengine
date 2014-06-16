//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "../../image/image-private.h"

using namespace std;

namespace lol
{

/*
 * Image implementation class
 */

DECLARE_IMAGE_CODEC(DummyImageCodec, 0)
{
public:
    virtual bool Load(Image *image, char const *path);
    virtual bool Save(Image *image, char const *path);
    virtual bool Close();
};

/*
 * Public Image class
 */

bool DummyImageCodec::Load(Image *image, char const *path)
{
    UNUSED(path);

    image->SetSize(ivec2(256));
    u8vec4 *pixels = image->Lock<PixelFormat::RGBA_8>();
    for (int j = 0; j < 256; j++)
        for (int i = 0; i < 256; i++)
        {
            pixels->r = ((i ^ j) & 1) * 0xff;
            pixels->g = (uint8_t)i;
            pixels->b = (uint8_t)j;
            pixels->a = (((i >> 4) ^ (j >> 4)) & 1) * 0xff;
            ++pixels;
        }
    image->Unlock();

    return true;
}

bool DummyImageCodec::Save(Image *image, char const *path)
{
    UNUSED(path);

    return true;
}

bool DummyImageCodec::Close()
{
    return true;
}

} /* namespace lol */

