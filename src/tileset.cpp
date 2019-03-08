//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <map>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#if defined _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
#endif

#include "textureimage-private.h"

namespace lol
{

/* The tileset cache */
static entity_dict<TileSet> tileset_cache;

/*
 * TileSet implementation class
 */

class TileSetData
{
    friend class TileSet;

protected:
    /* Pixels, then texture coordinates */
    array<ibox2, box2> m_tiles;
    ivec2 m_tile_size;
};

/*
 * Public TileSet class
 */

TileSet *TileSet::create(std::string const &path)
{
    auto ret = tileset_cache.get(path);
    return ret ? ret : tileset_cache.set(path, new TileSet(path));
}

TileSet *TileSet::create(std::string const &path, image* img)
{
    auto ret = tileset_cache.get(path);
    return ret ? ret : tileset_cache.set(path, new TileSet(path, img));
}

TileSet *TileSet::create(std::string const &path, image* img, array<ivec2, ivec2>& tiles)
{
    auto ret = tileset_cache.get(path);
    if (!ret)
    {
        ret = tileset_cache.set(path, new TileSet(path, img));
        ret->define_tile(tiles);
    }
    return ret;
}

TileSet *TileSet::create(std::string const &path, ivec2 size, ivec2 count)
{
    auto ret = tileset_cache.get(path);
    if (!ret)
    {
        ret = tileset_cache.set(path, new TileSet(path));

        /* If count is valid, fix size; otherwise, fix count. */
        if (count.x > 0 && count.y > 0)
        {
            size = ret->m_data->m_image_size / count;
        }
        else
        {
            if (size.x <= 0 || size.y <= 0)
                size = ivec2(32, 32);
            count = max(ivec2(1, 1), ret->m_data->m_image_size / size);
        }

        for (int j = 0; j < count.y; ++j)
        for (int i = 0; i < count.x; ++i)
        {
            ret->define_tile(ibox2(size * ivec2(i, j),
                                   size * ivec2(i + 1, j + 1)));
        }
    }

    return ret;
}

TileSet *TileSet::create(std::string const &path, image* img, ivec2 size, ivec2 count)
{
    auto ret = tileset_cache.get(path);
    if (!ret)
    {
        ret = tileset_cache.set(path, new TileSet(path, img));

        /* If count is valid, fix size; otherwise, fix count. */
        if (count.x > 0 && count.y > 0)
        {
            size = ret->m_data->m_image_size / count;
        }
        else
        {
            if (size.x <= 0 || size.y <= 0)
                size = ivec2(32, 32);
            count = max(ivec2(1, 1), ret->m_data->m_image_size / size);
        }

        for (int j = 0; j < count.y; ++j)
        for (int i = 0; i < count.x; ++i)
        {
            ret->define_tile(ibox2(size * ivec2(i, j),
                                   size * ivec2(i + 1, j + 1)));
        }
    }

    return ret;
}

void TileSet::destroy(TileSet *tileset)
{
    // FIXME: decrement!
    tileset_cache.erase(tileset);
}

TileSet::TileSet(std::string const &path)
  : TextureImage(path),
    m_tileset_data(new TileSetData()),
    m_palette(nullptr)
{
}

TileSet::TileSet(std::string const &path, image *img)
  : TextureImage(path, img),
    m_tileset_data(new TileSetData()),
    m_palette(nullptr)
{
}

TileSet::~TileSet()
{
    delete m_tileset_data;
}

void TileSet::Init(std::string const &path, ResourceCodecData* loaded_data)
{
    //Load tileset if available
    auto tileset_data = dynamic_cast<ResourceTilesetData*>(loaded_data);
    if (tileset_data != nullptr)
    {
        define_tile(tileset_data->m_tiles);
    }

    m_data->m_name = "<tileset> " + path;

    super::Init(path, loaded_data);
}

void TileSet::Init(std::string const &path, Image* image)
{
    super::Init(path, image);

    m_data->m_name = "<tileset> " + path;
}

//Inherited from entity -------------------------------------------------------
std::string TileSet::GetName() const
{
    return m_data->m_name;
}

//New methods -----------------------------------------------------------------
void TileSet::clear_all()
{
    m_tileset_data->m_tiles.clear();
}

int TileSet::define_tile(ibox2 rect)
{
    m_tileset_data->m_tiles.push(rect,
             box2((vec2)rect.aa / (vec2)m_data->m_texture_size,
                  (vec2)rect.bb / (vec2)m_data->m_texture_size));
    return m_tileset_data->m_tiles.count() - 1;
}

void TileSet::define_tile(ivec2 count)
{
    ivec2 size = m_data->m_image_size / count;

    for (int j = 0; j < count.y; ++j)
    for (int i = 0; i < count.x; ++i)
    {
        define_tile(ibox2(size * ivec2(i, j),
                          size * ivec2(i + 1, j + 1)));
    }
}

void TileSet::define_tile(array<ibox2>& tiles)
{
    for (int i = 0; i < tiles.count(); i++)
        define_tile(tiles[i]);
}

void TileSet::define_tile(array<ivec2, ivec2>& tiles)
{
    for (int i = 0; i < tiles.count(); i++)
        define_tile(ibox2(tiles[i].m1, tiles[i].m1 + tiles[i].m2));
}

int TileSet::GetTileCount() const
{
    return m_tileset_data->m_tiles.count();
}

ivec2 TileSet::GetTileSize(int tileid) const
{
    return m_tileset_data->m_tiles[tileid].m1.extent();
}

ibox2 TileSet::GetTilePixel(int tileid) const
{
    return m_tileset_data->m_tiles[tileid].m1;
}

box2 TileSet::GetTileTexel(int tileid) const
{
    return m_tileset_data->m_tiles[tileid].m2;
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

void TileSet::BlitTile(uint32_t id, mat4 model, vec3 *vertex, vec2 *texture)
{
    ibox2 pixels = m_tileset_data->m_tiles[id].m1;
    box2 texels = m_tileset_data->m_tiles[id].m2;
    float dtx = texels.extent().x;
    float dty = texels.extent().y;
    float tx = texels.aa.x;
    float ty = texels.aa.y;

    vec3 pos = (model * vec4(0.f, 0.f, 0.f, 1.f)).xyz;
    vec3 extent_x = 0.5f * pixels.extent().x * (model * vec4::axis_x).xyz;
    vec3 extent_y = 0.5f * pixels.extent().y * (model * vec4::axis_y).xyz;

    if (!m_data->m_image && m_data->m_texture)
    {
        *vertex++ = pos + extent_x + extent_y;
        *vertex++ = pos - extent_x + extent_y;
        *vertex++ = pos + extent_x - extent_y;
        *vertex++ = pos + extent_x - extent_y;
        *vertex++ = pos - extent_x + extent_y;
        *vertex++ = pos - extent_x - extent_y;

        *texture++ = vec2(tx + dtx, ty);
        *texture++ = vec2(tx,       ty);
        *texture++ = vec2(tx + dtx, ty + dty);
        *texture++ = vec2(tx + dtx, ty + dty);
        *texture++ = vec2(tx,       ty);
        *texture++ = vec2(tx,       ty + dty);
    }
    else
    {
        memset((void *)vertex, 0, 6 * sizeof(vec3));
        memset((void *)texture, 0, 6 * sizeof(vec2));
    }
}

} /* namespace lol */

