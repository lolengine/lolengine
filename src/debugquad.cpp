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

#define NUM_ARRAYS 2
#define NUM_BUFFERS 6
#define NUM_ATTRS 6
#define NUM_SHADERS 6
#define NUM_TEXTURES 1

class DebugQuadData
{
    friend class DebugQuad;

private:
    vec2 orig, step, aa, bb;

    int initialised;
    float time;
    GLuint array[NUM_ARRAYS];
    GLuint buffer[NUM_BUFFERS];
    Shader *shader[NUM_SHADERS];
    GLuint attr[NUM_ATTRS];
    GLuint texture[NUM_TEXTURES];
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
        glGenVertexArrays(NUM_ARRAYS, data->array);
        glGenBuffers(NUM_BUFFERS, data->buffer);
#if 0

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
        glGenTextures(1, data->texture);
#endif

        /* Checkerboard texture */
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, data->texture[0]);
        for (int j = 0; j < 32; j++)
            for (int i = 0; i < 32; i++)
            {
                uint8_t wb = (((i / 2) ^ (j / 2)) & 1) * 0xff;
                data->image[0][(j * 32 + i) * 4 + 0] = wb;
                data->image[0][(j * 32 + i) * 4 + 1] = wb;
                data->image[0][(j * 32 + i) * 4 + 2] = wb;
                data->image[0][(j * 32 + i) * 4 + 3] = 0xff;
            }
        /* Use GL_RGBA instead of 4 for the internal format (Android) */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, data->image[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        /* Quad #4: create texture in 1.10 fragment shader */
        data->shader[0] = Shader::Create(
            "#version 110\n"
            "void main()"
            "{"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 110\n"
            "void main()"
            "{"
            "    float dx = mod(gl_FragCoord.x * gl_FragCoord.y, 2.0);"
            "    float dy = mod(gl_FragCoord.x * 0.125, 1.0);"
            "    float dz = mod(gl_FragCoord.y * 0.125, 1.0);"
            "    gl_FragColor = vec4(dx, dy, dz, 1.0);"
            "}");

        /* Quad #5: pass color from 1.10 vertex shader to fragment shader */
        data->shader[1] = Shader::Create(
            "#version 110\n"
            "varying vec4 pass_Color;"
            "void main()"
            "{"
            "    float r = gl_MultiTexCoord0.x;"
            "    float g = gl_MultiTexCoord0.y;"
            "    pass_Color = vec4(1.0 - r, 1.0 - g, r, 1.0);"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 110\n"
            "varying vec4 pass_Color;"
            "void main()"
            "{"
            "    gl_FragColor = pass_Color;"
            "}");

        /* Quad #6: apply texture in 1.10 fragment shader */
        data->shader[2] = Shader::Create(
            "#version 110\n"
            "void main()"
            "{"
            "    gl_TexCoord[0] = gl_MultiTexCoord0;"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 110\n"
            "uniform sampler2D tex;"
            "void main()"
            "{"
            "    gl_FragColor = texture2D(tex, gl_TexCoord[0].xy * 0.25);"
            "}");

        /* Quad #8: vertex buffer, apply texture in 1.10 fragment shader */
        data->shader[3] = Shader::Create(
            "#version 110\n"
            "varying vec4 pass_Color;"
            "void main()"
            "{"
            "    gl_TexCoord[0] = gl_MultiTexCoord0;"
            "    pass_Color = gl_Color;"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 110\n"
            "varying vec4 pass_Color;"
            "uniform sampler2D tex;"
            "void main()"
            "{"
            "    vec4 tmp = texture2D(tex, gl_TexCoord[0].xy * 0.25);"
            "    gl_FragColor = vec4(abs(tmp.xyz - pass_Color.xyz), 1.0);"
            "}");

        /* Quad #9: vertex buffer, apply texture and color in 1.20 shader */
        data->shader[4] = Shader::Create(
            "#version 120\n"
            "attribute vec3 in_Vertex;"
            "attribute vec3 in_Color;"
            "attribute vec2 in_MultiTexCoord0;"
            "varying vec4 pass_Color;"
            "void main()"
            "{"
            "    gl_TexCoord[0] = vec4(in_MultiTexCoord0, 0.0, 0.0);"
            "    pass_Color = vec4(in_Color, 1.0);"
            "    gl_Position = vec4(in_Vertex, 1.0);"
            "}",

            "#version 120\n"
            "varying vec4 pass_Color;"
            "uniform sampler2D tex;"
            "void main()"
            "{"
            "    vec4 tmp = texture2D(tex, gl_TexCoord[0].xy * 0.25);"
            "    gl_FragColor = vec4(abs(tmp.xyz - pass_Color.xyz), 1.0);"
            "}");
        data->attr[0] = data->shader[4]->GetAttribLocation("in_Vertex");
        data->attr[1] = data->shader[4]->GetAttribLocation("in_Color");
        data->attr[2] = data->shader[4]->GetAttribLocation("in_MultiTexCoord0");

        /* Quad #10: vertex buffer, apply texture and color in 1.30 shader */
        data->shader[5] = Shader::Create(
            "#version 130\n"
            "in vec3 in_Vertex;"
            "in vec3 in_Color;"
            "in vec2 in_MultiTexCoord0;"
            "out vec4 pass_Color;"
            "void main()"
            "{"
            "    gl_TexCoord[0] = vec4(in_MultiTexCoord0, 0.0, 0.0);"
            "    pass_Color = vec4(in_Color, 1.0);"
            "    gl_Position = vec4(in_Vertex, 1.0);"
            "}",

            "#version 130\n"
            "in vec4 pass_Color;"
            "uniform sampler2D tex;"
            "void main()"
            "{"
            "    vec4 tmp = texture2D(tex, gl_TexCoord[0].xy * 0.25);"
            "    gl_FragColor = vec4(abs(tmp.xyz - pass_Color.xyz), 1.0);"
            "}");
        data->attr[3] = data->shader[4]->GetAttribLocation("in_Vertex");
        data->attr[4] = data->shader[4]->GetAttribLocation("in_Color");
        data->attr[5] = data->shader[4]->GetAttribLocation("in_MultiTexCoord0");

        data->initialised = 1;
    }
    else if (data->initialised && IsDestroying())
    {
        glDeleteVertexArrays(NUM_ARRAYS, data->array);
        glDeleteBuffers(NUM_BUFFERS, data->buffer);
        Shader::Destroy(data->shader[0]);
        Shader::Destroy(data->shader[1]);
        Shader::Destroy(data->shader[2]);
        Shader::Destroy(data->shader[3]);
        glDeleteTextures(NUM_TEXTURES, data->texture);

        data->initialised = 0;
    }

#if 0
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
    glBindTexture(GL_TEXTURE_2D, data->texture[0]);

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
    glBindTexture(GL_TEXTURE_2D, data->texture[0]);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[0]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(attr_pos, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[1]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cols, GL_STATIC_DRAW);
    glVertexAttribPointer(attr_col, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[2]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), tcs, GL_STATIC_DRAW);
    glVertexAttribPointer(attr_tex, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(attr_pos);
    glDisableVertexAttribArray(attr_col);
    glDisableVertexAttribArray(attr_tex);
#endif
#endif

    /* Reset GL states */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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

    GLfloat const colors[] = { f1, f2, f3, f4, f2, f1, f3, f1, f4,
                               f3, f1, f4, f4, f3, f2, f1, f2, f3 };
    GLfloat const texcoords[] = { f1, f3, f3, f2, f2, f4,
                                  f2, f4, f4, f1, f1, f3 };

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
    glBindTexture(GL_TEXTURE_2D, data->texture[0]);
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
    glDisable(GL_TEXTURE_2D);

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
    glBindTexture(GL_TEXTURE_2D, data->texture[0]);
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
    glDisable(GL_TEXTURE_2D);

    Advance();

    /* Quad #7: simple vertex buffer */
    GLfloat const vertices1[] = { data->aa.x, data->bb.y, 0.0f,
                                  data->bb.x, data->bb.y, 0.0f,
                                  data->bb.x, data->aa.y, 0.0f,
                                  data->bb.x, data->aa.y, 0.0f,
                                  data->aa.x, data->aa.y, 0.0f,
                                  data->aa.x, data->bb.y, 0.0f };

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(3, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices1);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glUseProgram(0);

    Advance();

    /* Quad #8: vertex buffer, apply texture and color in fragment shader */
    data->shader[3]->Bind();
    GLfloat const vertices2[] = { data->aa.x, data->bb.y, 0.0f,
                                  data->bb.x, data->bb.y, 0.0f,
                                  data->bb.x, data->aa.y, 0.0f,
                                  data->bb.x, data->aa.y, 0.0f,
                                  data->aa.x, data->aa.y, 0.0f,
                                  data->aa.x, data->bb.y, 0.0f };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices2);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glUseProgram(0);

    Advance();

    /* Quad #9: vertex buffer, apply texture and color in 1.20 shader */
    data->shader[4]->Bind();
    GLfloat const vertices3[] = { data->aa.x, data->bb.y, 0.0f,
                                  data->bb.x, data->bb.y, 0.0f,
                                  data->bb.x, data->aa.y, 0.0f,
                                  data->bb.x, data->aa.y, 0.0f,
                                  data->aa.x, data->aa.y, 0.0f,
                                  data->aa.x, data->bb.y, 0.0f };

    glBindVertexArray(data->array[0]);

    glBindBuffer(GL_ARRAY_BUFFER, data->buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(data->attr[0], 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(data->attr[0]);

    glBindBuffer(GL_ARRAY_BUFFER, data->buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(data->attr[1], 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(data->attr[1]);

    glBindBuffer(GL_ARRAY_BUFFER, data->buffer[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(data->attr[2], 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(data->attr[2]);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDisableVertexAttribArray(data->attr[0]);
    glDisableVertexAttribArray(data->attr[1]);
    glDisableVertexAttribArray(data->attr[2]);
    glUseProgram(0);

    Advance();

    /* Quad #10: vertex buffer, apply texture and color in 1.30 shader */
    data->shader[5]->Bind();
    GLfloat const vertices4[] = { data->aa.x, data->bb.y, 0.0f,
                                  data->bb.x, data->bb.y, 0.0f,
                                  data->bb.x, data->aa.y, 0.0f,
                                  data->bb.x, data->aa.y, 0.0f,
                                  data->aa.x, data->aa.y, 0.0f,
                                  data->aa.x, data->bb.y, 0.0f };

    glBindVertexArray(data->array[1]);

    glBindBuffer(GL_ARRAY_BUFFER, data->buffer[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices4), vertices4,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(data->attr[3], 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(data->attr[3]);

    glBindBuffer(GL_ARRAY_BUFFER, data->buffer[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(data->attr[4], 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(data->attr[4]);

    glBindBuffer(GL_ARRAY_BUFFER, data->buffer[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(data->attr[5], 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(data->attr[5]);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDisableVertexAttribArray(data->attr[3]);
    glDisableVertexAttribArray(data->attr[4]);
    glDisableVertexAttribArray(data->attr[5]);
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

