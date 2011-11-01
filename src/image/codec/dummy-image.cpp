//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cmath>

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
    virtual bool Close();

    virtual void *GetData() const;

private:
    uint8_t *pixels;
};

/*
 * Public Image class
 */

bool DummyImageData::Open(char const *path)
{
    size = ivec2(256);
    format = Image::FORMAT_RGBA;
    pixels = (uint8_t *)malloc(256 * 256 * 4 * sizeof(*pixels));
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

bool DummyImageData::Close()
{
    free(pixels);

    return true;
}

void * DummyImageData::GetData() const
{
    return pixels;
}

} /* namespace lol */

