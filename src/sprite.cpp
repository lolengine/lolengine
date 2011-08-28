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

#include "core.h"

namespace lol
{

/*
 * Sprite implementation class
 */

class SpriteData
{
    friend class Sprite;

private:
    TileSet *tileset;
    vec3 pos;
    int id;
};

/*
 * Public Sprite class
 */

Sprite::Sprite(TileSet *tileset, int id)
  : data(new SpriteData())
{
    data->tileset = tileset;
    data->pos = vec3(0);
    data->id = id;
}

void Sprite::TickGame(float deltams)
{
    Entity::TickGame(deltams);
}

void Sprite::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    Scene::GetDefault()->AddTile(data->tileset, data->id, data->pos, 0);
}

Sprite::~Sprite()
{
    Tiler::Deregister(data->tileset);
    delete data;
}

} /* namespace lol */

