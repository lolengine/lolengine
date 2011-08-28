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

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#include "core.h"
#include "lolgl.h"

using namespace std;

namespace lol
{

/*
 * TileSet implementation class
 */

class TileSetData
{
    friend class TileSet;

private:
    char *name, *path;
    int *tiles, ntiles;
    ivec2 size, isize, count;
    float dilate, tx, ty;

    Image *img;
    GLuint texture;
};

/*
 * Public TileSet class
 */

TileSet::TileSet(char const *path, ivec2 size, ivec2 count, float dilate)
  : data(new TileSetData())
{
    data->name = (char *)malloc(10 + strlen(path) + 1);
    data->path = data->name + 10;
    sprintf(data->name, "<tileset> %s", path);

    data->tiles = NULL;
    data->texture = 0;
    data->img = new Image(path);
    data->isize = data->img->GetSize();

    if (count.i > 0 && count.j > 0)
    {
        data->count = count;
        data->size = data->isize / count;
    }
    else
    {
        if (size.x <= 0 || size.y <= 0)
            size = 32;
        data->count.i = data->isize.x > size.i ? data->isize.x / size.i : 1;
        data->count.j = data->isize.y > size.j ? data->isize.y / size.j : 1;
        data->size = size;
    }

    data->tx = (float)data->size.x / PotUp(data->isize.x);
    data->ty = (float)data->size.y / PotUp(data->isize.y);

    data->dilate = dilate;
    data->ntiles = data->count.i * data->count.j;

    drawgroup = DRAWGROUP_BEFORE;
}

TileSet::~TileSet()
{
    free(data->tiles);
    free(data->name);
    delete data;
}

void TileSet::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    if (IsDestroying())
    {
        if (data->img)
            delete data->img;
        else
            glDeleteTextures(1, &data->texture);
    }
    else if (data->img)
    {
        GLuint format;
        int planes;

        switch (data->img->GetFormat())
        {
        case Image::FORMAT_RGB:
           format = GL_RGB;
           planes = 3;
           break;
        case Image::FORMAT_RGBA:
        default:
           format = GL_RGBA;
           planes = 4;
           break;
        }

        int w = PotUp(data->isize.x);
        int h = PotUp(data->isize.y);

        uint8_t *pixels = (uint8_t *)data->img->GetData();
        if (w != data->isize.x || h != data->isize.y)
        {
            uint8_t *tmp = (uint8_t *)malloc(planes * w * h);
            for (int line = 0; line < data->isize.y; line++)
                memcpy(tmp + planes * w * line,
                       pixels + planes * data->isize.x * line,
                       planes * data->isize.x);
            pixels = tmp;
        }

        glGenTextures(1, &data->texture);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, data->texture);

        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0,
                     format, GL_UNSIGNED_BYTE, pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        if (pixels != data->img->GetData())
            free(pixels);
        delete data->img;
        data->img = NULL;
    }
}

char const *TileSet::GetName()
{
    return data->name;
}

ivec2 TileSet::GetCount() const
{
    return data->count;
}

ivec2 TileSet::GetSize(int tileid) const
{
    return data->size;
}

void TileSet::Bind()
{
    if (!data->img && data->texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, data->texture);
    }
}

void TileSet::BlitTile(uint32_t id, vec3 pos, int o,
                       float *vertex, float *texture)
{
    float tx = data->tx * ((id & 0xffff) % data->count.i);
    float ty = data->ty * ((id & 0xffff) / data->count.i);
    float dilate = data->dilate;

    int dx = data->size.x;
    int dy = o ? 0 : data->size.y;
    int dz = o ? data->size.y : 0;

    if (!data->img && data->texture)
    {
        float tmp[10];

        *vertex++ = tmp[0] = pos.x;
        *vertex++ = tmp[1] = dilate * (pos.y + dy);
        *vertex++ = tmp[2] = dilate * (pos.z + dz);
        *texture++ = tmp[3] = tx;
        *texture++ = tmp[4] = ty;

        *vertex++ = pos.x + dx;
        *vertex++ = dilate * (pos.y + dy);
        *vertex++ = dilate * (pos.z + dz);
        *texture++ = tx + data->tx;
        *texture++ = ty;

        *vertex++ = tmp[5] = pos.x + dx;
        *vertex++ = tmp[6] = dilate * pos.y;
        *vertex++ = tmp[7] = dilate * pos.z;
        *texture++ = tmp[8] = tx + data->tx;
        *texture++ = tmp[9] = ty + data->ty;

        *vertex++ = tmp[0];
        *vertex++ = tmp[1];
        *vertex++ = tmp[2];
        *texture++ = tmp[3];
        *texture++ = tmp[4];

        *vertex++ = tmp[5];
        *vertex++ = tmp[6];
        *vertex++ = tmp[7];
        *texture++ = tmp[8];
        *texture++ = tmp[9];

        *vertex++ = pos.x;
        *vertex++ = dilate * pos.y;
        *vertex++ = dilate * pos.z;
        *texture++ = tx;
        *texture++ = ty + data->ty;
    }
    else
    {
        memset(vertex, 0, 3 * sizeof(float));
        memset(texture, 0, 2 * sizeof(float));
    }
}

} /* namespace lol */

