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
#include <cstdio>
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

#if SHADER_CRAP
extern GLuint prog;
#endif

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

    GLuint *bufs;
    int nbufs;

    static Scene *scene;
};

Scene *SceneData::scene = NULL;

/*
 * Public Scene class
 */

Scene::Scene(float angle)
  : data(new SceneData())
{
    data->tiles = 0;
    data->ntiles = 0;
    data->angle = angle;

    data->bufs = 0;
    data->nbufs = 0;
}

Scene::~Scene()
{
    /* FIXME: this must be done while the GL context is still active.
     * Change the architecture to make sure of that. */
    glDeleteBuffers(data->nbufs, data->bufs);
    delete data;
}

Scene *Scene::GetDefault()
{
    if (!SceneData::scene)
        SceneData::scene = new Scene(0.0f);
    return SceneData::scene;
}

void Scene::Reset()
{
    if (SceneData::scene)
        delete SceneData::scene;
    SceneData::scene = NULL;
}

void Scene::AddTile(uint32_t code, int x, int y, int z, int o)
{
    if ((data->ntiles % 1024) == 0)
        data->tiles = (Tile *)realloc(data->tiles,
                                      (data->ntiles + 1024) * sizeof(Tile));
    /* FIXME: this sorting only works for a 45-degree camera */
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

#if SHADER_CRAP
    float *vertices = new float[18];
    vertices[0] = -0.5f; vertices[1] = 0.5f; vertices[2] = 0.0f;
    vertices[3] = 0.5f; vertices[4] = 0.5f; vertices[5] = 0.0f;
    vertices[6] = -0.5f; vertices[7] = -0.5f; vertices[8] = 0.0f;

    vertices[9] = 0.5f; vertices[10] = -0.5f; vertices[11] = 0.0f;
    vertices[12] = -0.5f; vertices[13] = -0.5f; vertices[14] = 0.0f;
    vertices[15] = 0.5f; vertices[16] = 0.5f; vertices[17] = 0.0f;

    const GLfloat colors[6][3] = {
    {  0.0,  0.0,  1.0  },
    {  1.0,  0.0,  0.0  },
    {  0.0,  1.0,  0.0  },
    {  1.0,  1.0,  0.0  },
    {  0.0,  1.0,  0.0  },
    {  1.0,  0.0,  0.0  } };

    const GLfloat tex[6][2] = {
    {  0.0,  0.0  },
    {  1.0,  0.0  },
    {  0.0,  1.0  },
    {  1.0,  1.0  },
    {  0.0,  1.0  },
    {  1.0,  0.0  } };

    GLuint vao, vbo[3];
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(3, &vbo[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), tex, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    delete[] vertices;

    glUseProgram(prog);
    glBindVertexArray(vao);
    Tiler::Bind(1 << 16);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

#else
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
    // XXX: end of debug stuff

    for (int buf = 0, i = 0, n; i < data->ntiles; i = n, buf += 2)
    {
        /* Generate new vertex / texture coord buffers if necessary */
        if (buf + 2 > data->nbufs)
        {
            data->bufs = (GLuint *)realloc(data->bufs, (buf + 2) * sizeof(GLuint));
            glGenBuffers(buf + 2 - data->nbufs, data->bufs + data->nbufs);
            data->nbufs = buf + 2;
        }

        /* Count how many quads will be needed */
        for (n = i + 1; n < data->ntiles; n++)
            if (data->tiles[i].code >> 16 != data->tiles[n].code >> 16)
                break;

        /* Create a vertex array object */
        float *vertex = (float *)malloc(6 * 3 * (n - i) * sizeof(float));
        float *texture = (float *)malloc(6 * 2 * (n - i) * sizeof(float));

        for (int j = i; j < n; j++)
        {
            Tiler::BlitTile(data->tiles[j].code, data->tiles[j].x,
                            data->tiles[j].y, data->tiles[j].z, data->tiles[j].o,
                            vertex + 18 * (j - i), texture + 12 * (j - i));
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, data->bufs[buf]);
        glBufferData(GL_ARRAY_BUFFER, 6 * 3 * (n - i) * sizeof(float),
                     vertex, GL_DYNAMIC_DRAW);
        glVertexPointer(3, GL_FLOAT, 0, NULL);

        glBindBuffer(GL_ARRAY_BUFFER, data->bufs[buf + 1]);
        glBufferData(GL_ARRAY_BUFFER, 6 * 2 * (n - i) * sizeof(float),
                     texture, GL_DYNAMIC_DRAW);
        glTexCoordPointer(2, GL_FLOAT, 0, NULL);

        Tiler::Bind(data->tiles[i].code);
        glDrawArrays(GL_TRIANGLES, 0, (n - i) * 6);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        free(vertex);
        free(texture);
    }

    glPopMatrix();
#endif

    free(data->tiles);
    data->tiles = 0;
    data->ntiles = 0;
}

