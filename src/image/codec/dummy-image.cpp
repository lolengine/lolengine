//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "../../image/resource-private.h"

namespace lol
{

/*
 * Image implementation class
 */

class DummyImageCodec : public ResourceCodec
{
public:
    virtual char const *GetName() { return "<DummyImageCodec>"; }
    virtual ResourceCodecData* Load(char const *path);
    virtual bool Save(char const *path, ResourceCodecData* data);
};

//Priority 0 because it's supposed to be the last one
DECLARE_IMAGE_CODEC(DummyImageCodec, 0)

/*
 * Public Image class
 */

ResourceCodecData* DummyImageCodec::Load(char const *path)
{
    if (strcmp("DUMMY", path))
        return nullptr;

    auto data = new ResourceImageData(new Image(ivec2(256)));
    auto image = data->m_image;
    u8vec4 *pixels = image->Lock<PixelFormat::RGBA_8>(), *tmp = pixels;
    for (int j = 0; j < 256; j++)
        for (int i = 0; i < 256; i++)
        {
            tmp->r = ((i ^ j) & 1) * 0xff;
            tmp->g = (uint8_t)i;
            tmp->b = (uint8_t)j;
            tmp->a = (((i >> 4) ^ (j >> 4)) & 1) * 0xff;
            ++tmp;
        }
    image->Unlock(pixels);

    return data;
}

bool DummyImageCodec::Save(char const *path, ResourceCodecData* data)
{
    UNUSED(path, data);

    return false;
}

} /* namespace lol */

