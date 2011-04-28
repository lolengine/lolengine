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

#include "core.h"
#include "lolgl.h"
#include "loldebug.h"

using namespace std;

namespace lol
{

/*
 * DebugQuad implementation class
 */

class DebugQuadData
{
    friend class DebugQuad;

private:
    int initialised;
    float time;
    GLuint buflist[3];
    Shader *shader;
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

    if (IsDestroying())
    {
        if (data->initialised)
        {
            glDeleteBuffers(3, data->buflist);
            Shader::Destroy(data->shader);
            glDeleteTextures(1, data->texlist);
            data->initialised = 0;
        }
    }
    else if (!data->initialised)
    {
        glGenBuffers(3, data->buflist);

        static char const *vertexshader =
            "#version 130\n"
            "in vec2 in_Position;\n"
            "in vec4 in_Color;\n"
            "in vec2 in_TexCoord;\n"
            "out vec4 pass_Color;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = vec4(in_Position, 0.0f, 1.0f);\n"
            "    gl_TexCoord[0] = vec4(in_TexCoord, 0.0, 0.0);\n"
            "    pass_Color = in_Color;\n"
            "}\n";
        static char const *fragmentshader =
            "#version 130\n"
            "in vec4 pass_Color;\n"
            "uniform sampler2D in_Texture;\n"
            "void main()\n"
            "{\n"
            "    vec4 col = pass_Color;\n"
            "    vec4 tex = texture2D(in_Texture, vec2(gl_TexCoord[0]));\n"
            "    gl_FragColor = col * tex;\n"
            "}\n";
        data->shader = Shader::Create(vertexshader, fragmentshader);
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

        data->initialised = 1;
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

    data->shader->Bind();
    GLuint attr_pos, attr_col, attr_tex;
    attr_pos = data->shader->GetAttribLocation("in_Position");
    attr_col = data->shader->GetAttribLocation("in_Color");
    attr_tex = data->shader->GetAttribLocation("in_TexCoord");

#if !defined __CELLOS_LV2__
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
}

DebugQuad::~DebugQuad()
{
    delete data;
}

} /* namespace lol */

