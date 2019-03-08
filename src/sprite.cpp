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
    entity::tick_game(seconds);
}

void Sprite::tick_draw(float seconds, Scene &scene)
{
    entity::tick_draw(seconds, scene);

    scene.AddTile(data->tileset, data->id, m_position, vec2(1.0f), 0.0f);
}

Sprite::~Sprite()
{
    delete data;
}

} /* namespace lol */

