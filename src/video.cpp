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

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#include "core.h"
#include "lolgl.h"

#if LOL_EXPERIMENTAL
Shader *stdshader;
#endif
mat4 proj_matrix, view_matrix, model_matrix;

#if LOL_EXPERIMENTAL
static char const *vertexshader =
    "#version 130\n"
    "\n"
    "in vec3 in_Position;\n"
    //"in vec3 in_Color;\n"
    "in vec2 in_TexCoord;\n"
    //"out vec3 pass_Color;\n"
    "uniform mat4 proj_matrix;\n"
    "uniform mat4 view_matrix;\n"
    "uniform mat4 model_matrix;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = proj_matrix * view_matrix * model_matrix"
    "                * vec4(in_Position, 1.0f);\n"
    "    gl_TexCoord[0] = vec4(in_TexCoord, 0.0, 0.0);\n"
    //"    pass_Color = in_Color;\n"
    "}\n";

static char const *fragmentshader =
    "#version 130\n"
    "\n"
    "uniform sampler2D in_Texture;\n"
    //"in vec3 pass_Color;\n"
    "out vec4 out_Color;\n"
    "\n"
    "void main()\n"
    "{\n"
    //"    gl_FragColor = 0.5 * (texture2D(in_Texture, vec2(gl_TexCoord[0]))\n"
    //"                           + vec4(pass_Color, 1.0));\n"
    "    gl_FragColor = texture2D(in_Texture, vec2(gl_TexCoord[0]));\n"
    "}\n";
#endif

/*
 * Public Video class
 */

void Video::Setup(int width, int height)
{
    /* Initialise OpenGL */
    glViewport(0, 0, width, height);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

#if LOL_EXPERIMENTAL
    stdshader = Shader::Create(vertexshader, fragmentshader);
#endif
}

void Video::SetFov(float theta)
{
#undef near /* Fuck Microsoft */
#undef far /* Fuck Microsoft again */
    mat4 proj;

    float width = GetWidth();
    float height = GetHeight();
    float near = -width - height;
    float far = width + height;

    /* Set the projection matrix */
    if (theta < 1e-4f)
    {
        /* The easy way: purely orthogonal projection. */
        proj_matrix = mat4::ortho(0, width, 0, height, near, far);
    }
    else
    {
        /* Compute a view that approximates the glOrtho view when theta
         * approaches zero. This view ensures that the z=0 plane fills
         * the screen. */
        float t1 = tanf(theta / 2);
        float t2 = t1 * height / width;
        float dist = (float)width / (2.0f * t1);

        near += dist;
        far += dist;

        if (near <= 0.0f)
        {
            far -= (near - 1.0f);
            near = 1.0f;
        }

        proj_matrix = mat4::frustum(-near * t1, near * t1,
                                    -near * t2, near * t2, near, far)
                    * mat4::translate(-0.5f * width, -0.5f * height, -dist);
    }

    view_matrix = mat4(1.0f);

#if LOL_EXPERIMENTAL
    GLuint uni;
    uni = stdshader->GetUniformLocation("proj_matrix");
    glUniformMatrix4fv(uni, 1, GL_FALSE, &proj_matrix[0][0]);
    uni = stdshader->GetUniformLocation("view_matrix");
    glUniformMatrix4fv(uni, 1, GL_FALSE, &view_matrix[0][0]);
#else
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(&proj_matrix[0][0]);

    /* Reset the model view matrix, just in case */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(&view_matrix[0][0]);
#endif
}

void Video::SetDepth(bool set)
{
    if (set)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void Video::Clear()
{
    glViewport(0, 0, GetWidth(), GetHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    SetFov(0.0f);
}

void Video::Destroy()
{
#if LOL_EXPERIMENTAL
    Shader::Destroy(stdshader);
#endif
}

void Video::Capture(uint32_t *buffer)
{
    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    int width = v[2], height = v[3];

    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

#if defined GL_BGRA
    glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
#else
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
#endif

    for (int j = 0; j < height / 2; j++)
        for (int i = 0; i < width; i++)
        {
            uint32_t tmp = buffer[j * width + i];
            buffer[j * width + i] = buffer[(height - j - 1) * width + i];
            buffer[(height - j - 1) * width + i] = tmp;
        }
}

int Video::GetWidth()
{
    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    return v[2];
}

int Video::GetHeight()
{
    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    return v[3];
}

