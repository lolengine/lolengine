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

#include <cmath>

#include "core.h"

namespace lol
{

/*
 * Emitter implementation class
 */

class EmitterData
{
    friend class Emitter;

    static const int MAX_PARTICLES = 1000;

private:
    TileSet *tileset;
    vec3 gravity;
    int particles[MAX_PARTICLES];
    vec3 positions[MAX_PARTICLES];
    vec3 velocities[MAX_PARTICLES];
    int nparticles;
};

/*
 * Public Emitter class
 */

Emitter::Emitter(TileSet *tileset, vec3 gravity)
  : data(new EmitterData())
{
    data->tileset = tileset;
    data->gravity = gravity;
    data->nparticles = 0;
}

void Emitter::TickGame(float deltams)
{
    for (int i = 0; i < data->nparticles; i++)
    {
        data->velocities[i] = data->velocities[i] + deltams * data->gravity;
        data->positions[i] = data->positions[i] + deltams * data->velocities[i];
        if (data->positions[i].y < -100)
        {
            data->particles[i] = data->particles[data->nparticles - 1];
            data->positions[i] = data->positions[data->nparticles - 1];
            data->velocities[i] = data->velocities[data->nparticles - 1];
            data->nparticles--;
        }
    }

    Entity::TickGame(deltams);
}

void Emitter::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    for (int i = 0; i < data->nparticles; i++)
        Scene::GetDefault()->AddTile(data->tileset, data->particles[i],
                                     data->positions[i], 0);
}

void Emitter::AddParticle(int id, vec3 pos, vec3 vel)
{
    if (data->nparticles >= EmitterData::MAX_PARTICLES)
        return;

    data->particles[data->nparticles] = id;
    data->positions[data->nparticles] = pos;
    data->velocities[data->nparticles] = vel;
    data->nparticles++;
}

Emitter::~Emitter()
{
    delete data;
}

} /* namespace lol */

