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

DECLARE_IMAGE_LOADER(DummyImageData, 0)
{
public:
    virtual bool Open(char const *);
    virtual bool Save(char const *);
    virtual bool Close();

    virtual uint8_t *GetData() const;

private:
    uint8_t *pixels;
};

/*
 * Public Image class
 */

bool DummyImageData::Open(char const *path)
{
    UNUSED(path);

    m_size = ivec2(256);
    m_format = PixelFormat::RGBA_8;
    pixels = new uint8_t[256 * 256 * 4 * sizeof(*pixels)];
    uint8_t *parser = pixels;
    for (int j = 0; j < 256; j++)
        for (int i = 0; i < 256; i++)
        {
            *parser++ = ((i ^ j) & 1) * 0xff;
            *parser++ = (uint8_t)i;
            *parser++ = (uint8_t)j;
            *parser++ = (((i >> 4) ^ (j >> 4)) & 1) * 0xff;
        }

    return true;
}

bool DummyImageData::Save(char const *path)
{
    UNUSED(path);

    return true;
}

bool DummyImageData::Close()
{
    delete[] pixels;

    return true;
}

uint8_t * DummyImageData::GetData() const
{
    return pixels;
}

} /* namespace lol */

