//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//                   2013 Jean-Yves Lamoureux <jylam@lnxscene.org>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstring>
#include <cstdio>

namespace lol
{

/* The font cache */
static entity_dict<Font> font_cache;

/*
 * Font implementation class
 */

class FontData
{
    friend class Font;

private:
    std::string m_name;
    TileSet *tileset;
    ivec2 size;
};

/*
 * Public Font class
 */

Font *Font::create(std::string const &path)
{
    auto ret = font_cache.get(path);
    return ret ? ret : font_cache.set(path, new Font(path));
}

void Font::destroy(Font *f)
{
    // FIXME: decrement!
    font_cache.erase(f);
}

Font::Font(std::string const &path)
  : data(new FontData())
{
    data->m_name = "<font> " + path;

    data->tileset = TileSet::create(path, ivec2::zero, ivec2(16));
    data->size = data->tileset->GetTileSize(0);

    m_drawgroup = tickable::group::draw::texture;
}

Font::~Font()
{
    TileSet::destroy(data->tileset);
    delete data;
}

void Font::tick_draw(float seconds, Scene &scene)
{
    entity::tick_draw(seconds, scene);

    if (data->tileset->GetTexture())
    {
        data->tileset->GetTexture()->SetMinFiltering(TextureMinFilter::LINEAR_TEXEL_NO_MIPMAP);
        data->tileset->GetTexture()->SetMagFiltering(TextureMagFilter::LINEAR_TEXEL);
    }
}

std::string Font::GetName() const
{
    return data->m_name;
}

void Font::Print(Scene &scene, vec3 pos, std::string const &str, vec2 scale, float spacing)
{
    float origin_x = pos.x;
    for (int i = 0; i < (int)str.length(); ++i)
    {
        uint32_t ch = str[i];

        switch (ch)
        {
        case '\r': /* carriage return */
            pos.x = origin_x;
            break;
        case '\b': /* backspace */
            pos.x -= data->size.x * scale.x;
            break;
        case '\n': /* new line */
            pos.x = origin_x;
            pos.y -= data->size.y * scale.y;
            break;
        default:
            if (ch != ' ')
                scene.AddTile(data->tileset, ch & 255, pos, scale, 0.0f);
            pos.x += data->size.x * scale.x;
            break;
        }

        pos.x += data->size.x * scale.x * spacing;
    }
}

ivec2 Font::GetSize() const
{
    return data->size;
}

} /* namespace lol */

