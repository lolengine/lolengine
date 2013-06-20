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
    String m_name;

    /* Pixels, then texture coordinates */
    Array<ibox2, box2> m_tiles;
    ivec2 m_tile_size, m_image_size, m_texture_size;

    Image *m_image;
    Texture *m_texture;
};

/*
 * Public TileSet class
 */

TileSet::TileSet(char const *path)
  : m_data(new TileSetData())
{
    Init(path);
}

TileSet::TileSet(char const *path, ivec2 size, ivec2 count)
  : m_data(new TileSetData())
{
    Init(path);

    /* If count is valid, fix size; otherwise, fix count. */
    if (count.x > 0 && count.y > 0)
    {
        size = m_data->m_image_size / count;
    }
    else
    {
        if (size.x <= 0 || size.y <= 0)
            size = ivec2(32, 32);
        count = max(ivec2(1, 1), m_data->m_image_size / size);
    }

    for (int j = 0; j < count.y; ++j)
    for (int i = 0; i < count.x; ++i)
    {
        AddTile(ibox2(size * ivec2(i, j),
                      size * ivec2(i + 1, j + 1)));
    }
}

void TileSet::Init(char const *path)
{
    m_data->m_name = String("<tileset> ") + path;

    m_data->m_texture = 0;
    m_data->m_image = Image::Create(path);
    m_data->m_image_size = m_data->m_image->GetSize();
    m_data->m_texture_size = ivec2(PotUp(m_data->m_image_size.x),
                                   PotUp(m_data->m_image_size.y));

    m_drawgroup = DRAWGROUP_BEFORE;
}

int TileSet::AddTile(ibox2 rect)
{
    m_data->m_tiles.Push(rect,
                         box2((vec2)rect.A / (vec2)m_data->m_texture_size,
                              (vec2)rect.B / (vec2)m_data->m_texture_size));
    return m_data->m_tiles.Count() - 1;
}

TileSet::~TileSet()
{
    delete m_data;
}

void TileSet::TickDraw(float seconds)
{
    Entity::TickDraw(seconds);

    if (IsDestroying())
    {
        if (m_data->m_image)
        {
            Image::Destroy(m_data->m_image);
            m_data->m_image = nullptr;
        }
        else
        {
            delete m_data->m_texture;
            m_data->m_texture = nullptr;
        }
    }
    else if (m_data->m_image)
    {
        PixelFormat format = m_data->m_image->GetFormat();
        int planes = format.BytesPerPixel();

        int w = m_data->m_texture_size.x;
        int h = m_data->m_texture_size.y;

        uint8_t *pixels = m_data->m_image->GetData();
        if (w != m_data->m_image_size.x || h != m_data->m_image_size.y)
        {
            uint8_t *tmp = new uint8_t[planes * w * h];
            for (int line = 0; line < m_data->m_image_size.y; line++)
                memcpy(tmp + planes * w * line,
                       pixels + planes * m_data->m_image_size.x * line,
                       planes * m_data->m_image_size.x);
            pixels = tmp;
        }

        m_data->m_texture = new Texture(ivec2(w, h), format);
        m_data->m_texture->SetData(pixels);

        if (pixels != m_data->m_image->GetData())
            delete[] pixels;
        Image::Destroy(m_data->m_image);
        m_data->m_image = nullptr;
    }
}

char const *TileSet::GetName()
{
    return m_data->m_name.C();
}

int TileSet::GetTileCount() const
{
    return m_data->m_tiles.Count();
}

ivec2 TileSet::GetTileSize(int tileid) const
{
    ibox2 const &box = m_data->m_tiles[tileid].m1;
    return box.B - box.A;
}

ivec2 TileSet::GetTextureSize() const
{
    return m_data->m_texture_size;
}

ShaderTexture TileSet::GetTexture() const
{
    return m_data->m_texture->GetTexture();
}

void TileSet::Bind()
{
    if (!m_data->m_image && m_data->m_texture)
        m_data->m_texture->Bind();
}

void TileSet::Unbind()
{
    ;
}

void TileSet::BlitTile(uint32_t id, vec3 pos, int o, vec2 scale,
                       float *vertex, float *texture)
{
    ibox2 pixels = m_data->m_tiles[id].m1;
    box2 texels = m_data->m_tiles[id].m2;
    float dtx = texels.B.x - texels.A.x;
    float dty = texels.B.y - texels.A.y;
    float tx = texels.A.x;
    float ty = texels.A.y;

    int dx = (pixels.B.x - pixels.A.x) * scale.x;
    int dy = o ? 0 : (pixels.B.y - pixels.A.y) * scale.y;
    int dz = o ? (pixels.B.y - pixels.A.y) * scale.y : 0;

    /* If scaling is negative, switch triangle winding */
    if (scale.x * scale.y < 0.0f)
    {
        pos.x += dx;
        dx = -dx;

        tx += dtx;
        dtx = -dtx;
    }

#if 1
    /* HACK: tweak UV values */
    tx += (1.f / 128.f) * dtx;
    ty += (1.f / 128.f) * dty;
    dtx *= 126.f / 128.f;
    dty *= 126.f / 128.f;
#endif

    if (!m_data->m_image && m_data->m_texture)
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

