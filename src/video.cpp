//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined _XBOX
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#elif defined _WIN32
#   if defined USE_D3D9
#      include <d3d9.h>
#   endif
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

#include "core.h"
#include "lolgl.h"

using namespace std;

/* FIXME: g_d3ddevice should never be exported */
#if defined USE_D3D9
IDirect3DDevice9 *g_d3ddevice;
extern HWND g_hwnd;
#elif defined _XBOX
D3DDevice *g_d3ddevice;
HWND g_hwnd = 0;
#endif

namespace lol
{

class VideoData
{
    friend class Video;

private:
    static DebugRenderMode render_mode;
#if defined USE_D3D9 || defined _XBOX
#   if defined USE_D3D9
    static IDirect3D9 *d3d_ctx;
    static IDirect3DDevice9 *d3d_dev;
#   elif defined _XBOX
    static Direct3D *d3d_ctx;
    static D3DDevice *d3d_dev;
#   endif
#endif
};

DebugRenderMode VideoData::render_mode = DebugRenderMode::Default;

#if defined USE_D3D9 || defined _XBOX
#   if defined USE_D3D9
IDirect3D9 *VideoData::d3d_ctx;
IDirect3DDevice9 *VideoData::d3d_dev;
#   elif defined _XBOX
Direct3D *VideoData::d3d_ctx;
D3DDevice *VideoData::d3d_dev;
#   endif
#endif

/*
 * Public Video class
 */

void Video::Setup(ivec2 size)
{
#if defined USE_D3D9 || defined _XBOX
    VideoData::d3d_ctx = Direct3DCreate9(D3D_SDK_VERSION);
    if (!VideoData::d3d_ctx)
    {
        Log::Error("cannot initialise D3D\n");
        exit(EXIT_FAILURE);
    }

    /* Choose best viewport size */
#   if defined _XBOX
    XVIDEO_MODE VideoMode;
    XGetVideoMode(&VideoMode);
    size = lol::min(size, ivec2(VideoMode.dwDisplayWidth,
                                VideoMode.dwDisplayHeight);
#   endif
    D3DPRESENT_PARAMETERS d3dpp;
    memset(&d3dpp, 0, sizeof(d3dpp));
    d3dpp.BackBufferWidth = size.x;
    d3dpp.BackBufferHeight = size.y;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.hDeviceWindow = g_hwnd;
#   if defined USE_SDL
    d3dpp.Windowed = TRUE;
#   endif
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    HRESULT hr = VideoData::d3d_ctx->CreateDevice(0, D3DDEVTYPE_HAL, g_hwnd,
                                                  D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                                  &d3dpp, &VideoData::d3d_dev);
    if (FAILED(hr))
    {
        Log::Error("cannot create D3D device\n");
        exit(EXIT_FAILURE);
    }

    g_d3ddevice = VideoData::d3d_dev;

    g_renderer = new Renderer(size);
#else
    /* Initialise OpenGL */
    g_renderer = new Renderer(size);
#endif

    /* Initialise reasonable scene default properties */
    SetDebugRenderMode(DebugRenderMode::Default);
}

void Video::SetDebugRenderMode(DebugRenderMode d)
{
    switch(d)
    {
        //All these modes are handled in the shaders.
        case DebugRenderMode::Default:
        case DebugRenderMode::Lighting:
        case DebugRenderMode::Normal:
        case DebugRenderMode::UV:
        {
#if defined USE_D3D9 || defined _XBOX
#elif defined HAVE_GLES_2X
//            glEnable(GL_CULL_FACE);
#else
//            if (VideoData::render_mode == d && glIsEnabled(GL_CULL_FACE) == GL_TRUE)
//                SetFaceCulling(false);
//            else
//                SetFaceCulling(true);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
            break;
        }
        case DebugRenderMode::Wireframe:
        {
            if (VideoData::render_mode == d)
            {
#if defined USE_D3D9 || defined _XBOX
#else
//                SetFaceCulling(!VideoData::face_culling);
#endif
            }
            else
            {
#if defined USE_D3D9 || defined _XBOX
#else
//                SetFaceCulling(false);
#endif
            }
#if defined USE_D3D9 || defined _XBOX
#elif defined HAVE_GLES_2X
#else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
            break;
        }
        default:
            return; /* Unknown render mode */
    }
    VideoData::render_mode = d;
}

DebugRenderMode Video::GetDebugRenderMode()
{
    return VideoData::render_mode;
}

void Video::Clear(ClearMask m)
{
#if defined USE_D3D9 || defined _XBOX
    int mask = 0;
    if (m & ClearMask::Color)
        mask |= D3DCLEAR_TARGET;
    if (m & ClearMask::Depth)
        mask |= D3DCLEAR_ZBUFFER;
    if (m & ClearMask::Stencil)
        mask |= D3DCLEAR_STENCIL;

    vec4 tmp = 255.999f * g_renderer->GetClearColor();
    D3DCOLOR clear_color = D3DCOLOR_XRGB((int)tmp.r, (int)tmp.g, (int)tmp.b);

    if (FAILED(VideoData::d3d_dev->Clear(0, nullptr, mask,
                                         clear_color,
                                         g_renderer->GetClearDepth(), 0)))
        Abort();
#else
    GLbitfield mask = 0;
    if (m & ClearMask::Color)
        mask |= GL_COLOR_BUFFER_BIT;
    if (m & ClearMask::Depth)
        mask |= GL_DEPTH_BUFFER_BIT;
    if (m & ClearMask::Stencil)
        mask |= GL_STENCIL_BUFFER_BIT;
    glClear(mask);
#endif
}

void Video::Destroy()
{
    delete g_renderer;
    g_renderer = nullptr;
}

void Video::Capture(uint32_t *buffer)
{
#if defined USE_D3D9 || defined _XBOX
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
    ibox2 viewport = g_renderer->GetViewport();

    return viewport.B - viewport.A;
}

} /* namespace lol */

