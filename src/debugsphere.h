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
// The DebugSphere class
// ---------------------
//

#if !defined __DH_DEBUGSPHERE_H__
#define __DH_DEBUGSPHERE_H__

#include "entity.h"

namespace lol
{

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

} /* namespace lol */

#endif // __DH_DEBUGSPHERE_H__

