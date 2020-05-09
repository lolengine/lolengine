//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>
#include <../legacy/lol/base/assert.h>

#include <string>

#include "../../image/resource-private.h"

namespace lol
{

/*
 * Image implementation class
 */

class DummyImageCodec : public ResourceCodec
{
public:
    virtual std::string GetName() { return "<DummyImageCodec>"; }
    virtual ResourceCodecData* Load(std::string const &path);
    virtual bool Save(std::string const &path, ResourceCodecData* data);
};

//Priority 0 because it's supposed to be the last one
DECLARE_IMAGE_CODEC(DummyImageCodec, 0)

/*
 * Public Image class
 */

ResourceCodecData* DummyImageCodec::Load(std::string const &path)
{
    if (path == "DUMMY")
        return nullptr;

    auto data = new ResourceImageData(new image(ivec2(256)));
    auto image = data->m_image;
    u8vec4 *pixels = image->lock<PixelFormat::RGBA_8>(), *tmp = pixels;
    for (int j = 0; j < 256; j++)
        for (int i = 0; i < 256; i++)
        {
            tmp->r = ((i ^ j) & 1) * 0xff;
            tmp->g = (uint8_t)i;
            tmp->b = (uint8_t)j;
            tmp->a = (((i >> 4) ^ (j >> 4)) & 1) * 0xff;
            ++tmp;
        }
    image->unlock(pixels);

    return data;
}

bool DummyImageCodec::Save(std::string const &, ResourceCodecData *)
{
    return false;
}

} /* namespace lol */

