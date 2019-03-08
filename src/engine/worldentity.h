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

#pragma once

//
// The GraphicEntity class
// -----------------------
//

#include <lol/math/transform.h>

#include "engine/entity.h"

namespace lol
{

class WorldEntity : public entity
{
public:
    virtual std::string GetName() const;

public:
    box3 m_aabb;
    vec3 m_position = vec3::zero;
    vec3 m_velocity = vec3::zero;
    quat m_rotation = quat(1);
    vec3 m_rotation_velocity = vec3::zero;

protected:
    WorldEntity();
    virtual ~WorldEntity();

    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, Scene &scene);
};

} /* namespace lol */

