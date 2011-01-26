//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cmath>

#include "core.h"

/*
 * Emitter implementation class
 */

class EmitterData
{
    friend class Emitter;

private:
    int tiler;
    Float3 gravity;
    int particles[100];
    Float3 positions[100];
    Float3 velocities[100];
    int nparticles;
};

/*
 * Public Emitter class
 */

Emitter::Emitter(int tiler, Float3 gravity)
  : data(new EmitterData())
{
    data->tiler = tiler;
    data->gravity = gravity;
    data->nparticles = 0;
}

void Emitter::TickGame(float deltams)
{
    Entity::TickGame(deltams);
}

void Emitter::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);
}

Emitter::~Emitter()
{
    delete data;
}

