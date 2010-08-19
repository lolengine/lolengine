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

Asset::Group DebugSprite::GetGroup()
{
    return GROUP_DEFAULT;
}

void DebugSprite::TickGame(float delta_time)
{
    Asset::TickGame(delta_time);
}

void DebugSprite::TickRender(float delta_time)
{
    Asset::TickRender(delta_time);

    data->game->GetScene()->AddTile((data->tiler << 16) | 15, 300, 200, 32, 1);
    data->game->GetScene()->AddTile((data->tiler << 16) | 31, 300, 200, 0, 1);
}

DebugSprite::~DebugSprite()
{
    Tiler::Deregister(data->tiler);
    delete data;
}

