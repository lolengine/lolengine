//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctype.h>

#include "core.h"

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
{
    data = new WorldData();
    data->width = 0;
    data->height = 0;

    drawgroup = DRAWGROUP_BEFORE;
}

World::~World()
{
    delete data;
}

char const *World::GetName()
{
    return "<world>";
}

void World::TickGame(float deltams)
{
    Entity::TickGame(deltams);
}

void World::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);
}

int World::GetWidth()
{
    return data->width * 32;
}

int World::GetHeight()
{
    return data->height * 32;
}

