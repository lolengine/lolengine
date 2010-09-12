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
 * MapViewer implementation class
 */

class MapViewerData
{
    friend class MapViewer;

private:
    Map *map;
    int x, y;
    int povx, povy;
    int done;

    Scene *scene;
};

/*
 * Public MapViewer class
 */

MapViewer::MapViewer(char const *mapname)
{
    data = new MapViewerData();
    data->map = new Map(mapname);
    data->x = data->y = 0;
    data->done = 0;
    data->scene = NULL;
}

MapViewer::~MapViewer()
{
    delete data->map;
    delete data;
}

void MapViewer::TickGame(float deltams)
{
    Entity::TickGame(deltams);
}

void MapViewer::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    GetScene();

    data->map->Render(data->scene, -data->povx, -data->povy, 0);
    data->scene->Render();

    delete data->scene;
    data->scene = NULL;
}

Scene *MapViewer::GetScene()
{
    if (!data->scene)
        data->scene = new Scene();
    return data->scene;
}

int MapViewer::GetWidth()
{
    return data->map->GetWidth();
}

int MapViewer::GetHeight()
{
    return data->map->GetHeight();
}

void MapViewer::SetPOV(int x, int y)
{
    data->povx = x;
    data->povy = y;
}

