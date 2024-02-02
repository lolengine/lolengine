//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Emitter class
// -----------------
//

#include "engine/entity.h"
#include "tileset.h"

namespace lol
{

class EmitterData;

class Emitter : public entity
{
public:
    Emitter(TileSet *tileset, vec3 gravity);
    virtual ~Emitter();

    void AddParticle(int id, vec3 pos, vec3 vel);

protected:
    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, Scene &scene);

private:
    EmitterData *data;
};

} /* namespace lol */

