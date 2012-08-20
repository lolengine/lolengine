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

#include "core.h"
#include "lolgl.h"

#if defined _WIN32 && defined USE_D3D9
#   define FAR
#   define NEAR
#   include <d3d9.h>
#endif

using namespace std;

#if defined USE_D3D9
extern IDirect3DDevice9 *g_d3ddevice;
#elif defined _XBOX
extern D3DDevice *g_d3ddevice;
#endif

namespace lol
{

//
// The FrameBufferData class
// -------------------------
//

class FrameBufferData
{
    friend class FrameBuffer;

    ivec2 m_size;

#if defined USE_D3D9
    IDirect3DTexture9 *m_texture;
    IDirect3DSurface9 *m_surface, *m_back_surface;
#elif defined _XBOX
    D3DTexture *m_texture;
    D3DSurface *m_surface, *m_back_surface;
#else
    GLuint m_fbo, m_texture, m_depth;
#endif
};

//
// The FrameBuffer class
// ----------------------
//

FrameBuffer::FrameBuffer(ivec2 size)
  : m_data(new FrameBufferData)
{
    m_data->m_size = size;
#if defined USE_D3D9
    if (FAILED(g_d3ddevice->CreateTexture(size.x, size.y, 1,
                                          D3DUSAGE_RENDERTARGET,
                                          D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
                                          &m_data->m_texture, NULL)))
        Abort();
    if (FAILED(m_data->m_texture->GetSurfaceLevel(0, &m_data->m_surface)))
        Abort();
#elif defined _XBOX
    if (FAILED(g_d3ddevice->CreateTexture(size.x, size.y, 1, 0,
                                          D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
                                          &m_data->m_texture, NULL)))
        Abort();
    if (FAILED(g_d3ddevice->CreateRenderTarget(size.x, size.y,
                                               D3DFMT_A8R8G8B8,
                                               D3DMULTISAMPLE_NONE, 0, 0,
                                               &m_data->m_surface, NULL)))
        Abort();
#else
#   if GL_VERSION_1_1
    GLenum internal_format = GL_RGBA8;
    GLenum format = GL_BGRA;
    GLenum depth = GL_DEPTH_COMPONENT;
#   else
    GLenum internal_format = GL_RGBA;
    GLenum format = GL_RGBA;
#   endif
    GLenum wrapmode = GL_REPEAT;
    GLenum filtering = GL_NEAREST;

#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    glGenFramebuffers(1, &m_data->m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_data->m_fbo);
#   else
    glGenFramebuffersOES(1, &m_data->m_fbo);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_data->m_fbo);
#   endif

    glGenTextures(1, &m_data->m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_data->m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapmode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapmode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)filtering);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size.x, size.y, 0,
                 format, GL_UNSIGNED_BYTE, NULL);

#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_data->m_texture, 0);
#   else
    glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, m_data->m_texture, 0);
#   endif

    m_data->m_depth = GL_INVALID_ENUM;
#   if GL_VERSION_1_1
    /* FIXME: not implemented on GL ES, see
     * http://stackoverflow.com/q/4041682/111461 */
    if (depth != GL_INVALID_ENUM)
    {
        glGenRenderbuffers(1, &m_data->m_depth);
        glBindRenderbuffer(GL_RENDERBUFFER, m_data->m_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, depth, size.x, size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, m_data->m_depth);
    }
#   endif

#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    glCheckFramebufferStatus(GL_FRAMEBUFFER);
#   endif

    Unbind();
#endif
}

FrameBuffer::~FrameBuffer()
{
#if defined USE_D3D9 || defined _XBOX
    m_data->m_surface->Release();
    m_data->m_texture->Release();
#else
#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    glDeleteFramebuffers(1, &m_data->m_fbo);
#   else
    glDeleteFramebuffersOES(1, &m_data->m_fbo);
#   endif
    glDeleteTextures(1, &m_data->m_texture);
#   if GL_VERSION_1_1
    if (m_data->m_depth != GL_INVALID_ENUM)
        glDeleteRenderbuffers(1, &m_data->m_depth);
#   endif
#endif
    delete m_data;
}

ShaderTexture FrameBuffer::GetTexture() const
{
    ShaderTexture ret;
#if defined USE_D3D9 || defined _XBOX
    ret.m_flags = (uint64_t)(uintptr_t)m_data->m_texture;
#else
    ret.m_flags = m_data->m_texture;
#endif
    return ret;
}

void FrameBuffer::Bind()
{
#if defined USE_D3D9 || defined _XBOX
    if (FAILED(g_d3ddevice->GetRenderTarget(0, &m_data->m_back_surface)))
        Abort();
    if (FAILED(g_d3ddevice->SetRenderTarget(0, m_data->m_surface)))
        Abort();
#else
#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    glBindFramebuffer(GL_FRAMEBUFFER, m_data->m_fbo);
#   else
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_data->m_fbo);
#   endif
#endif
}

void FrameBuffer::Unbind()
{
#if defined USE_D3D9
    if (FAILED(g_d3ddevice->SetRenderTarget(0, m_data->m_back_surface)))
        Abort();
    m_data->m_back_surface->Release();
#elif defined _XBOX
    if (FAILED(g_d3ddevice->Resolve(D3DRESOLVE_RENDERTARGET0, NULL,
                                    m_data->m_texture, NULL, 0, 0, NULL,
                                    0, 0, NULL)))
        Abort();
    if (FAILED(g_d3ddevice->SetRenderTarget(0, m_data->m_back_surface)))
        Abort();
    m_data->m_back_surface->Release();
#else
#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
#   else
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, NULL);
#   endif
#endif
}

} /* namespace lol */

