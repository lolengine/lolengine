//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Sprite class
// ----------------
//

#include "engine/worldentity.h"
#include "tileset.h"

namespace lol
{

class SpriteData;

class Sprite : public WorldEntity
{
public:
    Sprite(TileSet *tileset, int id);
    virtual ~Sprite();

protected:
    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, Scene &scene);

private:
    SpriteData *data;
};

} /* namespace lol */

