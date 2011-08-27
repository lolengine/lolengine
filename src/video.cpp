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

class VideoData
{
    friend class Video;

private:
    static mat4 proj_matrix, view_matrix;
#if defined ANDROID_NDK || defined __CELLOS_LV2__
    static ivec2 saved_viewport;
#endif
};

mat4 VideoData::proj_matrix;
mat4 VideoData::view_matrix;

#if defined ANDROID_NDK || defined __CELLOS_LV2__
ivec2 VideoData::saved_viewport = 0;
#endif

/*
 * Public Video class
 */

void Video::Setup(ivec2 size)
{
#if defined USE_GLEW
    /* Initialise GLEW if necessary */
    GLenum glerr = glewInit();
    if (glerr != GLEW_OK)
    {
        Log::Error("cannot initialise GLEW: %s\n", glewGetErrorString(glerr));
        exit(EXIT_FAILURE);
    }
#endif

    /* Initialise OpenGL */
    glViewport(0, 0, size.x, size.y);

#if defined ANDROID_NDK || defined __CELLOS_LV2__
    VideoData::saved_viewport = size;
#endif

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    glClearDepth(1.0);

#if defined HAVE_GL_2X
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
}

void Video::SetFov(float theta)
{
#undef near /* Fuck Microsoft */
#undef far /* Fuck Microsoft again */
    vec2 size = GetSize();
    float near = -size.x - size.y;
    float far = size.x + size.y;

#if defined ANDROID_NDK
    size = vec2(640.0f, 480.0f);
#endif

    /* Set the projection matrix */
    if (theta < 1e-4f)
    {
        /* The easy way: purely orthogonal projection. */
        VideoData::proj_matrix = mat4::ortho(0, size.x, 0, size.y, near, far);
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

        mat4 proj = mat4::frustum(-near * t1, near * t1,
                                  -near * t2, near * t2, near, far);
        mat4 trans = mat4::translate(-0.5f * size.x, -0.5f * size.y, -dist);
        VideoData::proj_matrix = proj * trans;
    }

    VideoData::view_matrix = mat4(1.0f);
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
    ivec2 size = GetSize();
    glViewport(0, 0, size.x, size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    SetFov(0.0f);
}

void Video::Destroy()
{
    ;
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

ivec2 Video::GetSize()
{
#if defined ANDROID_NDK
    return VideoData::saved_viewport;
#elif defined __CELLOS_LV2__
    // FIXME: use psglCreateDeviceAuto && psglGetDeviceDimensions
    return VideoData::saved_viewport;
#else
    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    return ivec2(v[2], v[3]);
#endif
}

mat4 const & Video::GetProjMatrix()
{
    return VideoData::proj_matrix;
}

mat4 const & Video::GetViewMatrix()
{
    return VideoData::view_matrix;
}

} /* namespace lol */

