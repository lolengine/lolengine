//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include "core.h"

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

    Scene *scene;
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
    data->scene = NULL;
}

Game::~Game()
{
    delete data->map;
    delete data;
}

Entity::Group Game::GetGroup()
{
    return Entity::GetGroup();
}

void Game::TickGame(float deltams)
{
    Entity::TickGame(deltams);
}

void Game::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    GetScene();

    data->map->Render(data->scene, -data->mousex, -data->mousey, 0);
    data->scene->Render();

    delete data->scene;
    data->scene = NULL;
}

Scene *Game::GetScene()
{
    if (!data->scene)
        data->scene = new Scene();
    return data->scene;
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

