//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Sprite class
// ----------------
//

#if !defined __LOL_SPRITE_H__
#define __LOL_SPRITE_H__

#include "worldentity.h"
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
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

private:
    SpriteData *data;
};

} /* namespace lol */

#endif // __LOL_SPRITE_H__

