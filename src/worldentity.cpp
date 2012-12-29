//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>

#include "core.h"

namespace lol
{

/*
 * Public WorldEntity class
 */

WorldEntity::WorldEntity()
{
    m_position = vec3(0);
    m_rotation = quat(1);
    m_velocity = vec3(0);
    m_rotation_velocity = vec3(0);
    m_bbox[0] = m_bbox[1] = vec3(0);

    m_mousepos = ivec2(0);
    m_mousebuttons = ivec3(0);
    m_pressed = ivec3(0);
    m_clicked = ivec3(0);
    m_released = ivec3(0);
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

void WorldEntity::TickDraw(float seconds)
{
    Entity::TickDraw(seconds);
}

} /* namespace lol */

