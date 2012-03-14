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

#if defined WIN32 && !defined _XBOX
#   define _USE_MATH_DEFINES /* for M_PI */
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#include <cmath>

#include "core.h"
#include "lolgl.h"
#include "loldebug.h"

using namespace std;

namespace lol
{

/*
 * DebugSphere implementation class
 */

class DebugSphereData
{
    friend class DebugSphere;

    void DrawSphere(int ndiv, GLfloat r, float *&vertex, float *&normal)
    {
        for (int i = 0; i < 20; i++)
            DrawTriangle(vdata[tindices[i][0]],
                         vdata[tindices[i][2]],
                         vdata[tindices[i][1]], ndiv, r, vertex, normal);
    }

    void DrawTriangle(GLfloat const *a, GLfloat const *b, GLfloat const *c,
                      int div, GLfloat r, float *&vertex, float *&normal)
    {
        if (div <= 0)
        {
            *normal++ = a[0]; *normal++ = a[1]; *normal++ = a[2];
            *vertex++ = a[0] * r; *vertex++ = a[1] * r; *vertex++ = a[2] * r;
            *normal++ = b[0]; *normal++ = b[1]; *normal++ = b[2];
            *vertex++ = b[0] * r; *vertex++ = b[1] * r; *vertex++ = b[2] * r;
            *normal++ = c[0]; *normal++ = c[1]; *normal++ = c[2];
            *vertex++ = c[0] * r; *vertex++ = c[1] * r; *vertex++ = c[2] * r;
        }
        else
        {
            GLfloat ab[3], ac[3], bc[3];
            for (int i = 0; i < 3; i++)
            {
                ab[i] = (a[i] + b[i]) * 0.5;
                ac[i] = (a[i] + c[i]) * 0.5;
                bc[i] = (b[i] + c[i]) * 0.5;
            }
            Normalize(ab); Normalize(ac); Normalize(bc);
            DrawTriangle(a, ab, ac, div - 1, r, vertex, normal);
            DrawTriangle(b, bc, ab, div - 1, r, vertex, normal);
            DrawTriangle(c, ac, bc, div - 1, r, vertex, normal);
            DrawTriangle(ab, bc, ac, div - 1, r, vertex, normal);
        }
    }

    void Normalize(GLfloat *a)
    {
        GLfloat d = 1.0f / std::sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
        a[0] *= d; a[1] *= d; a[2] *= d;
    }

private:
    float time;
    int initialised;
    GLuint buflist[2];

    static GLfloat const vdata[12][3];
    static GLuint const tindices[20][3];
    static GLfloat const X, Z;
};

GLfloat const DebugSphereData::X = .525731112119133606;
GLfloat const DebugSphereData::Z = .850650808352039932;

GLfloat const DebugSphereData::vdata[12][3] =
{
    {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
    {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
    {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

GLuint const DebugSphereData::tindices[20][3] =
{
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
};

/*
 * Public DebugSphere class
 */

DebugSphere::DebugSphere()
  : data(new DebugSphereData())
{
    data->time = 0.0f;
    data->initialised = 0;
}

void DebugSphere::TickGame(float deltams)
{
    Entity::TickGame(deltams);

    data->time += 0.003f * deltams;
    while (data->time > 6.0 * M_PI)
        data->time -= 6.0 * M_PI;
}

void DebugSphere::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    if (IsDestroying())
    {
        if (data->initialised)
        {
            glDeleteBuffers(2, data->buflist);
            data->initialised = 0;
        }
    }
    else if (!data->initialised)
    {
        glGenBuffers(2, data->buflist);
        data->initialised = 1;
    }

    float a = sinf(data->time);
#if 0
    float b = sinf(data->time * 0.5f);
#endif

    int const ndiv = 2;
    using std::log;
    int const ntriangles = 20 * (1 << (ndiv * 2))
                              * (int)(log(1.0f / 0.01f) / log(1.1f) + 0.9999f);

    float *vertex = (float *)malloc(ntriangles * 3 * 3 * sizeof(float));
    float *normal = (float *)malloc(ntriangles * 3 * 3 * sizeof(float));

    float *vertex_parser = vertex;
    float *normal_parser = normal;
    for (float t = 0.01f; t < 1.0f; t *= 1.1f)
        data->DrawSphere(ndiv, t * (60.0f + 40.0f * a),
                         vertex_parser, normal_parser);

#if 0 // FIXME: does not work with GLES2
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[0]);
    glBufferData(GL_ARRAY_BUFFER, ntriangles * 3 * 3 * sizeof(float),
                 vertex, GL_DYNAMIC_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[1]);
    glBufferData(GL_ARRAY_BUFFER, ntriangles * 3 * 3 * sizeof(float),
                 normal, GL_DYNAMIC_DRAW);
    glNormalPointer(GL_FLOAT, 0, NULL);

#if 0
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
#endif
    glColor4f(1.0f, b, a, 0.1f);
    glBindTexture(GL_TEXTURE_2D, NULL);

    glTranslatef(320.0f, 240.0f, 32.0f);
    glDrawArrays(GL_TRIANGLES, 0, ntriangles * 3);
    glTranslatef(-320.0f, -240.0f, -32.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
#if 0
    glPopAttrib();
#endif

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

    free(vertex);
    free(normal);
}

DebugSphere::~DebugSphere()
{
    delete data;
}

} /* namespace lol */

