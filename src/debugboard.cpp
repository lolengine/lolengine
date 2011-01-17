//
// Deus Hax (working title)
// Copyright (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cmath>

#include "core.h"
#include "debugboard.h"

/*
 * DebugBoard implementation class
 */

class DebugBoardData
{
    friend class DebugBoard;

private:
    Game *game;
    int tiler;
    float x, y, z;
};

/*
 * Public DebugBoard class
 */

DebugBoard::DebugBoard(Game *game)
{
    data = new DebugBoardData();
    data->game = game;
    Ticker::Ref(game);
    data->tiler = Tiler::Register("monsterz/tiles.png", 48);
    data->x = 32;
    data->y = 0;
    data->z = 112;
}

void DebugBoard::TickGame(float deltams)
{
    Entity::TickGame(deltams);
}

void DebugBoard::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    int x = data->x;
    int y = data->y;
    int z = data->z;

    for (int j = 0; j < 8; j++)
        for (int i = 0; i < 8; i++)
    {
        int id = 28 + ((i + 3) * (j + 1) % 10) * 3 + ((i ^ (j + 2)) % 5);
        id += (id % 5) / 4;
        data->game->GetScene()->AddTile((data->tiler << 16) | id,
                                        x + i * 48 - 16, y + j * 48, z, 1);
    }
}

DebugBoard::~DebugBoard()
{
    Ticker::Unref(data->game);
    Tiler::Deregister(data->tiler);
    delete data;
}

