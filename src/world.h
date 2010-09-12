//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The World class
// ---------------
//

#if !defined __DH_WORLD_H__
#define __DH_WORLD_H__

#include "entity.h"

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

#endif // __DH_WORLD_H__

