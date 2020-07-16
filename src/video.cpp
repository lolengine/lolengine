//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if defined _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
#   undef near
#   undef far
#endif

#include "lolgl.h"

namespace lol
{

class VideoData
{
    friend class video;

private:
    static DebugRenderMode render_mode;
};

DebugRenderMode VideoData::render_mode = DebugRenderMode::Default;

/*
 * Public Video class
 */

void video::init(ivec2 size)
{
    Scene::AddNew(size);

    /* Initialise reasonable scene default properties */
    SetDebugRenderMode(DebugRenderMode::Default);
}

void video::SetDebugRenderMode(DebugRenderMode d)
{
    switch(d)
    {
        //All these modes are handled in the shaders.
        case DebugRenderMode::Default:
        case DebugRenderMode::Flat:
        case DebugRenderMode::Lighting:
        case DebugRenderMode::Normal:
        case DebugRenderMode::UV:
        {
#if defined HAVE_GLES_2X
#elif defined LOL_USE_GLEW || defined HAVE_GL_2X
            glEnable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
            break;
        }
        case DebugRenderMode::Wireframe:
        {
#if defined HAVE_GLES_2X
#elif defined LOL_USE_GLEW || defined HAVE_GL_2X
            glDisable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
            break;
        }
        default:
            return; /* Unknown render mode */
    }
    VideoData::render_mode = d;
}

DebugRenderMode video::GetDebugRenderMode()
{
    return VideoData::render_mode;
}

void video::release()
{
    Scene::DestroyAll();
}

void video::capture(uint32_t *buffer)
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLEX_2X
    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
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
#else
    (void)buffer;
#endif
}

void video::resize(ivec2 size)
{
    Scene::GetScene(0).resize(size);
}

ivec2 video::size()
{
    return Scene::GetScene(0).get_renderer()->viewport().extent();
}

} /* namespace lol */

