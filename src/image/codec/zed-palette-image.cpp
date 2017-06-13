//
//  Lol Engine
//
//  Copyright © 2010—2017 Sam Hocevar <sam@hocevar.net>
//              2014 Benjamin Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "../../image/resource-private.h"

namespace lol
{

/*
 * Image implementation class
 */

class ZedPaletteImageCodec : public ResourceCodec
{
public:
    virtual char const *GetName() { return "<ZedPaletteImageCodec>"; }
    virtual ResourceCodecData* Load(char const *path);
    virtual bool Save(char const *path, ResourceCodecData* data);
};

DECLARE_IMAGE_CODEC(ZedPaletteImageCodec, 10)

/*
 * Public Image class
 */

ResourceCodecData* ZedPaletteImageCodec::Load(char const *path)
{
    if (!lol::String(path).ends_with(".pal"))
        return nullptr;

    File file;
    file.Open(path, FileAccess::Read, true);

    // Put file in memory
    long file_size = file.size();
    array<uint8_t> file_buffer;
    file_buffer.resize(file_size);
    file.Read((uint8_t*)&file_buffer[0], file_size);
    file.Close();

#if 0 //2D PALETTE
    int32_t tex_sqrt = (int32_t)lol::sqrt((float)file_size / 3);
    int32_t tex_size = 2;
    while (tex_size < tex_sqrt)
        tex_size <<= 1;
    auto data = new ResourceImageData(new image(ivec2(tex_size)));
    auto image = data->m_image;
#else
    int32_t tex_sqrt = file_size / 3;
    int32_t tex_size = 2;
    while (tex_size < tex_sqrt)
        tex_size <<= 1;
    auto data = new ResourceImageData(new image(ivec2(tex_size, 1)));
    auto image = data->m_image;
#endif

    u8vec4 *pixels = image->lock<PixelFormat::RGBA_8>();
    for (int i = 0; i < file_buffer.count();)
    {
        pixels->r = file_buffer[i++];
        pixels->g = file_buffer[i++];
        pixels->b = file_buffer[i++];
        pixels->a = (i == 0) ? 0 : 255;
        ++pixels;
    }
    image->unlock(pixels);

    return data;
}

bool ZedPaletteImageCodec::Save(char const *path, ResourceCodecData* data)
{
    UNUSED(path, data);
    /* FIXME: do we need to implement this? */
    return true;
}

} /* namespace lol */

