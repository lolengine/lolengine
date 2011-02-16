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

#define ATTRIB_POSITION 42 /* arbitrary id */
#define SHADER_CRAP 0

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

#if SHADER_CRAP
    GLuint prog, sh1, sh2;
    GLint uni_mvp, uni_color;
#endif
    GLuint *bufs;
    int nbufs;

    static Scene *scene;
};

Scene *SceneData::scene = NULL;

#if SHADER_CRAP
static char const *vertexshader =
    "attribute vec4 position;\n"
    "uniform mat4 mvp;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = mvp * position;\n"
    "}\n";

static char const *fragmentshader =
    "uniform /*lowp*/ vec4 color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = color;\n"
    "}\n";
#endif

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

#if SHADER_CRAP
    data->sh1 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(data->sh1, 1, &vertexshader, NULL);
    glCompileShader(data->sh1);

    char buf[4096];
    GLsizei dummy;
    glGetShaderInfoLog(data->sh1, 4096, &dummy, buf);
    fprintf(stderr, "sh1 %i: %s", data->sh1, buf);

    data->sh2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(data->sh2, 1, &fragmentshader, NULL);
    glCompileShader(data->sh2);

    glGetShaderInfoLog(data->sh2, 4096, &dummy, buf);
    fprintf(stderr, "sh2 %i: %s", data->sh2, buf);

    data->prog = glCreateProgram();
    glAttachShader(data->prog, data->sh1);
    glAttachShader(data->prog, data->sh2);

    glBindAttribLocation(data->prog, ATTRIB_POSITION, "position");
    glLinkProgram(data->prog);
    glValidateProgram(data->prog);

    data->uni_mvp = glGetUniformLocation(data->prog, "mvp");
    data->uni_color = glGetUniformLocation(data->prog, "color");
#endif
}

Scene::~Scene()
{
    /* FIXME: this must be done while the GL context is still active.
     * Change the architecture to make sure of that. */
#if SHADER_CRAP
    glDetachShader(data->prog, data->sh1);
    glDetachShader(data->prog, data->sh2);
    glDeleteShader(data->sh1);
    glDeleteShader(data->sh2);
    glDeleteProgram(data->prog);
#endif
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
#if SHADER_CRAP
        glUseProgram(data->prog);
        float4x4 mvp = float4x4::identity();
        mvp = mvp - mvp;
        glUniformMatrix4fv(data->uni_mvp, 1, GL_FALSE, (GLfloat *)&mvp[0][0]);
        glUniform4f(data->uni_color, 1.0f, 0.0f, 1.0f, 1.0f);
#endif

        glBindBuffer(GL_ARRAY_BUFFER, data->bufs[buf]);
        glBufferData(GL_ARRAY_BUFFER, 6 * 3 * (n - i) * sizeof(float),
                     vertex, GL_DYNAMIC_DRAW);
        glVertexPointer(3, GL_FLOAT, 0, NULL);
#if SHADER_CRAP
        glVertexAttribPointer(ATTRIB_POSITION, (n - i) * 6, GL_FLOAT, false, 0, vertex);
        glEnableVertexAttribArray(ATTRIB_POSITION);
#endif

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

    free(data->tiles);
    data->tiles = 0;
    data->ntiles = 0;
}

