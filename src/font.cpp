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

#include <cstring>
#include <cstdio>

#include "core.h"

using namespace std;

namespace lol
{

/*
 * Font implementation class
 */

class FontData
{
    friend class Font;

private:
    String m_name;
    TileSet *tileset;
    ivec2 size;
};

/*
 * Public Font class
 */

Font::Font(char const *path)
  : data(new FontData())
{
    data->m_name = String("<font> ") + path;

    data->tileset = Tiler::Register(path, ivec2::zero, ivec2(16));
    data->size = data->tileset->GetTileSize(0);

    m_drawgroup = DRAWGROUP_BEFORE;
}

Font::~Font()
{
    Tiler::Deregister(data->tileset);
    delete data;
}

void Font::TickDraw(float seconds)
{
    Entity::TickDraw(seconds);
}

char const *Font::GetName()
{
    return data->m_name.C();
}

void Font::Print(vec3 pos, char const *str, vec2 scale)
{
    int origin_x = pos.x;
    while (*str)
    {
        uint32_t ch = (uint8_t)*str++;

        if(ch == '\n')
        {
            pos.x = origin_x;
            pos.y-=data->size.y * scale.y;
            continue;
        }else if (ch != ' '){
            g_scene->AddTile(data->tileset, ch & 255, pos, 0, scale);
        }
        pos.x += data->size.x * scale.x;
    }
}

ivec2 Font::GetSize() const
{
    return data->size;
}

} /* namespace lol */

