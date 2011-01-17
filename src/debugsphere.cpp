//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#ifdef WIN32
#   define _USE_MATH_DEFINES /* for M_PI */
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif
#if defined __APPLE__ && defined __MACH__
#   include <OpenGL/gl.h>
#else
#   define GL_GLEXT_PROTOTYPES
#   include <GL/gl.h>
#endif

#include <cstdio>
#include <cmath>

#include "core.h"
#include "debugsphere.h"

/*
 * DebugSphere implementation class
 */

class DebugSphereData
{
    friend class DebugSphere;

    void DrawSphere(int ndiv, GLfloat r)
    {
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 20; i++)
            DrawTriangle(vdata[tindices[i][0]],
                         vdata[tindices[i][2]],
                         vdata[tindices[i][1]], ndiv, r);
        glEnd();
    }

    void DrawTriangle(GLfloat const *a, GLfloat const *b, GLfloat const *c,
                      int div, GLfloat r)
    {
        if (div <= 0)
        {
            glNormal3fv(a); glVertex3f(a[0] * r, a[1] * r, a[2] * r);
            glNormal3fv(b); glVertex3f(b[0] * r, b[1] * r, b[2] * r);
            glNormal3fv(c); glVertex3f(c[0] * r, c[1] * r, c[2] * r);
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
            DrawTriangle(a, ab, ac, div - 1, r);
            DrawTriangle(b, bc, ab, div - 1, r);
            DrawTriangle(c, ac, bc, div - 1, r);
            DrawTriangle(ab, bc, ac, div - 1, r);
        }
    }

    void Normalize(GLfloat *a)
    {
        GLfloat d = 1.0f / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
        a[0] *= d; a[1] *= d; a[2] *= d;
    }

private:
    float time;

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
{
    data = new DebugSphereData();
    data->time = 0.0f;
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

    float a = sinf(data->time);
    float b = sinf(data->time * 0.5f);

    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
    glBindTexture(GL_TEXTURE_2D, NULL);
    glColor4f(1.0f, b, a, 0.1f);
    glTranslatef(320.0f, 240.0f, 32.0f);
    for (float t = 0.01f; t < 1.0f; t *= 1.1f)
        data->DrawSphere(2, t * (60.0f + 40.0f * a));
    glTranslatef(-320.0f, -240.0f, -32.0f);
    glPopAttrib();
}

DebugSphere::~DebugSphere()
{
    delete data;
}

