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

#include <cstdlib>

namespace lol
{

/*
 * Public WorldEntity class
 */

WorldEntity::WorldEntity()
{
    m_bbox[0] = m_bbox[1] = vec3::zero;
}

WorldEntity::~WorldEntity()
{
}

char const *WorldEntity::GetName()
{
    return "<worldentity>";
}

void WorldEntity::TickGame(float seconds)
{
    Entity::TickGame(seconds);
}

void WorldEntity::TickDraw(float seconds, Scene &scene)
{
    Entity::TickDraw(seconds, scene);
}

} /* namespace lol */

