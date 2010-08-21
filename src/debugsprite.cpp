//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include "debugsprite.h"
#include "game.h"
#include "tiler.h"

/*
 * DebugSprite implementation class
 */

class DebugSpriteData
{
    friend class DebugSprite;

private:
    Game *game;
    int tiler;
    int frame;
};

/*
 * Public DebugSprite class
 */

DebugSprite::DebugSprite(Game *game)
{
    data = new DebugSpriteData();
    data->game = game;
    data->tiler = Tiler::Register("art/test/character-dress.png");
}

Entity::Group DebugSprite::GetGroup()
{
    return GROUP_DEFAULT;
}

void DebugSprite::TickGame(float deltams)
{
    Entity::TickGame(deltams);
}

void DebugSprite::TickRender(float deltams)
{
    Entity::TickRender(deltams);

    data->game->GetScene()->AddTile((data->tiler << 16) | 15, 320, 240, 32, 1);
    data->game->GetScene()->AddTile((data->tiler << 16) | 31, 320, 240, 0, 1);
}

DebugSprite::~DebugSprite()
{
    Tiler::Deregister(data->tiler);
    delete data;
}

