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

DECLARE_IMAGE_LOADER(ZedPaletteImageData, 0)
{
public:
    virtual bool Open(char const *);
    virtual bool Save(char const *);
    virtual bool Close();

    virtual uint8_t *GetData() const;

private:
    uint8_t *m_pixels;
};

/*
 * Public Image class
 */

bool ZedPaletteImageData::Open(char const *path)
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
    m_size = ivec2(tex_size);
#else
    int32_t tex_sqrt = file_size / 3;
    int32_t tex_size = 2;
    while (tex_size < tex_sqrt)
        tex_size <<= 1;
    m_size = ivec2(tex_size, 1);
#endif

    m_format = PixelFormat::RGBA_8;
    m_pixels = new uint8_t[tex_size * tex_size * 4 * sizeof(*m_pixels)];
    uint8_t *parser = m_pixels;
    for (int i = 0; i < file_buffer.Count();)
    {
        *parser++ = file_buffer[i++];
        *parser++ = file_buffer[i++];
        *parser++ = file_buffer[i++];
        *parser++ = (i == 0) ? 0 : 255;
    }

    return true;
}

bool ZedPaletteImageData::Save(char const *path)
{
    UNUSED(path);
    /* FIXME: do we need to implement this? */
    return true;
}

bool ZedPaletteImageData::Close()
{
    delete[] m_pixels;

    return true;
}

uint8_t * ZedPaletteImageData::GetData() const
{
    return m_pixels;
}

} /* namespace lol */

