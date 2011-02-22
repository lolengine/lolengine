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

#include <cstdlib>
#include <cstdio>

#include "core.h"

namespace lol
{

/*
 * Public WorldEntity class
 */

WorldEntity::WorldEntity()
{
}

WorldEntity::~WorldEntity()
{
}

char const *WorldEntity::GetName()
{
    return "<worldentity>";
}

void WorldEntity::TickGame(float deltams)
{
    Entity::TickGame(deltams);
}

void WorldEntity::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);
}

} /* namespace lol */

