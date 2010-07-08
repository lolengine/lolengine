
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
};

/*
 * Public Game class
 */

Game::Game(char const *mapname)
{
    data = new GameData();
    data->map = new Map(mapname);
    data->x = data->y = 0;
}

Game::~Game()
{
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
}

