//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Emitter class
// -----------------
//

#if !defined __LOL_EMITTER_H__
#define __LOL_EMITTER_H__

#include "entity.h"
#include "tileset.h"

namespace lol
{

class EmitterData;

class Emitter : public Entity
{
public:
    Emitter(TileSet *tileset, vec3 gravity);
    virtual ~Emitter();

    void AddParticle(int id, vec3 pos, vec3 vel);

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds, Scene &scene);

private:
    EmitterData *data;
};

} /* namespace lol */

#endif // __LOL_EMITTER_H__

