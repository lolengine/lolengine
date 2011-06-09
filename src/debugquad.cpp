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

#include <cmath>
#include <cstdio>

#include "core.h"
#include "lolgl.h"
#include "loldebug.h"

using namespace std;

namespace lol
{

/*
 * DebugQuad implementation class
 */

#define NUM_SHADERS 3

class DebugQuadData
{
    friend class DebugQuad;

private:
    vec2 orig, step, aa, bb;

    int initialised;
    float time;
    GLuint buflist[3];
    Shader *shader[NUM_SHADERS];
    GLuint texlist[1];
    uint8_t image[1][32 * 32 * 4];
};

/*
 * Public DebugQuad class
 */

DebugQuad::DebugQuad()
  : data(new DebugQuadData())
{
    data->initialised = 0;
    data->time = RandF(10000.0f);

    drawgroup = DRAWGROUP_HUD;
}

void DebugQuad::TickGame(float deltams)
{
    Entity::TickGame(deltams);

    data->time += deltams;
}

void DebugQuad::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    if (!data->initialised && !IsDestroying())
    {
        glGenBuffers(3, data->buflist);

        static char const *vertexshader =
            "//#version 130\n"
            "varying vec2 in_Position;\n"
            "varying vec4 in_Color;\n"
            "varying vec2 in_TexCoord;\n"
            "varying vec4 pass_Color;\n"
            "void main()\n"
            "{\n"
            "gl_TexCoord[0] = gl_MultiTexCoord0;\n"
            "    gl_Position = vec4(in_Position, 0.0f, 1.0f);\n"
            "    gl_TexCoord[0] = vec4(in_TexCoord, 0.0, 0.0);\n"
            "    pass_Color = in_Color;\n"
            "}\n";
        static char const *fragmentshader =
            "//#version 130\n"
            "varying vec4 pass_Color;\n"
            "uniform sampler2D in_Texture;\n"
            "void main()\n"
            "{\n"
            "    vec4 col = pass_Color;\n"
            "    vec4 tex = texture2D(in_Texture, vec2(gl_TexCoord[0]));\n"
            "    gl_FragColor = col * tex;\n"
            "    gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);\n"
            "}\n";
        data->shader[0] = Shader::Create(vertexshader, fragmentshader);
        glGenTextures(1, data->texlist);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, data->texlist[0]);
        for (int j = 0; j < 32; j++)
            for (int i = 0; i < 32; i++)
            {
                uint8_t wb = (((i / 2) ^ (j / 2)) & 1) * 0xff;
                data->image[0][(j * 32 + i) * 4 + 0] = wb;
                data->image[0][(j * 32 + i) * 4 + 1] = wb;
                data->image[0][(j * 32 + i) * 4 + 2] = wb;
                data->image[0][(j * 32 + i) * 4 + 3] = wb;
            }
        /* Use GL_RGBA instead of 4 for the internal format (Android) */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, data->image[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        /* Quad #4: create texture in fragment shader */
        data->shader[0] = Shader::Create(
            "#version 120\n"
            "void main()"
            "{"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 120\n"
            "void main()"
            "{"
            "    float dx = mod(gl_FragCoord.x * gl_FragCoord.y, 2.0);"
            "    float dy = mod(gl_FragCoord.x * 0.125, 1.0);"
            "    float dz = mod(gl_FragCoord.y * 0.125, 1.0);"
            "    gl_FragColor = vec4(dx, dy, dz, 1.0);"
            "}");

        /* Quad #5: pass color from vertex shader to fragment shader */
        data->shader[1] = Shader::Create(
            "#version 120\n"
            "varying vec4 color;"
            "void main()"
            "{"
            "    float r = gl_MultiTexCoord0.x;"
            "    float g = gl_MultiTexCoord0.y;"
            "    color = vec4(1.0 - r, 1.0 - g, r, 1.0);"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 120\n"
            "varying vec4 color;"
            "void main()"
            "{"
            "    gl_FragColor = color;"
            "}");

        /* Quad #6: apply texture in fragment shader */
        data->shader[2] = Shader::Create(
            "void main()"
            "{"
            "    gl_TexCoord[0] = gl_MultiTexCoord0;"
            "    gl_Position = gl_Vertex;"
            "}",

            "uniform sampler2D tex;"
            "void main()"
            "{"
            "    gl_FragColor = texture2D(tex, gl_TexCoord[0].xy * 0.25);"
            "}");

        data->initialised = 1;
    }
    else if (data->initialised && IsDestroying())
    {
        glDeleteBuffers(3, data->buflist);
        Shader::Destroy(data->shader[0]);
        Shader::Destroy(data->shader[1]);
        Shader::Destroy(data->shader[2]);
        glDeleteTextures(1, data->texlist);

        data->initialised = 0;
    }

    float const st = sinf(0.0005f * data->time);
    float const ct = cosf(0.0005f * data->time);

    GLfloat const verts[6][2] =
    {
        { -0.7f * (st + ct),  0.7f * (st - ct) },
        {  0.7f * (st - ct),  0.7f * (st + ct) },
        { -0.7f * (st - ct), -0.7f * (st + ct) },

        { -0.7f * (st - ct), -0.7f * (st + ct) },
        {  0.7f * (st - ct),  0.7f * (st + ct) },
        {  0.7f * (st + ct), -0.7f * (st - ct) },
    };

    /* Using only 3 components breaks on Android for some reason. */
    static GLfloat const cols[6][4] =
    {
        { 1.0f, 0.2f, 0.2f, 1.0f },
        { 0.2f, 0.2f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 0.2f, 1.0f },

        { 1.0f, 1.0f, 0.2f, 1.0f },
        { 0.2f, 0.2f, 1.0f, 1.0f },
        { 0.2f, 1.0f, 0.2f, 1.0f },
    };

    static GLfloat const tcs[6][2] =
    {
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f },
        { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
    };

#if defined __CELLOS_LV2__
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, data->texlist[0]);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[0]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), verts, GL_STATIC_DRAW);
    glVertexPointer(2, GL_FLOAT, GL_FALSE, 0);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[1]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cols, GL_STATIC_DRAW);
    glColorPointer(4, GL_FLOAT, GL_FALSE, 0);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[2]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), tcs, GL_STATIC_DRAW);
    glTexCoordPointer(2, GL_FLOAT, GL_FALSE, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else
    data->shader[0]->Bind();
    GLuint attr_pos, attr_col, attr_tex;
    attr_pos = data->shader[0]->GetAttribLocation("in_Position");
    attr_col = data->shader[0]->GetAttribLocation("in_Color");
    attr_tex = data->shader[0]->GetAttribLocation("in_TexCoord");

    glEnableVertexAttribArray(attr_pos);
    glEnableVertexAttribArray(attr_col);
    glEnableVertexAttribArray(attr_tex);

    /* Bind texture */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, data->texlist[0]);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[0]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(attr_pos, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[1]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cols, GL_STATIC_DRAW);
    glVertexAttribPointer(attr_col, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[2]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), tcs, GL_STATIC_DRAW);
    glVertexAttribPointer(attr_tex, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(attr_pos);
    glDisableVertexAttribArray(attr_col);
    glDisableVertexAttribArray(attr_tex);
#endif

    /* Reset GL states */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glUseProgram(0);

    /* Prepare our quad coordinates */
    vec2i const layout(4, 3);
    data->step = vec2(2.0f, -2.0f) / (3 * layout + vec2i(1));
    data->orig = vec2(-1.0f, 1.0f) + data->step;
    data->aa = data->orig;
    data->bb = data->orig + 2.0f * data->step;

    /* Generate a few random numbers */
    float f1 = 0.5f + 0.5f * sinf(0.00034f * data->time);
    float f2 = 0.5f + 0.5f * sinf(0.00053f * data->time + 1.0f);
    float f3 = 0.5f + 0.5f * sinf(0.00072f * data->time + 4.0f);
    float f4 = 0.5f + 0.5f * sinf(0.00091f * data->time + 8.0f);

    /* Quad #1: simple glBegin program */
    glColor3f(0.8f, 0.5f, 0.2f);
    glBegin(GL_TRIANGLES);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
        glVertex3f(data->bb.x, data->bb.y, 0.0f);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);

        glVertex3f(data->bb.x, data->aa.y, 0.0f);
        glVertex3f(data->aa.x, data->aa.y, 0.0f);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
    glEnd();

    Advance();

    /* Quad #2: glBegin program with varying color */
    glBegin(GL_TRIANGLES);
        glColor3f(f1, f2, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
        glColor3f(f4, f2, f1);
        glVertex3f(data->bb.x, data->bb.y, 0.0f);
        glColor3f(f3, f1, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);

        glVertex3f(data->bb.x, data->aa.y, 0.0f);
        glColor3f(f4, f3, f2);
        glVertex3f(data->aa.x, data->aa.y, 0.0f);
        glColor3f(f1, f2, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
    glEnd();

    Advance();

    /* Quad #3: textured quad */
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, data->texlist[0]);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(f1, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
        glTexCoord2f(f3, f2);
        glVertex3f(data->bb.x, data->bb.y, 0.0f);
        glTexCoord2f(f2, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);

        glTexCoord2f(f2, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);
        glTexCoord2f(f4, f1);
        glVertex3f(data->aa.x, data->aa.y, 0.0f);
        glTexCoord2f(f1, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
    glEnd();

    Advance();

    /* Quad #4: set color in fragment shader */
    data->shader[0]->Bind();
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
        glVertex3f(data->bb.x, data->bb.y, 0.0f);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);

        glVertex3f(data->bb.x, data->aa.y, 0.0f);
        glVertex3f(data->aa.x, data->aa.y, 0.0f);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
    glEnd();
    glUseProgram(0);

    Advance();

    /* Quad #5: pass color from vertex shader to fragment shader */
    data->shader[1]->Bind();
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(f1, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
        glTexCoord2f(f3, f2);
        glVertex3f(data->bb.x, data->bb.y, 0.0f);
        glTexCoord2f(f2, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);

        glTexCoord2f(f2, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);
        glTexCoord2f(f4, f1);
        glVertex3f(data->aa.x, data->aa.y, 0.0f);
        glTexCoord2f(f1, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
    glEnd();
    glUseProgram(0);

    Advance();

    /* Quad #6: apply texture in fragment shader */
    data->shader[2]->Bind();
    glColor3f(0.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, data->texlist[0]);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(f1, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
        glTexCoord2f(f3, f2);
        glVertex3f(data->bb.x, data->bb.y, 0.0f);
        glTexCoord2f(f2, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);

        glTexCoord2f(f2, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);
        glTexCoord2f(f4, f1);
        glVertex3f(data->aa.x, data->aa.y, 0.0f);
        glTexCoord2f(f1, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
    glEnd();
    glUseProgram(0);

    Advance();

}

void DebugQuad::Advance()
{
    data->aa.x += 3.0f * data->step.x;
    data->bb.x += 3.0f * data->step.x;
    if (data->bb.x > 1.0f)
    {
        data->aa.x = data->orig.x;
        data->bb.x = data->orig.x + 2.0f * data->step.x;
        data->aa.y += 3.0f * data->step.y;
        data->bb.y += 3.0f * data->step.y;
    }
}

DebugQuad::~DebugQuad()
{
    delete data;
}

} /* namespace lol */

