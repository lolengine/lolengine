//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
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
    vec3 m_position;
    vec3 m_rotation;
    vec3 m_velocity;
    vec3 m_bbox[2];

    ivec2 m_mousepos;
    ivec3 m_mousebuttons;
    ivec3 m_pressed, m_clicked, m_released;

protected:
    WorldEntity();
    virtual ~WorldEntity();

    virtual char const *GetName();

    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);
};

} /* namespace lol */

#endif // __LOL_WORLDENTITY_H__

