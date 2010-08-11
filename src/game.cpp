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
#include "font.h"

/*
 * Game implementation class
 */

class GameData
{
    friend class Game;

private:
    Map *map;
    Font *font;
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
    data->font = new Font("gfx/font/ascii.png");
    data->x = data->y = 0;
    data->done = 0;
    data->frame = 0;
}

Game::~Game()
{
    delete data->font;
    delete data->map;
    delete data;
}

void Game::SetMouse(int x, int y)
{
    data->mousex = x;
    data->mousey = y;
}

void Game::Render()
{
    Scene *scene = new Scene();

    data->map->Render(scene, data->mousex, data->mousey, 0);

    scene->Render();
    delete scene;

    char buf[1024];
    sprintf(buf, "Frame %i", data->frame++);
    data->font->Print(10, 10, buf);
}

void Game::Quit()
{
    data->done = 1;
}

int Game::Finished()
{
    return data->done;
}

