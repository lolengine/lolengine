//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstring>
#include <cstdlib>
#include <ctype.h>

namespace lol
{

/*
 * World implementation class
 */

class WorldData
{
    friend class World;
};

static WorldData g_world_data;
World g_world;

/*
 * Public World class
 */

World::World()
{
}

World::~World()
{
}

} /* namespace lol */

