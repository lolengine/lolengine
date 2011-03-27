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
// The World class
// ---------------
//

#if !defined __LOL_WORLD_H__
#define __LOL_WORLD_H__

#include "entity.h"

namespace lol
{

class WorldData;

class World : public Entity
{
public:
    World();
    virtual ~World();

protected:
    /* Inherited from Entity */
    virtual char const *GetName();
    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);

public:
    /* New methods */
    int GetWidth();
    int GetHeight();

private:
    WorldData *data;
};

} /* namespace lol */

#endif // __LOL_WORLD_H__

