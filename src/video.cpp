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
#if defined __APPLE__ && defined __MACH__
#   include <OpenGL/gl.h>
#else
#   define GL_GLEXT_PROTOTYPES
#   include <GL/gl.h>
#endif

#include "core.h"

#if LOL_EXPERIMENTAL
Shader *stdshader;

    float4x4 projection_matrix, view_matrix, model_matrix;
#endif

#if LOL_EXPERIMENTAL
static char const *vertexshader =
    "#version 130\n"
    "\n"
    "in vec3 in_Position;\n"
    "in vec3 in_Color;\n"
    "in vec2 in_TexCoord;\n"
    "out vec3 pass_Color;\n"
    "uniform mat4 projection_matrix;\n"
    "uniform mat4 view_matrix;\n"
    "uniform mat4 model_matrix;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = projection_matrix * view_matrix *"
    "                  model_matrix * vec4(in_Position, 1.0f);\n"
    "    gl_TexCoord[0] = vec4(in_TexCoord, 0.0, 0.0);\n"
    "    pass_Color = in_Color;\n"
    "}\n";

static char const *fragmentshader =
    "#version 130\n"
    "\n"
    "uniform sampler2D in_Texture;\n"
    "in vec3 pass_Color;\n"
    "out vec4 out_Color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = 0.5 * (texture2D(in_Texture, vec2(gl_TexCoord[0]))\n"
    "                           + vec4(pass_Color, 1.0));\n"
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
    stdshader = new Shader(vertexshader, fragmentshader);
#endif
}

void Video::SetFov(float theta)
{
#if LOL_EXPERIMENTAL
    float width = GetWidth();
    float height = GetHeight();
    //float near = -width - height;
    //float far = width + height;
    float near = 20.0f;
    float far = 0.1f;
    projection_matrix = float4x4::perspective(theta, width, height, near, far);
#else
#undef near /* Fuck Microsoft */
#undef far /* Fuck Microsoft again */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float width = GetWidth();
    float height = GetHeight();
    float near = -width - height;
    float far = width + height;

    /* Set the projection matrix */
    if (theta < 1e-4f)
    {
        /* The easy way: purely orthogonal projection. */
        glOrtho(0, width, 0, height, near, far);
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

        glFrustum(-near * t1, near * t1, -near * t2, near * t2, near, far);
        glTranslatef(-0.5f * width, -0.5f * height, -dist);
    }

    /* Reset the model view matrix, just in case */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
#if LOL_EXPERIMENTAL
    glViewport(0, 0, GetWidth(), GetHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    view_matrix = float4x4(1.0f);
    view_matrix[3][0] = 0.0f;
    view_matrix[3][1] = 0.0f;
    view_matrix[3][2] = -5.0f;

    model_matrix = float4x4(1.0f);
    model_matrix[0][0] = 0.5f;
    model_matrix[1][1] = 0.5f;
    model_matrix[2][2] = 0.5f;

    GLuint uni;
    uni = stdshader->GetUniformLocation("projection_matrix");
    glUniformMatrix4fv(uni, 1, GL_FALSE, &projection_matrix[0][0]);
    uni = stdshader->GetUniformLocation("view_matrix");
    glUniformMatrix4fv(uni, 1, GL_FALSE, &view_matrix[0][0]);
    uni = stdshader->GetUniformLocation("model_matrix");
    glUniformMatrix4fv(uni, 1, GL_FALSE, &model_matrix[0][0]);
#else
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.01f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

#if LOL_EXPERIMENTAL
    static float time;
    time += 0.01f;
    SetFov(1.0f + sinf(time));
#else
    SetFov(0.5f);
#endif
}

void Video::Destroy()
{
#if LOL_EXPERIMENTAL
    delete stdshader;
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

