//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The GraphicEntity class
// -----------------------
//

#if !defined __LOL_WORLDENTITY_H__
#define __LOL_WORLDENTITY_H__

#include "entity.h"

namespace lol
{

class WorldEntity : public Entity
{
public:
    virtual char const *GetName();

public:
    vec3 m_position;
    vec3 m_velocity;
    quat m_rotation;
    vec3 m_rotation_velocity;
    vec3 m_bbox[2];

    ivec2 m_mousepos;
    ivec3 m_mousebuttons;
    ivec3 m_pressed, m_clicked, m_released;

protected:
    WorldEntity();
    virtual ~WorldEntity();

    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);
};

} /* namespace lol */

#endif // __LOL_WORLDENTITY_H__

