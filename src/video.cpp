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

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#include "core.h"
#include "lolgl.h"

namespace lol
{

#if defined ANDROID_NDK
vec2i saved_viewport;
#endif

#if defined HAVE_GL_2X || defined HAVE_GLES_2X
Shader *stdshader;
#endif
mat4 proj_matrix, view_matrix, model_matrix;

#if defined HAVE_GL_2X || defined HAVE_GLES_2X
static char const *vertexshader =
#if !defined HAVE_GLES_2X
    "#version 130\n"
#endif
    "\n"
#if defined HAVE_GLES_2X
    "attribute vec3 in_Position;\n"
    "attribute vec2 in_TexCoord;\n"
    "varying vec2 pass_TexCoord;\n"
#else
    "in vec3 in_Position;\n"
    "in vec2 in_TexCoord;\n"
#endif
    //"in vec3 in_Color;\n"
    //"out vec3 pass_Color;\n"
    "uniform mat4 proj_matrix;\n"
    "uniform mat4 view_matrix;\n"
    "uniform mat4 model_matrix;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = proj_matrix * view_matrix * model_matrix"
    "                * vec4(in_Position, 1.0f);\n"
    //"    pass_Color = in_Color;\n"
#if defined HAVE_GLES_2X
    "    pass_TexCoord = in_TexCoord;\n"
#else
    "    gl_TexCoord[0] = vec4(in_TexCoord, 0.0, 0.0);\n"
#endif
    "}\n";

static char const *fragmentshader =
#if !defined HAVE_GLES_2X
    "#version 130\n"
#endif
    "\n"
    "uniform sampler2D in_Texture;\n"
    //"in vec3 pass_Color;\n"
    //"out vec4 out_Color;\n"
#if defined HAVE_GLES_2X
    "varying vec2 pass_TexCoord;\n"
#endif
    "\n"
    "void main()\n"
    "{\n"
#if defined HAVE_GLES_2X
    "    gl_FragColor = texture2D(in_Texture, pass_TexCoord);\n"
    //"    gl_FragColor = vec4(0.5, 1.0, 0.0, 0.5);\n"
    //"    gl_FragColor = vec4(pass_TexCoord * 4.0, 0.0, 0.25);\n"
#else
    "    vec4 col = texture2D(in_Texture, vec2(gl_TexCoord[0]));\n"
#if 0
    "    float dx1 = mod(gl_FragCoord.x, 2.0);\n"
    "    float dy1 = mod(gl_FragCoord.y, 2.0);\n"
    "    float t1 = mod(3.0 * dx1 + 2.0 * dy1, 4.0);\n"
    "    float dx2 = mod(floor(gl_FragCoord.x * 0.5), 2.0);\n"
    "    float dy2 = mod(floor(gl_FragCoord.y * 0.5), 2.0);\n"
    "    float t2 = mod(3.0 * dx2 + 2.0 * dy2, 4.0);\n"
    "    float t = (1.0 + 4.0 * t1 + t2) / 17.0;\n"
    "    col.x = col.x > t ? 1.0 : 0.0;\n"
    "    col.y = col.y > t ? 1.0 : 0.0;\n"
    "    col.z = col.z > t ? 1.0 : 0.0;\n"
#endif
    "    gl_FragColor = col;\n"
#endif
    "}\n";
#endif

/*
 * Public Video class
 */

void Video::Setup(int width, int height)
{
    /* Initialise OpenGL */
    glViewport(0, 0, width, height);

#if defined ANDROID_NDK
    saved_viewport = vec2i(width, height);
#endif

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    glClearDepthf(1.0);

#if defined HAVE_GL_2X || defined HAVE_GLES_1X
    glShadeModel(GL_SMOOTH);
#endif
#if defined HAVE_GL_2X || defined HAVE_GLES_1X
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif

#if defined HAVE_GL_2X || defined HAVE_GLES_2X
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

#if defined ANDROID_NDK
    width = 640.0f;
    height = 480.0f;
#endif

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

#if defined HAVE_GL_2X || defined HAVE_GLES_2X
    stdshader->Bind(); /* Required on GLES 2.x? */
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
#if defined HAVE_GL_2X || defined HAVE_GLES_2X
    Shader::Destroy(stdshader);
#endif
}

void Video::Capture(uint32_t *buffer)
{
    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    int width = v[2], height = v[3];

#if defined HAVE_GL_1X || defined HAVE_GL_2X
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
#endif
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
#if defined ANDROID_NDK
    return saved_viewport.x;
#else
    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    return v[2];
#endif
}

int Video::GetHeight()
{
#if defined ANDROID_NDK
    return saved_viewport.y;
#else
    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    return v[3];
#endif
}

} /* namespace lol */

