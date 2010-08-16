//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <stdlib.h>

#include "scene.h"
#include "tiler.h"

struct Tile
{
    uint32_t prio, code;
    int x, y;
};

/*
 * Scene implementation class
 */

class SceneData
{
    friend class Scene;

private:
    static int Compare(void const *p1, void const *p2)
    {
        Tile const *t1 = (Tile const *)p1;
        Tile const *t2 = (Tile const *)p2;

        return t2->prio - t1->prio;
    }

    Tile *tiles;
    int ntiles;
};

/*
 * Public Scene class
 */

Scene::Scene()
{
    data = new SceneData();
    data->tiles = 0;
    data->ntiles = 0;
}

Scene::~Scene()
{
    delete data;
}

void Scene::AddTile(uint32_t code, int x, int y, int z, int o)
{
    if ((data->ntiles % 1024) == 0)
        data->tiles = (Tile *)realloc(data->tiles,
                                      (data->ntiles + 1024) * sizeof(Tile));
    data->tiles[data->ntiles].prio = y - 2 * 32 * z + (o ? 0 : 32);
    data->tiles[data->ntiles].code = code;
    data->tiles[data->ntiles].x = x;
    data->tiles[data->ntiles].y = y;
    data->ntiles++;
}

void Scene::Render() // XXX: rename to Blit()
{
    qsort(data->tiles, data->ntiles, sizeof(Tile), SceneData::Compare);

    for (int i = 0; i < data->ntiles; i++)
        Tiler::Render(data->tiles[i].code, data->tiles[i].x, data->tiles[i].y);

    free(data->tiles);
    data->tiles = 0;
    data->ntiles = 0;
}

