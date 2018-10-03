//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

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

void Sprite::tick_game(float seconds)
{
    Entity::tick_game(seconds);
}

void Sprite::tick_draw(float seconds, Scene &scene)
{
    Entity::tick_draw(seconds, scene);

    scene.AddTile(data->tileset, data->id, m_position, vec2(1.0f), 0.0f);
}

Sprite::~Sprite()
{
    delete data;
}

} /* namespace lol */

