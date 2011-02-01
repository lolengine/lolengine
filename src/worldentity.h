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

#if !defined __DH_WORLDENTITY_H__
#define __DH_WORLDENTITY_H__

#include "entity.h"

class WorldEntity : public Entity
{
public:
    float3 position;
    float3 rotation;
    float3 velocity;
    float3 bbox[2];

    int2 mousepos;
    int3 mousebuttons;

protected:
    WorldEntity();
    virtual ~WorldEntity();

    virtual char const *GetName();

    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);
};

#endif // __DH_WORLDENTITY_H__

