//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstring>
#include <cstdlib>

namespace lol
{

Light::Light()
  : m_color(1.f),
    m_type(LightType::Directional)
{
    m_gamegroup = tickable::group::game::light;
    m_drawgroup = tickable::group::draw::light;

    SetPosition(vec3::zero);
}

Light::~Light()
{
}

void Light::SetType(LightType const &type)
{
    m_type = type;
}

LightType Light::GetType()
{
    return m_type;
}

void Light::SetPosition(vec3 const &pos)
{
    m_position = pos;
}

vec3 Light::GetPosition()
{
    return m_position;
}

void Light::SetColor(vec4 const &color)
{
    m_color = color;
}

vec4 Light::GetColor()
{
    return m_color;
}

void Light::tick_game(float seconds)
{
    WorldEntity::tick_game(seconds);
}

void Light::tick_draw(float seconds, Scene &scene)
{
    WorldEntity::tick_draw(seconds, scene);

    scene.AddLight(this);
}

} /* namespace lol */

