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
// The DebugQuad class
// -------------------
//

#if !defined __LOL_DEBUGTRI_H__
#define __LOL_DEBUGTRI_H__

#include "entity.h"

namespace lol
{

class DebugQuadData;

class DebugQuad : public Entity
{
public:
    DebugQuad();
    virtual ~DebugQuad();

protected:
    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);

private:
    void Advance();

    DebugQuadData *data;
};

} /* namespace lol */

#endif // __LOL_DEBUGTRI_H__

