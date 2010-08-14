//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include "game.h"
#include "map.h"

/*
 * Game implementation class
 */

class GameData
{
    friend class Game;

private:
    Map *map;
    int x, y;
    int mousex, mousey;
    int done;

    int frame;
};

/*
 * Public Game class
 */

Game::Game(char const *mapname)
{
    data = new GameData();
    data->map = new Map(mapname);
    data->x = data->y = 0;
    data->done = 0;
    data->frame = 0;
}

Game::~Game()
{
    delete data->map;
    delete data;
}

Asset::Group Game::GetGroup()
{
    return Asset::GetGroup();
}

void Game::TickGame(float delta_time)
{
    Asset::TickGame(delta_time);
}

void Game::TickRender(float delta_time)
{
    Asset::TickRender(delta_time);

    Scene *scene = new Scene();

    data->map->Render(scene, data->mousex, data->mousey, 0);

    scene->Render();
    delete scene;
}

void Game::SetMouse(int x, int y)
{
    data->mousex = x;
    data->mousey = y;
}

void Game::Quit()
{
    data->done = 1;
}

int Game::Finished()
{
    return data->done;
}

