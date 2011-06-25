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
    int tiler;
    vec2i size;
};

/*
 * Public Font class
 */

Font::Font(char const *path)
  : data(new FontData())
{
    data->name = (char *)malloc(7 + strlen(path) + 1);
    sprintf(data->name, "<font> %s", path);

    data->tiler = Tiler::Register(path, 0, 16, 1.0f);
    data->size = Tiler::GetSize(data->tiler, 0);

    drawgroup = DRAWGROUP_BEFORE;
}

Font::~Font()
{
    Tiler::Deregister(data->tiler);
    free(data->name);
    delete data;
}

void Font::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);
}

char const *Font::GetName()
{
    return data->name;
}

void Font::Print(vec3i pos, char const *str)
{
    Scene *scene = Scene::GetDefault();

    while (*str)
    {
        uint32_t ch = (uint8_t)*str++;

        if (ch != ' ')
            scene->AddTile((data->tiler << 16) | (ch & 255),
                           pos.x, pos.y, pos.z, 0);

        pos.x += data->size.x;
    }
}

vec2i Font::GetSize() const
{
    return data->size;
}

} /* namespace lol */

