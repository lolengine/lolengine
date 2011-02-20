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

#include "core.h"
#include "lolgl.h"

struct Tile
{
    uint32_t prio, code;
    int x, y, z, o;
};

#if LOL_EXPERIMENTAL
extern Shader *stdshader;
#endif
extern mat4 model_matrix;

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

#if LOL_EXPERIMENTAL
    GLuint vao;
#endif
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

#if LOL_EXPERIMENTAL
    glGenVertexArrays(1, &data->vao);
#endif
}

Scene::~Scene()
{
    /* FIXME: this must be done while the GL context is still active.
     * Change the architecture to make sure of that. */
    glDeleteBuffers(data->nbufs, data->bufs);
#if LOL_EXPERIMENTAL
    glDeleteVertexArrays(1, &data->vao);
#endif
    free(data->bufs);
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

    // XXX: debug stuff
    model_matrix = mat4::translate(320.0f, 240.0f, 0.0f);
    model_matrix *= mat4::rotate(-data->angle, 1.0f, 0.0f, 0.0f);
#if 0
    static float f = 0.0f;
    f += 0.01f;
    model_matrix *= mat4::rotate(0.1f * sinf(f), 1.0f, 0.0f, 0.0f);
    model_matrix *= mat4::rotate(0.3f * cosf(f), 0.0f, 0.0f, 1.0f);
#endif
    model_matrix *= mat4::translate(-320.0f, -240.0f, 0.0f);
    // XXX: end of debug stuff

#if LOL_EXPERIMENTAL
    GLuint uni, attr_pos, attr_tex;
    uni = stdshader->GetUniformLocation("model_matrix");
    attr_pos = stdshader->GetAttribLocation("in_Position");
    attr_tex = stdshader->GetAttribLocation("in_TexCoord");

    glUniformMatrix4fv(uni, 1, GL_FALSE, &model_matrix[0][0]);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.01f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#else
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.01f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLoadIdentity();
    glMultMatrixf(&model_matrix[0][0]);
#endif

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

#if LOL_EXPERIMENTAL
        glBindVertexArray(data->vao);
#else
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

        glBindBuffer(GL_ARRAY_BUFFER, data->bufs[buf]);
        glBufferData(GL_ARRAY_BUFFER, 18 * (n - i) * sizeof(GLfloat),
                     vertex, GL_STATIC_DRAW);
#if LOL_EXPERIMENTAL
        glVertexAttribPointer(attr_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(attr_pos);
#else
        glVertexPointer(3, GL_FLOAT, 0, NULL);
#endif

        glBindBuffer(GL_ARRAY_BUFFER, data->bufs[buf + 1]);
        glBufferData(GL_ARRAY_BUFFER, 12 * (n - i) * sizeof(GLfloat),
                     texture, GL_STATIC_DRAW);
#if LOL_EXPERIMENTAL
        glVertexAttribPointer(attr_tex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(attr_tex);
#else
        glTexCoordPointer(2, GL_FLOAT, 0, NULL);
#endif

#if LOL_EXPERIMENTAL
        stdshader->Bind();
#endif

        Tiler::Bind(data->tiles[i].code);
        glDrawArrays(GL_TRIANGLES, 0, (n - i) * 6);

#if LOL_EXPERIMENTAL
        glBindVertexArray(0);
#else
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

        free(vertex);
        free(texture);
    }

    free(data->tiles);
    data->tiles = 0;
    data->ntiles = 0;
}

