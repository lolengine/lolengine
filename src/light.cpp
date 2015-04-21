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

namespace lol
{

Light::Light()
  : m_color(1.f),
    m_type(LightType::Directional)
{
    m_gamegroup = GAMEGROUP_LIGHT;
    m_drawgroup = DRAWGROUP_LIGHT;

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

void Light::TickGame(float seconds)
{
    WorldEntity::TickGame(seconds);
}

void Light::TickDraw(float seconds, Scene &scene)
{
    WorldEntity::TickDraw(seconds, scene);

    Scene::AddLight(this);
}

} /* namespace lol */

