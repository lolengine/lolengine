//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdlib>

namespace lol
{

/*
 * Public WorldEntity class
 */

WorldEntity::WorldEntity()
  : m_aabb(vec3(0.f), vec3(0.f)),
    m_position(0.f),
    m_velocity(0.f),
    m_rotation(1.f),
    m_rotation_velocity(0.f)
{
}

WorldEntity::~WorldEntity()
{
}

std::string WorldEntity::GetName() const
{
    return "<worldentity>";
}

void WorldEntity::tick_game(float seconds)
{
    Entity::tick_game(seconds);
}

void WorldEntity::tick_draw(float seconds, Scene &scene)
{
    Entity::tick_draw(seconds, scene);
}

} /* namespace lol */

