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
// The Emitter class
// -----------------
//

#if !defined __LOL_EMITTER_H__
#define __LOL_EMITTER_H__

#include "entity.h"

namespace lol
{

class EmitterData;

class Emitter : public Entity
{
public:
    Emitter(int tiler, vec3 gravity);
    virtual ~Emitter();

    void AddParticle(int id, vec3 pos, vec3 vel);

protected:
    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);

private:
    EmitterData *data;
};

} /* namespace lol */

#endif // __LOL_EMITTER_H__

