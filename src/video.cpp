//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cmath>

#if _XBOX
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#elif defined _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

#include "core.h"
#include "lolgl.h"

using namespace std;

#if defined _XBOX
/* FIXME: g_d3ddevice should never be exported */
D3DDevice *g_d3ddevice;
#endif

namespace lol
{

class VideoData
{
    friend class Video;

private:
    static mat4 proj_matrix, view_matrix;
    static ivec2 saved_viewport;
#if defined _XBOX
    static Direct3D *d3d_ctx;
    static D3DDevice *d3d_dev;
#endif
};

mat4 VideoData::proj_matrix;
mat4 VideoData::view_matrix;
ivec2 VideoData::saved_viewport(0, 0);

#if defined _XBOX
Direct3D *VideoData::d3d_ctx;
D3DDevice *VideoData::d3d_dev;
#endif

/*
 * Public Video class
 */

void Video::Setup(ivec2 size)
{
#if defined _XBOX
    VideoData::d3d_ctx = Direct3DCreate9(D3D_SDK_VERSION);
    if (!VideoData::d3d_ctx)
    {
        Log::Error("cannot initialise D3D\n");
        exit(EXIT_FAILURE);
    }

    D3DPRESENT_PARAMETERS d3dpp;
    memset(&d3dpp, 0, sizeof(d3dpp));

    XVIDEO_MODE VideoMode;
    XGetVideoMode( &VideoMode );
    if (size.x > VideoMode.dwDisplayWidth)
        size.x = VideoMode.dwDisplayWidth;
    if (size.y > VideoMode.dwDisplayHeight)
        size.y = VideoMode.dwDisplayHeight;
    VideoData::saved_viewport = size;

    d3dpp.BackBufferWidth = size.x;
    d3dpp.BackBufferHeight = size.y;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (FAILED(VideoData::d3d_ctx->CreateDevice(0, D3DDEVTYPE_HAL, NULL,
                                                D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                                &d3dpp, &VideoData::d3d_dev)))
    {
        Log::Error("cannot create D3D device\n");
        exit(EXIT_FAILURE);
    }

    g_d3ddevice = VideoData::d3d_dev;
#else
#   if defined USE_GLEW && !defined __APPLE__
    /* Initialise GLEW if necessary */
    GLenum glerr = glewInit();
    if (glerr != GLEW_OK)
    {
        Log::Error("cannot initialise GLEW: %s\n", glewGetErrorString(glerr));
        exit(EXIT_FAILURE);
    }
#   endif

    /* Initialise OpenGL */
    glViewport(0, 0, size.x, size.y);
    VideoData::saved_viewport = size;

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClearDepth(1.0);

#   if defined HAVE_GL_2X && !defined __APPLE__
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#   endif
#endif
}

void Video::SetFov(float theta)
{
    vec2 size = GetSize();
    float near = -size.x - size.y;
    float far = size.x + size.y;

#if defined __ANDROID__
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
#if defined _XBOX
#   define STR0(x) #x
#   define STR(x) STR0(x)
#   pragma message(__FILE__ "(" STR(__LINE__) "): warning: Video::SetDepth() not implemented")
#else
    if (set)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
#endif
}

void Video::Clear()
{
    ivec2 size = GetSize();
#if defined _XBOX
    VideoData::d3d_dev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
                                        | D3DCLEAR_STENCIL,
                              D3DCOLOR_XRGB(26, 51, 77), 1.0f, 0);
#else
    glViewport(0, 0, size.x, size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#endif

    SetFov(0.0f);
}

void Video::Destroy()
{
    ;
}

void Video::Capture(uint32_t *buffer)
{
#if _XBOX
    /* TODO */
#else
    GLint v[4];
#   if defined __CELLOS_LV2__
    // FIXME: use psglCreateDeviceAuto && psglGetDeviceDimensions
    v[2] = 1920;
    v[3] = 1080;
#   else
    glGetIntegerv(GL_VIEWPORT, v);
#   endif
    int width = v[2], height = v[3];

#   if defined HAVE_GL_2X
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
#   endif
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

#   if defined GL_BGRA
    glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
#   else
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
#   endif

    for (int j = 0; j < height / 2; j++)
        for (int i = 0; i < width; i++)
        {
            uint32_t tmp = buffer[j * width + i];
            buffer[j * width + i] = buffer[(height - j - 1) * width + i];
            buffer[(height - j - 1) * width + i] = tmp;
        }
#endif
}

ivec2 Video::GetSize()
{
#if _XBOX
    return VideoData::saved_viewport;
#elif 1
    /* GetSize() is called too often on the game thread; we cannot rely on
     * the GL context at this point */
    return VideoData::saved_viewport;
#elif defined __CELLOS_LV2__
    // FIXME: use psglCreateDeviceAuto && psglGetDeviceDimensions
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

