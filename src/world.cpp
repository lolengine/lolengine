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
#include <cstdlib>
#include <ctype.h>

#include "core.h"

namespace lol
{

/*
 * World implementation class
 */

class WorldData
{
    friend class World;

private:
    int width, height;
};

/*
 * Public World class
 */

World::World()
  : data(new WorldData())
{
    data->width = 0;
    data->height = 0;

    m_drawgroup = DRAWGROUP_BEFORE;
}

World::~World()
{
    delete data;
}

char const *World::GetName()
{
    return "<world>";
}

void World::TickGame(float seconds)
{
    Entity::TickGame(seconds);
}

void World::TickDraw(float seconds)
{
    Entity::TickDraw(seconds);
}

int World::GetWidth()
{
    return data->width * 32;
}

int World::GetHeight()
{
    return data->height * 32;
}

} /* namespace lol */

