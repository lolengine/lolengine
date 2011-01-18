//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <cmath>

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif
#if defined __APPLE__ && defined __MACH__
#   include <OpenGL/gl.h>
#else
#   define GL_GLEXT_PROTOTYPES
#   include <GL/gl.h>
#endif

#include "core.h"

struct Tile
{
    uint32_t prio, code;
    int x, y, z, o;
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
    float angle;
};

/*
 * Public Scene class
 */

Scene::Scene(float angle)
{
    data = new SceneData();
    data->tiles = 0;
    data->ntiles = 0;
    data->angle = angle;
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
    data->tiles[data->ntiles].prio = -y - 2 * 32 * z + (o ? 0 : 32);
    data->tiles[data->ntiles].code = code;
    data->tiles[data->ntiles].x = x;
    data->tiles[data->ntiles].y = y;
    data->tiles[data->ntiles].z = z;
    data->tiles[data->ntiles].o = o;
    data->ntiles++;
}

void Scene::Render() // XXX: rename to Blit()
{
#if 0
    // Randomise, then sort.
    for (int i = 0; i < data->ntiles; i++)
    {
        Tile tmp = data->tiles[i];
        int j = rand() % data->ntiles;
        data->tiles[i] = data->tiles[j];
        data->tiles[j] = tmp;
    }
#endif
    qsort(data->tiles, data->ntiles, sizeof(Tile), SceneData::Compare);

    // XXX: debug stuff
    glPushMatrix();
    static float f = 0.0f;
    f += 0.05f;
    glTranslatef(320.0f, 240.0f, 0.0f);
    glRotatef(-data->angle, 1.0f, 0.0f, 0.0f);
#if 0
    glRotatef(3.0f * sinf(f), 1.0f, 0.0f, 0.0f);
    glRotatef(8.0f * cosf(f), 0.0f, 0.0f, 1.0f);
#endif
    glTranslatef(-320.0f, -240.0f, 0.0f);

    for (int i = 0; i < data->ntiles; i++)
        Tiler::BlitTile(data->tiles[i].code, data->tiles[i].x,
                        data->tiles[i].y, data->tiles[i].z, data->tiles[i].o);

    glPopMatrix();
    // XXX: end of debug stuff

    free(data->tiles);
    data->tiles = 0;
    data->ntiles = 0;
}

