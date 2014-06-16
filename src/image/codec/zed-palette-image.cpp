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

DECLARE_IMAGE_CODEC(ZedPaletteImageCodec, 0)
{
public:
    virtual bool Load(Image *image, char const *path);
    virtual bool Save(Image *image, char const *path);
    virtual bool Close();
};

/*
 * Public Image class
 */

bool ZedPaletteImageCodec::Load(Image *image, char const *path)
{
    if (!lol::String(path).EndsWith(".pal"))
        return false;

    File file;
    file.Open(path, FileAccess::Read, true);

    //Put file in memory
    long file_size = file.GetSize();
    Array<uint8_t> file_buffer;
    file_buffer.Resize(file_size);
    file.Read((uint8_t*)&file_buffer[0], file_size);
    file.Close();

#if 0 //2D PALETTE
    int32_t tex_sqrt = (int32_t)lol::sqrt((float)file_size / 3);
    int32_t tex_size = 2;
    while (tex_size < tex_sqrt)
        tex_size <<= 1;
    image->m_data->m_size = ivec2(tex_size);
#else
    int32_t tex_sqrt = file_size / 3;
    int32_t tex_size = 2;
    while (tex_size < tex_sqrt)
        tex_size <<= 1;
    image->SetSize(ivec2(tex_size, 1));
#endif

    u8vec4 *pixels = image->Lock<PixelFormat::RGBA_8>();
    for (int i = 0; i < file_buffer.Count();)
    {
        pixels->r = file_buffer[i++];
        pixels->g = file_buffer[i++];
        pixels->b = file_buffer[i++];
        pixels->a = (i == 0) ? 0 : 255;
        ++pixels;
    }
    image->Unlock();

    return true;
}

bool ZedPaletteImageCodec::Save(Image *image, char const *path)
{
    UNUSED(path);
    /* FIXME: do we need to implement this? */
    return true;
}

bool ZedPaletteImageCodec::Close()
{
    return true;
}

} /* namespace lol */

