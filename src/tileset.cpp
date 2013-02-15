//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <cstdio>
#include <cstring>

#if defined WIN32 && !defined _XBOX
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   if defined USE_D3D9
#       define FAR
#       define NEAR
#       include <d3d9.h>
#   endif
#endif

#include "core.h"
#include "lolgl.h"

using namespace std;

#if defined USE_D3D9
extern IDirect3DDevice9 *g_d3ddevice;
#elif defined _XBOX
extern D3DDevice *g_d3ddevice;
#endif

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
    vec2 scale;
    float tx, ty;

    Image *img;
    Texture *m_texture;
};

/*
 * Public TileSet class
 */

TileSet::TileSet(char const *path, ivec2 size, ivec2 count)
  : data(new TileSetData())
{
    data->name = (char *)malloc(10 + strlen(path) + 1);
    data->path = data->name + 10;
    sprintf(data->name, "<tileset> %s", path);

    data->tiles = NULL;
    data->m_texture = 0;
    data->img = new Image(path);
    data->isize = data->img->GetSize();

    if (count.x > 0 && count.y > 0)
    {
        data->count = count;
        data->size = data->isize / count;
    }
    else
    {
        if (size.x <= 0 || size.y <= 0)
            size = ivec2(32, 32);
        data->count.x = data->isize.x > size.x ? data->isize.x / size.x : 1;
        data->count.y = data->isize.y > size.y ? data->isize.y / size.y : 1;
        data->size = size;
    }

    data->tx = (float)data->size.x / PotUp(data->isize.x);
    data->ty = (float)data->size.y / PotUp(data->isize.y);

    data->ntiles = data->count.x * data->count.y;

    m_drawgroup = DRAWGROUP_BEFORE;
}

TileSet::~TileSet()
{
    free(data->tiles);
    free(data->name);
    delete data;
}

void TileSet::TickDraw(float seconds)
{
    Entity::TickDraw(seconds);

    if (IsDestroying())
    {
        if (data->img)
            delete data->img;
        else
            delete data->m_texture;
    }
    else if (data->img)
    {
        int planes;
        PixelFormat format = PixelFormat::Unknown;

        switch (data->img->GetFormat())
        {
        case Image::FORMAT_RGB:
            format = PixelFormat::RGB_8;
            planes = 3;
            break;
        case Image::FORMAT_RGBA:
        default:
            format = PixelFormat::ARGB_8;
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

        data->m_texture = new Texture(ivec2(w, h), format);
        data->m_texture->SetData(pixels);

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
    (void)tileid;

    return data->size;
}

ShaderTexture TileSet::GetTexture() const
{
    return data->m_texture->GetTexture();
}

void TileSet::Bind()
{
    if (!data->img && data->m_texture)
        data->m_texture->Bind();
}

void TileSet::Unbind()
{
    ;
}

void TileSet::BlitTile(uint32_t id, vec3 pos, int o, vec2 scale,
                       float *vertex, float *texture)
{
    float dtx = data->tx;
    float dty = data->ty;
    float tx = dtx * ((id & 0xffff) % data->count.x);
    float ty = dty * ((id & 0xffff) / data->count.x);

    int dx = data->size.x * scale.x;
    int dy = o ? 0 : data->size.y * scale.y;
    int dz = o ? data->size.y * scale.y : 0;

    /* If scaling is negative, switch triangle winding */
    if (scale.x * scale.y < 0.0f)
    {
        pos.x += dx;
        dx = -dx;

        tx += dtx;
        dtx = -dtx;
    }

    if (!data->img && data->m_texture)
    {
        float tmp[10];

        *vertex++ = pos.x + dx;
        *vertex++ = pos.y + dy;
        *vertex++ = pos.z + dz;
        *texture++ = tx + dtx;
        *texture++ = ty;

        *vertex++ = tmp[0] = pos.x;
        *vertex++ = tmp[1] = pos.y + dy;
        *vertex++ = tmp[2] = pos.z + dz;
        *texture++ = tmp[3] = tx;
        *texture++ = tmp[4] = ty;

        *vertex++ = tmp[5] = pos.x + dx;
        *vertex++ = tmp[6] = pos.y;
        *vertex++ = tmp[7] = pos.z;
        *texture++ = tmp[8] = tx + dtx;
        *texture++ = tmp[9] = ty + dty;

        *vertex++ = tmp[5];
        *vertex++ = tmp[6];
        *vertex++ = tmp[7];
        *texture++ = tmp[8];
        *texture++ = tmp[9];

        *vertex++ = tmp[0];
        *vertex++ = tmp[1];
        *vertex++ = tmp[2];
        *texture++ = tmp[3];
        *texture++ = tmp[4];

        *vertex++ = pos.x;
        *vertex++ = pos.y;
        *vertex++ = pos.z;
        *texture++ = tx;
        *texture++ = ty + dty;
    }
    else
    {
        memset(vertex, 0, 3 * sizeof(float));
        memset(texture, 0, 2 * sizeof(float));
    }
}

} /* namespace lol */

