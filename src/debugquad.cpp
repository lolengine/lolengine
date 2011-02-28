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

#include <cstdio>
#include <cmath>

#include "core.h"
#include "lolgl.h"
#include "loldebug.h"

namespace lol
{

/*
 * DebugTri implementation class
 */

class DebugTriData
{
    friend class DebugTri;

private:
    int initialised;
#if defined HAVE_GL_2X || defined HAVE_GLES_2X
    GLuint buflist[2];
    Shader *shader;
#elif defined HAVE_GL_1X || defined HAVE_GLES_1X
    GLuint buflist[2];
#endif
};

/*
 * Public DebugTri class
 */

DebugTri::DebugTri()
  : data(new DebugTriData())
{
    data->initialised = 0;
}

void DebugTri::TickGame(float deltams)
{
    Entity::TickGame(deltams);
}

void DebugTri::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    if (IsDestroying())
    {
        if (data->initialised)
        {
#if defined HAVE_GL_2X || defined HAVE_GLES_2X
            glDeleteBuffers(2, data->buflist);
            Shader::Destroy(data->shader);
#elif defined HAVE_GL_1X || defined HAVE_GLES_1X
            glDeleteBuffers(2, data->buflist);
#endif
            data->initialised = 0;
        }
    }
    else if (!data->initialised)
    {
#if defined HAVE_GL_2X || defined HAVE_GLES_2X
        glGenBuffers(2, data->buflist);

        static char const *vertexshader =
            "#version 130\n"
            "in vec2 in_Position;\n"
            "in vec3 in_Color;\n"
            "out vec3 pass_Color;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = vec4(in_Position, 0.0f, 1.0f);\n"
            "    pass_Color = in_Color;\n"
            "}\n";
        static char const *fragmentshader =
            "#version 130\n"
            "in vec3 pass_Color;\n"
            "void main()\n"
            "{\n"
            "    gl_FragColor = vec4(pass_Color, 1.0);\n"
            "}\n";
        data->shader = Shader::Create(vertexshader, fragmentshader);
#elif defined HAVE_GL_1X || defined HAVE_GLES_1X
        glGenBuffers(2, data->buflist);
#endif

        data->initialised = 1;
    }

    static GLfloat const verts[6][2] =
    {
        { -0.9f,  0.9f }, {  0.9f,  0.9f }, { -0.9f, -0.9f },
        { -0.9f, -0.9f }, {  0.9f,  0.9f }, {  0.9f, -0.9f },
    };

    static GLfloat const cols[6][4] =
    {
        { 0.8f, 0.2f, 0.2f, 1.0f }, { 0.2f, 0.2f, 0.8f, 1.0f }, { 0.8f, 0.8f, 0.2f, 1.0f },
        { 0.8f, 0.8f, 0.2f, 1.0f }, { 0.2f, 0.2f, 0.8f, 1.0f }, { 0.2f, 0.8f, 0.2f, 1.0f },
    };

#if defined HAVE_GL_2X || defined HAVE_GLES_2X
    data->shader->Bind();
    GLuint attr_pos, attr_col;
    attr_pos = data->shader->GetAttribLocation("in_Position");
    attr_col = data->shader->GetAttribLocation("in_Color");

    glEnableVertexAttribArray(attr_pos);
    glEnableVertexAttribArray(attr_col);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[0]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(attr_pos, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[1]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cols, GL_STATIC_DRAW);
    glVertexAttribPointer(attr_col, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(attr_pos);
    glDisableVertexAttribArray(attr_col);
#elif defined HAVE_GL_1X || defined HAVE_GLES_1X
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

#if defined HAVE_GL_1X
    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[0]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), verts, GL_STATIC_DRAW);
    glVertexPointer(2, GL_FLOAT, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[1]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cols, GL_STATIC_DRAW);
    glColorPointer(4, GL_FLOAT, 0, NULL);
#else
    glVertexPointer(2, GL_FLOAT, 0, verts);
    glColorPointer(4, GL_FLOAT, 0, cols);
#endif

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
#endif
}

DebugTri::~DebugTri()
{
    delete data;
}

} /* namespace lol */

