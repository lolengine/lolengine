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
    String m_name;
    int ntiles;
    ivec2 m_tile_size, m_image_size, count;
    vec2 m_texcoord;

    Image *m_img;
    Texture *m_texture;
};

/*
 * Public TileSet class
 */

TileSet::TileSet(char const *path, ivec2 size, ivec2 count)
  : m_data(new TileSetData())
{
    m_data->m_name = String("<tileset> ") + path;

    m_data->m_texture = 0;
    m_data->m_img = Image::Create(path);
    m_data->m_image_size = m_data->m_img->GetSize();

    if (count.x > 0 && count.y > 0)
    {
        m_data->count = count;
        m_data->m_tile_size = m_data->m_image_size / count;
    }
    else
    {
        if (size.x <= 0 || size.y <= 0)
            size = ivec2(32, 32);
        m_data->count.x = m_data->m_image_size.x / min(size.x, m_data->m_image_size.x);
        m_data->count.y = m_data->m_image_size.y / min(size.y, m_data->m_image_size.y);
        m_data->m_tile_size = size;
    }

    m_data->m_texcoord.x = (float)m_data->m_tile_size.x / PotUp(m_data->m_image_size.x);
    m_data->m_texcoord.y = (float)m_data->m_tile_size.y / PotUp(m_data->m_image_size.y);

    m_data->ntiles = m_data->count.x * m_data->count.y;

    m_drawgroup = DRAWGROUP_BEFORE;
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
        if (m_data->m_img)
        {
            Image::Destroy(m_data->m_img);
            m_data->m_img = nullptr;
        }
        else
        {
            delete m_data->m_texture;
            m_data->m_texture = nullptr;
        }
    }
    else if (m_data->m_img)
    {
        int planes;
        PixelFormat format = m_data->m_img->GetFormat();

        switch (format)
        {
        case PixelFormat::RGB_8:
            planes = 3;
            break;
        case PixelFormat::RGBA_8:
        case PixelFormat::ARGB_8:
        case PixelFormat::ABGR_8:
        default:
            planes = 4;
            break;
        }

        int w = PotUp(m_data->m_image_size.x);
        int h = PotUp(m_data->m_image_size.y);

        uint8_t *pixels = m_data->m_img->GetData();
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

        if (pixels != m_data->m_img->GetData())
            delete[] pixels;
        Image::Destroy(m_data->m_img);
        m_data->m_img = nullptr;
    }
}

char const *TileSet::GetName()
{
    return m_data->m_name.C();
}

ivec2 TileSet::GetTileCount() const
{
    return m_data->count;
}

ivec2 TileSet::GetTileSize(int tileid) const
{
    (void)tileid;

    return m_data->m_tile_size;
}

vec2 TileSet::GetImageSize() const
{
    return vec2(PotUp(m_data->m_image_size.x),
                PotUp(m_data->m_image_size.y));
}

ShaderTexture TileSet::GetTexture() const
{
    return m_data->m_texture->GetTexture();
}

void TileSet::Bind()
{
    if (!m_data->m_img && m_data->m_texture)
        m_data->m_texture->Bind();
}

void TileSet::Unbind()
{
    ;
}

void TileSet::BlitTile(uint32_t id, vec3 pos, int o, vec2 scale,
                       float *vertex, float *texture)
{
    float dtx = m_data->m_texcoord.x;
    float dty = m_data->m_texcoord.y;
    float tx = dtx * ((id & 0xffff) % m_data->count.x);
    float ty = dty * ((id & 0xffff) / m_data->count.x);

    int dx = m_data->m_tile_size.x * scale.x;
    int dy = o ? 0 : m_data->m_tile_size.y * scale.y;
    int dz = o ? m_data->m_tile_size.y * scale.y : 0;

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

    if (!m_data->m_img && m_data->m_texture)
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

