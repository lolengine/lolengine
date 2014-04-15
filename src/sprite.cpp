//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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
    int id;
};

/*
 * Public Sprite class
 */

Sprite::Sprite(TileSet *tileset, int id)
  : data(new SpriteData())
{
    data->tileset = tileset;
    data->id = id;
}

void Sprite::TickGame(float seconds)
{
    Entity::TickGame(seconds);
}

void Sprite::TickDraw(float seconds)
{
    Entity::TickDraw(seconds);

    g_scene->AddTile(data->tileset, data->id, m_position, 0, vec2(1.0f), 0.0f);
}

Sprite::~Sprite()
{
    delete data;
}

} /* namespace lol */

