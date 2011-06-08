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

using namespace std;

namespace lol
{

#if defined ANDROID_NDK
vec2i saved_viewport;
#endif

Shader *stdshader;
mat4 proj_matrix, view_matrix, model_matrix;

#define OLD

static char const *vertexshader =
#if !defined OLD
    "#version 130\n"
#endif
    "\n"
#if defined HAVE_GLES_2X
    "attribute vec3 in_Position;\n"
    "attribute vec2 in_TexCoord;\n"
    "varying vec2 pass_TexCoord;\n"
#elif !defined OLD
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
#if defined OLD
    "    vec3 in_Position = gl_Vertex.xyz;\n"
    "    vec2 in_TexCoord = gl_MultiTexCoord0.xy;\n"
#endif
    "    gl_Position = proj_matrix * view_matrix * model_matrix"
    "                * vec4(in_Position, 1.0);\n"
    //"    pass_Color = in_Color;\n"
#if defined HAVE_GLES_2X
    "    pass_TexCoord = in_TexCoord;\n"
#else
    "    gl_TexCoord[0] = vec4(in_TexCoord, 0.0, 0.0);\n"
#endif
    "}\n";

static char const *fragmentshader =
#if !defined OLD
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
    "    vec4 col = texture2D(in_Texture, pass_TexCoord);\n"
    //"    vec4 col = vec4(0.5, 1.0, 0.0, 0.5);\n"
    //"    vec4 col = vec4(pass_TexCoord * 4.0, 0.0, 0.25);\n"
#else
    "    vec4 col = texture2D(in_Texture, vec2(gl_TexCoord[0]));\n"
#endif
#if 0
    "    float mul = 2.0;\n"
#if 0
    "    vec2 d1 = mod(vec2(gl_FragCoord), vec2(2.0, 2.0));\n"
    "    float t1 = mod(3.0 * d1.x + 2.0 * d1.y, 4.0);\n"
    "    float dx2 = mod(floor(gl_FragCoord.x * 0.5), 2.0);\n"
    "    float dy2 = mod(floor(gl_FragCoord.y * 0.5), 2.0);\n"
    "    float t2 = mod(3.0 * dx2 + 2.0 * dy2, 4.0);\n"
    "    float dx3 = mod(floor(gl_FragCoord.x * 0.25), 2.0);\n"
    "    float dy3 = mod(floor(gl_FragCoord.y * 0.25), 2.0);\n"
    "    float t3 = mod(3.0 * dx3 + 2.0 * dy3, 4.0);\n"
    "    float t1 = (1.0 + 16.0 * t1 + 4.0 * t2 + t3) / 65.0;\n"
    "    float t2 = t1;\n"
    "    float t3 = t1;\n"
#else
    "    float rand = sin(gl_FragCoord.x * 1.23456) * 123.456\n"
    "               + cos(gl_FragCoord.y * 2.34567) * 789.012;\n"
    "    float t1 = mod(sin(rand) * 17.13043, 1.0);\n"
    "    float t2 = mod(sin(rand) * 27.13043, 1.0);\n"
    "    float t3 = mod(sin(rand) * 37.13043, 1.0);\n"
#endif
    "    float fracx = fract(col.x * mul);\n"
    "    float fracy = fract(col.y * mul);\n"
    "    float fracz = fract(col.z * mul);\n"
    "    fracx = fracx > t1 ? 1.0 : 0.0;\n"
    "    fracy = fracy > t2 ? 1.0 : 0.0;\n"
    "    fracz = fracz > t3 ? 1.0 : 0.0;\n"
    "    col.x = (floor(col.x * mul) + fracx) / mul;\n"
    "    col.y = (floor(col.y * mul) + fracy) / mul;\n"
    "    col.z = (floor(col.z * mul) + fracz) / mul;\n"
#endif
    "    gl_FragColor = col;\n"
    "}\n";

/*
 * Public Video class
 */

void Video::Setup(vec2i size)
{
    /* Initialise OpenGL */
    glViewport(0, 0, size.x, size.y);

#if defined ANDROID_NDK
    saved_viewport = vec2i(size.x, size.y);
#endif

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    glClearDepthf(1.0);

#if defined HAVE_GL_2X
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif

    stdshader = Shader::Create(vertexshader, fragmentshader);
}

void Video::SetFov(float theta)
{
#undef near /* Fuck Microsoft */
#undef far /* Fuck Microsoft again */
    mat4 proj;

    vec2 size = GetSize();
    float near = -size.x - size.y;
    float far = size.x + size.y;

#if defined ANDROID_NDK
    size = vec(640.0f, 480.0f);
#endif

    /* Set the projection matrix */
    if (theta < 1e-4f)
    {
        /* The easy way: purely orthogonal projection. */
        proj_matrix = mat4::ortho(0, size.x, 0, size.y, near, far);
    }
    else
    {
        /* Compute a view that approximates the glOrtho view when theta
         * approaches zero. This view ensures that the z=0 plane fills
         * the screen. */
        float t1 = tanf(theta / 2);
        float t2 = t1 * size.y / size.y;
        float dist = size.x / (2.0f * t1);

        near += dist;
        far += dist;

        if (near <= 0.0f)
        {
            far -= (near - 1.0f);
            near = 1.0f;
        }

        proj_matrix = mat4::frustum(-near * t1, near * t1,
                                    -near * t2, near * t2, near, far)
                    * mat4::translate(-0.5f * size.x, -0.5f * size.y, -dist);
    }

    view_matrix = mat4(1.0f);

    stdshader->Bind(); /* Required on GLES 2.x? */
#if !defined __CELLOS_LV2__ // Use cgGetNamedParameter etc.
    GLuint uni;
    uni = stdshader->GetUniformLocation("proj_matrix");
    glUniformMatrix4fv(uni, 1, GL_FALSE, &proj_matrix[0][0]);
    uni = stdshader->GetUniformLocation("view_matrix");
    glUniformMatrix4fv(uni, 1, GL_FALSE, &view_matrix[0][0]);
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
    vec2i size = GetSize();
    glViewport(0, 0, size.x, size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    SetFov(0.0f);
}

void Video::Destroy()
{
    Shader::Destroy(stdshader);
}

void Video::Capture(uint32_t *buffer)
{
    GLint v[4];
#if defined __CELLOS_LV2__
    // FIXME: use psglCreateDeviceAuto && psglGetDeviceDimensions
    v[2] = 1920;
    v[3] = 1080;
#else
    glGetIntegerv(GL_VIEWPORT, v);
#endif
    int width = v[2], height = v[3];

#if defined HAVE_GL_2X
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

vec2i Video::GetSize()
{
#if defined ANDROID_NDK
    return saved_viewport;
#elif defined __CELLOS_LV2__
    // FIXME: use psglCreateDeviceAuto && psglGetDeviceDimensions
    return vec2i(1920, 1080);
#else
    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    return vec2i(v[2], v[3]);
#endif
}

} /* namespace lol */

