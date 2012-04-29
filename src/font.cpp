//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
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
    char *name;
    TileSet *tileset;
    ivec2 size;
};

/*
 * Public Font class
 */

Font::Font(char const *path)
  : data(new FontData())
{
    data->name = (char *)malloc(7 + strlen(path) + 1);
    sprintf(data->name, "<font> %s", path);

    data->tileset = Tiler::Register(path, ivec2(0), ivec2(16));
    data->size = data->tileset->GetSize(0);

    m_drawgroup = DRAWGROUP_BEFORE;
}

Font::~Font()
{
    Tiler::Deregister(data->tileset);
    free(data->name);
    delete data;
}

void Font::TickDraw(float seconds)
{
    Entity::TickDraw(seconds);
}

char const *Font::GetName()
{
    return data->name;
}

void Font::Print(vec3 pos, char const *str, vec2 scale)
{
    Scene *scene = Scene::GetDefault();

    while (*str)
    {
        uint32_t ch = (uint8_t)*str++;

        if (ch != ' ')
            scene->AddTile(data->tileset, ch & 255, pos, 0, scale);

        pos.x += data->size.x * scale.x;
    }
}

ivec2 Font::GetSize() const
{
    return data->size;
}

} /* namespace lol */

