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

#include "core.h"

namespace lol
{

Light::Light()
  : m_color(1.f),
    m_directional(true)
{
    m_gamegroup = GAMEGROUP_BEFORE;
    m_drawgroup = DRAWGROUP_CAMERA;

    SetPosition(vec4(0.f));
}

Light::~Light()
{
}

void Light::SetPosition(vec4 const &pos)
{
    m_directional = (pos.w > 0.f);
    m_position = pos.xyz;
}

vec4 Light::GetPosition()
{
    return vec4(m_position, m_directional ? 1.f : 0.f);
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

void Light::TickDraw(float seconds)
{
    WorldEntity::TickDraw(seconds);

    g_scene->AddLight(this);
}

} /* namespace lol */

