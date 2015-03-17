//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#if defined(_WIN32) && !defined(_XBOX)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   if defined USE_D3D9
#       define FAR
#       define NEAR
#       include <d3d9.h>
#   endif
#endif

#include "textureimage-private.h"

namespace lol
{

/*
 * TileSet implementation class
 */

class TileSetData : public TextureImageData
{
    friend class TileSet;

protected:
    /* Pixels, then texture coordinates */
    array<ibox2, box2> m_tiles;
    ivec2 m_tile_size;
};

TextureImageData* TileSet::GetNewData()
{
    return new TileSetData();
}

TileSetData* TileSet::GetData()
{
    return (TileSetData*)m_data;
}

TileSetData const* TileSet::GetData() const
{
    return (TileSetData const*)m_data;
}

/*
 * Public TileSet class
 */

TileSet::TileSet(char const *path)
    : TextureImage(path)
{
    array<ivec2, ivec2> tiles;
    if (m_data->m_image->RetrieveTiles(tiles))
        for (ptrdiff_t i = 0; i < tiles.Count(); i++)
            AddTile(ibox2(tiles[0].m1, tiles[0].m1 + tiles[0].m2));
}

TileSet::TileSet(char const *path, Image* image)
    : TextureImage(path, image)
{
    array<ivec2, ivec2> tiles;
    if (m_data->m_image->RetrieveTiles(tiles))
        for (ptrdiff_t i = 0; i < tiles.Count(); i++)
            AddTile(ibox2(tiles[0].m1, tiles[0].m1 + tiles[0].m2));
}

TileSet::TileSet(char const *path, ivec2 size, ivec2 count)
    : TileSet(path)
{
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

TileSet::TileSet(char const *path, Image* image, ivec2 size, ivec2 count)
    : TileSet(path, image)
{
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

TileSet::~TileSet()
{
}

void TileSet::Init(char const *path, Image* image)
{
    super::Init(path, image);

    m_data->m_name = String("<tileset> ") + path;
}

//Inherited from Entity -------------------------------------------------------
char const *TileSet::GetName()
{
    return m_data->m_name.C();
}

//New methods -----------------------------------------------------------------
ptrdiff_t TileSet::AddTile(ibox2 rect)
{
    TileSetData* data = GetData();
    data->m_tiles.Push(rect,
                        box2((vec2)rect.A / (vec2)m_data->m_texture_size,
                            (vec2)rect.B / (vec2)m_data->m_texture_size));
    return data->m_tiles.Count() - 1;
}

void TileSet::AddTile(ivec2 count)
{
    ivec2 size = m_data->m_image_size / count;

    for (int j = 0; j < count.y; ++j)
    for (int i = 0; i < count.x; ++i)
    {
        AddTile(ibox2(size * ivec2(i, j),
                      size * ivec2(i + 1, j + 1)));
    }
}

ptrdiff_t TileSet::GetTileCount() const
{
    return GetData()->m_tiles.Count();
}

ivec2 TileSet::GetTileSize(ptrdiff_t tileid) const
{
    ibox2 const &box = GetData()->m_tiles[tileid].m1;
    return box.B - box.A;
}

//Palette ---------------------------------------------------------------------
void TileSet::SetPalette(TileSet* palette)
{
    m_palette = palette;
}

TileSet* TileSet::GetPalette()
{
    return m_palette;
}

TileSet const* TileSet::GetPalette() const
{
    return m_palette;
}

void TileSet::BlitTile(uint32_t id, vec3 pos, int o, vec2 scale, float angle,
                       vec3 *vertex, vec2 *texture)
{
    TileSetData* data = GetData();
    ibox2 pixels = data->m_tiles[id].m1;
    box2 texels = data->m_tiles[id].m2;
    float dtx = texels.B.x - texels.A.x;
    float dty = texels.B.y - texels.A.y;
    float tx = texels.A.x;
    float ty = texels.A.y;

    int dx =         (int)((float)(pixels.B.x - pixels.A.x) * scale.x);
    int dy = o ? 0 : (int)((float)(pixels.B.y - pixels.A.y) * scale.y);
    int dz = o ?     (int)((float)(pixels.B.y - pixels.A.y) * scale.y) : 0;

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

    vec3 extent_x = 0.5f * vec3((float)dx, 0.f, 0.f);
    vec3 extent_y = 0.5f * vec3(0.f, (float)dy, (float)dz);
    vec3 center = pos + extent_x + extent_y;
    extent_x = mat3::rotate(angle, vec3::axis_z) * extent_x;
    extent_y = mat3::rotate(angle, vec3::axis_z) * extent_y;

    if (!m_data->m_image && m_data->m_texture)
    {
        *vertex++ = center + extent_x + extent_y;
        *vertex++ = center - extent_x + extent_y;
        *vertex++ = center + extent_x - extent_y;
        *vertex++ = center + extent_x - extent_y;
        *vertex++ = center - extent_x + extent_y;
        *vertex++ = center - extent_x - extent_y;

        *texture++ = vec2(tx + dtx, ty);
        *texture++ = vec2(tx,       ty);
        *texture++ = vec2(tx + dtx, ty + dty);
        *texture++ = vec2(tx + dtx, ty + dty);
        *texture++ = vec2(tx,       ty);
        *texture++ = vec2(tx,       ty + dty);
    }
    else
    {
        memset(vertex, 0, 6 * sizeof(vec3));
        memset(texture, 0, 6 * sizeof(vec2));
    }
}

} /* namespace lol */

