//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cmath>

#include "core.h"
#include "debugsprite.h"

/*
 * DebugSprite implementation class
 */

class DebugSpriteData
{
    friend class DebugSprite;

private:
    Game *game;
    int tiler;
    float x, y, z;
};

/*
 * Public DebugSprite class
 */

DebugSprite::DebugSprite(Game *game)
{
    data = new DebugSpriteData();
    data->game = game;
    data->tiler = Tiler::Register("art/test/character-dress.png");
    data->x = 320;
    data->y = 206;
    data->z = 0;
}

Entity::Group DebugSprite::GetGroup()
{
    return GROUP_DEFAULT;
}

void DebugSprite::TickGame(float deltams)
{
    Entity::TickGame(deltams);

    Float2 axis = Input::GetAxis(0);
    data->x += 0.1f * sqrtf(2.0f) * deltams * axis.x;
    data->y += 0.1f * deltams * axis.y;
}

void DebugSprite::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    int x = data->x;
    int y = data->y;
    int z = data->z;

    data->game->GetScene()->AddTile((data->tiler << 16) | 15,
                                    x - 16, y - 32, z + 32, 1);
    data->game->GetScene()->AddTile((data->tiler << 16) | 31,
                                    x - 16, y - 32, z, 1);
}

DebugSprite::~DebugSprite()
{
    Tiler::Deregister(data->tiler);
    delete data;
}

