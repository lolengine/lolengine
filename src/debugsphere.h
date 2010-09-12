//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The DebugSphere class
// ---------------------
//

#if !defined __DH_DEBUGSPHERE_H__
#define __DH_DEBUGSPHERE_H__

#include "entity.h"

class DebugSphereData;

class DebugSphere : public Entity
{
public:
    DebugSphere();
    virtual ~DebugSphere();

protected:
    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);

private:
    DebugSphereData *data;
};

#endif // __DH_DEBUGSPHERE_H__

