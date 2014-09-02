//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "lolgl.h"

#if defined _WIN32 && defined USE_D3D9
#   define FAR
#   define NEAR
#   include <d3d9.h>
#elif defined _XBOX
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

namespace lol
{

//
// The FramebufferData class
// -------------------------
//

class FramebufferData
{
    friend class Framebuffer;

    ibox2 m_saved_viewport;
    ivec2 m_size;
    bool m_bound;

#if defined USE_D3D9
    IDirect3DDevice9 *m_dev;
    IDirect3DTexture9 *m_texture;
    IDirect3DSurface9 *m_surface, *m_back_surface;
#elif defined _XBOX
    D3DDevice *m_dev;
    D3DTexture *m_texture;
    D3DSurface *m_surface, *m_back_surface;
#else
    GLuint m_fbo, m_texture, m_depth;
#endif
};

//
// The FramebufferFormat struct
// ----------------------
//

uint32_t FramebufferFormat::GetFormat()
{
    switch (m_format)
    {
#if defined USE_D3D9
    case R_16_F:        return D3DFMT_R16F;
    case R_32_F:        return D3DFMT_R32F;
    case RG_16:
    case RG_16_I:
    case RG_16_UI:      return D3DFMT_G16R16;
    case RG_16_F:       return D3DFMT_G16R16F;
    case RG_32_F:       return D3DFMT_G32R32F;
    case RGB_8:
    case RGB_8_I:
    case RGB_8_UI:      return D3DFMT_R8G8B8;
    case RGBA_8:
    case RGBA_8_I:
    case RGBA_8_UI:     return D3DFMT_A8R8G8B8;
    case RGBA_16:
    case RGBA_16_I:
    case RGBA_16_UI:    return D3DFMT_A16B16G16R16;
    case RGBA_16_F:     return D3DFMT_A16B16G16R16F;
    case RGBA_32_F:     return D3DFMT_A32B32G32R32F;
#elif defined _XBOX
    case R_16_F:        return D3DFMT_R16F;
    case R_32_F:        return D3DFMT_R32F;
    case RG_16:
    case RG_16_I:
    case RG_16_UI:      return D3DFMT_G16R16;
    case RG_16_F:       return D3DFMT_G16R16F;
    case RG_32_F:       return D3DFMT_G32R32F;
    case RGB_8:
    case RGB_8_I:
    case RGB_8_UI:      return D3DFMT_X8R8G8B8;
    case RGBA_8:
    case RGBA_8_I:
    case RGBA_8_UI:     return D3DFMT_A8R8G8B8;
    case RGBA_16:
    case RGBA_16_I:
    case RGBA_16_UI:    return D3DFMT_A16B16G16R16;
    case RGBA_16_F:     return D3DFMT_A16B16G16R16F;
    case RGBA_32_F:     return D3DFMT_A32B32G32R32F;
#elif defined __CELLOS_LV2__
    /* Supported drawable formats on the PS3: GL_ARGB_SCE, GL_RGB16F_ARB,
     * GL_RGBA16F_ARB, GL_RGB32F_ARB, GL_RGBA32F_ARB, GL_LUMINANCE32F_ARB. */
    case RGB_16_F:      return GL_RGB16F_ARB;
    case RGB_32_F:      return GL_RGB32F_ARB;
    case RGBA_8:        return GL_ARGB_SCE;
    case RGBA_16_F:     return GL_RGBA16F_ARB;
    case RGBA_32_F:     return GL_RGBA32F_ARB;
#elif defined HAVE_GLES_2X
    /* FIXME: incomplete */
    case RGBA_8:
    case RGBA_8_I:
    case RGBA_8_UI:     return GL_RGBA;
#elif defined __APPLE__ && defined __MACH__
    case R_8:
    case R_8_I:
    case R_8_UI:
    case R_8_F:

    case R_16:
    case R_16_I:
    case R_16_UI:
    case R_16_F:

    case R_32_I:
    case R_32:
    case R_32_UI:
    case R_32_F:        return GL_RED;

    case RG_8:
    case RG_8_I:
    case RG_8_UI:
    case RG_8_F:

    case RG_16:
    case RG_16_I:
    case RG_16_UI:
    case RG_16_F:

    case RG_32:
    case RG_32_I:
    case RG_32_UI:
    case RG_32_F:       return GL_RG;

    case RGB_8:
    case RGB_8_I:
    case RGB_8_UI:
    case RGB_8_F:

    case RGB_16:
    case RGB_16_I:
    case RGB_16_UI:
    case RGB_16_F:

    case RGB_32:
    case RGB_32_I:
    case RGB_32_UI:
    case RGB_32_F:      return (m_invert_rgb)?(GL_BGR):(GL_RGB);

    case RGBA_8:
    case RGBA_8_I:
    case RGBA_8_UI:
    case RGBA_8_F:

    case RGBA_16:
    case RGBA_16_I:
    case RGBA_16_UI:
    case RGBA_16_F:

    case RGBA_32:
    case RGBA_32_I:
    case RGBA_32_UI:
#   if defined GL_BGRA
    case RGBA_32_F:     return (m_invert_rgb)?(GL_BGRA):(GL_RGBA);
#   else
    case RGBA_32_F:     return GL_RGBA;
#   endif
#else
    case R_8:           return GL_R8;
    case R_8_I:         return GL_R8I;
    case R_8_UI:        return GL_R8UI;

    case R_16:          return GL_R16;
    case R_16_I:        return GL_R16I;
    case R_16_UI:       return GL_R16UI;
    case R_16_F:        return GL_R16F;

    case R_32_I:        return GL_R32I;
    case R_32_UI:       return GL_R32UI;
    case R_32_F:        return GL_R32F;

    case RG_8:          return GL_RG8;
    case RG_8_I:        return GL_RG8I;
    case RG_8_UI:       return GL_RG8UI;

    case RG_16:         return GL_RG16;
    case RG_16_I:       return GL_RG16I;
    case RG_16_UI:      return GL_RG16UI;
    case RG_16_F:       return GL_RG16F;

    case RG_32_I:       return GL_RG32I;
    case RG_32_UI:      return GL_RG32UI;
    case RG_32_F:       return GL_RG32F;

    case RGB_8:         return GL_RGB8;
    case RGB_8_I:       return GL_RGB8I;
    case RGB_8_UI:      return GL_RGB8UI;

    case RGB_16:        return GL_RGB16;
    case RGB_16_I:      return GL_RGB16I;
    case RGB_16_UI:     return GL_RGB16UI;
    case RGB_16_F:      return GL_RGB16F;

    case RGB_32_I:      return GL_RGB32I;
    case RGB_32_UI:     return GL_RGB32UI;
    case RGB_32_F:      return GL_RGB32F;

    case RGBA_8:        return GL_RGBA8;
    case RGBA_8_I:      return GL_RGBA8I;
    case RGBA_8_UI:     return GL_RGBA8UI;

    case RGBA_16:       return GL_RGBA16;
    case RGBA_16_I:     return GL_RGBA16I;
    case RGBA_16_UI:    return GL_RGBA16UI;
    case RGBA_16_F:     return GL_RGBA16F;

    case RGBA_32_I:     return GL_RGBA32I;
    case RGBA_32_UI:    return GL_RGBA32UI;
    case RGBA_32_F:     return GL_RGBA32F;
#endif
    default:
        ASSERT(false, "unknown framebuffer format %d", m_format);
        return 0;
    }
}

uint32_t FramebufferFormat::GetFormatOrder()
{
    switch (m_format)
    {
#if defined USE_D3D9 || defined _XBOX
    /* FIXME: not implemented at all */
#elif defined __CELLOS_LV2__
    /* FIXME: not implemented at all */
#elif defined HAVE_GLES_2X
    /* FIXME: incomplete */
    case R_8:   case RG_8:   case RGB_8:   case RGBA_8:
    case R_8_I: case RG_8_I: case RGB_8_I: case RGBA_8_I:
        return GL_BYTE;
    case R_8_UI: case RG_8_UI: case RGB_8_UI: case RGBA_8_UI:
        return GL_UNSIGNED_BYTE;
#elif defined __APPLE__ && defined __MACH__
    case R_8:   case RG_8:   case RGB_8:   case RGBA_8:
    case R_8_I: case RG_8_I: case RGB_8_I: case RGBA_8_I:
        return GL_BYTE;
    case R_8_UI: case RG_8_UI: case RGB_8_UI: case RGBA_8_UI:
        return GL_UNSIGNED_BYTE;

    case R_16:   case RG_16:   case RGB_16:   case RGBA_16:
    case R_16_I: case RG_16_I: case RGB_16_I: case RGBA_16_I:
        return GL_SHORT;
    case R_16_UI: case RG_16_UI: case RGB_16_UI: case RGBA_16_UI:
        return GL_UNSIGNED_SHORT;

    case R_16_F: case RG_16_F: case RGB_16_F: case RGBA_16_F:
        ASSERT(false, "unsupported framebuffer format order %d", m_format);
        return 0;

    case R_32_I: case RG_32_I: case RGB_32_I: case RGBA_32_I:
        return GL_INT;
    case R_32_UI: case RG_32_UI: case RGB_32_UI: case RGBA_32_UI:
        return GL_UNSIGNED_INT;
    case R_32_F: case RG_32_F: case RGB_32_F: case RGBA_32_F:
        return GL_FLOAT;
#else
    case R_8:  case R_8_I:  case R_8_UI:  case R_8_F:
    case R_16: case R_16_I: case R_16_UI: case R_16_F:
    case R_32: case R_32_I: case R_32_UI: case R_32_F:
        return GL_RED;

    case RG_8:  case RG_8_I:  case RG_8_UI:  case RG_8_F:
    case RG_16: case RG_16_I: case RG_16_UI: case RG_16_F:
    case RG_32: case RG_32_I: case RG_32_UI: case RG_32_F:
        return GL_RG;

    case RGB_8:  case RGB_8_I:  case RGB_8_UI:  case RGB_8_F:
    case RGB_16: case RGB_16_I: case RGB_16_UI: case RGB_16_F:
    case RGB_32: case RGB_32_I: case RGB_32_UI: case RGB_32_F:
        return m_invert_rgb ? GL_BGR : GL_RGB;

    case RGBA_8:  case RGBA_8_I:  case RGBA_8_UI:  case RGBA_8_F:
    case RGBA_16: case RGBA_16_I: case RGBA_16_UI: case RGBA_16_F:
    case RGBA_32: case RGBA_32_I: case RGBA_32_UI: case RGBA_32_F:
#   if defined GL_BGRA
        return (m_invert_rgb)?(GL_BGRA):(GL_RGBA);
#   else
        return GL_RGBA;
#   endif
#endif
    default:
        ASSERT(false, "unknown framebuffer format order %d", m_format);
        return 0;
    }
}

//
// The Framebuffer class
// ----------------------
//

Framebuffer::Framebuffer(ivec2 size, FramebufferFormat fbo_format)
  : m_data(new FramebufferData)
{
    m_data->m_size = size;
    m_data->m_bound = false;
#if defined USE_D3D9
    m_data->m_dev = (IDirect3DDevice9 *)g_renderer->GetDevice();

    if (FAILED(m_data->m_dev->CreateTexture(size.x, size.y, 1,
                                            D3DUSAGE_RENDERTARGET,
                                            (D3DFORMAT)fbo_format.GetFormat(),
                                            D3DPOOL_DEFAULT,
                                            &m_data->m_texture, nullptr)))
        Abort();
    if (FAILED(m_data->m_texture->GetSurfaceLevel(0, &m_data->m_surface)))
        Abort();
#elif defined _XBOX
    m_data->m_dev = (D3DDevice *)g_renderer->GetDevice();

    if (FAILED(m_data->m_dev->CreateTexture(size.x, size.y, 1, 0,
                                            (D3DFORMAT)fbo_format.GetFormat(),
                                            D3DPOOL_DEFAULT,
                                            &m_data->m_texture, nullptr)))
        Abort();
    if (FAILED(m_data->m_dev->CreateRenderTarget(size.x, size.y,
                                                 (D3DFORMAT)fbo_format.GetFormat(),
                                                 D3DMULTISAMPLE_NONE, 0, 0,
                                                 &m_data->m_surface, nullptr)))
        Abort();
#else
#   if GL_VERSION_1_1
    GLenum internal_format = fbo_format.GetFormat();
    GLenum format = fbo_format.GetFormatOrder();
    GLenum depth = GL_DEPTH_COMPONENT;
#   elif defined __CELLOS_LV2__
    /* Supported drawable formats on the PS3: GL_ARGB_SCE, GL_RGB16F_ARB,
     * GL_RGBA16F_ARB, GL_RGB32F_ARB, GL_RGBA32F_ARB, GL_LUMINANCE32F_ARB. */
    GLenum internal_format = fbo_format.GetFormat();
    GLenum format = fbo_format.GetFormatOrder();
#   elif GL_ES_VERSION_2_0
    /* In OpenGL ES, internal format and format must match. */
    GLenum internal_format = fbo_format.GetFormat();
    GLenum format = fbo_format.GetFormat();
    GLenum depth = GL_DEPTH_COMPONENT16; /* for WebGL */
#   else
    /* In OpenGL ES, internal format and format must match. */
    GLenum internal_format = fbo_format.GetFormat();
    GLenum format = fbo_format.GetFormat();
#   endif
    GLenum wrapmode = GL_CLAMP_TO_EDGE;
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size.x, size.y, 0,
                 format, GL_UNSIGNED_BYTE, nullptr);

#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_data->m_texture, 0);
#   else
    glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, m_data->m_texture, 0);
#   endif

    m_data->m_depth = GL_INVALID_ENUM;
#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    if (depth != GL_INVALID_ENUM)
    {
        /* XXX: might not work on GL ES, see
         * http://stackoverflow.com/q/4041682/111461
         * See also http://qt-project.org/forums/viewthread/11734 */
        glGenRenderbuffers(1, &m_data->m_depth);
        glBindRenderbuffer(GL_RENDERBUFFER, m_data->m_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, depth, size.x, size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, m_data->m_depth);
    }
#   endif

    glBindTexture(GL_TEXTURE_2D, 0);

#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    ASSERT(status == GL_FRAMEBUFFER_COMPLETE,
           "invalid framebuffer status 0x%x", status);
#   endif

#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#   else
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
#   endif
#endif
}

Framebuffer::~Framebuffer()
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
#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    if (m_data->m_depth != GL_INVALID_ENUM)
        glDeleteRenderbuffers(1, &m_data->m_depth);
#   endif
#endif
    delete m_data;
}

TextureUniform Framebuffer::GetTextureUniform() const
{
    TextureUniform ret;
#if defined USE_D3D9 || defined _XBOX
    ret.m_flags = (uint64_t)(uintptr_t)m_data->m_texture;
#else
    ret.m_flags = m_data->m_texture;
#endif
    return ret;
}

ivec2 Framebuffer::GetSize() const
{
    return m_data->m_size;
}

Image Framebuffer::GetImage() const
{
    Image ret(m_data->m_size);

#if defined USE_D3D9 || defined _XBOX
    /* TODO: implement D3D Framebuffer::GetImage() */
#else
    u8vec4 *buffer = ret.Lock<PixelFormat::RGBA_8>();
    glReadPixels(0, 0, m_data->m_size.x, m_data->m_size.y,
                 GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    ret.Unlock(buffer);
#endif

    return ret;
}

void Framebuffer::Bind()
{
    ASSERT(!m_data->m_bound, "trying to bind an already bound framebuffer");

#if defined USE_D3D9 || defined _XBOX
    if (FAILED(m_data->m_dev->GetRenderTarget(0, &m_data->m_back_surface)))
        Abort();
    if (FAILED(m_data->m_dev->SetRenderTarget(0, m_data->m_surface)))
        Abort();
#else
#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    glBindFramebuffer(GL_FRAMEBUFFER, m_data->m_fbo);
#   else
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_data->m_fbo);
#   endif
#endif

    /* FIXME: this should be done in the RenderContext object
     * instead, maybe by getting rid of Framebuffer::Bind() and
     * creating RenderContext::SetFramebuffer() instead. */
    m_data->m_saved_viewport = g_renderer->GetViewport();
    g_renderer->SetViewport(ibox2(ivec2::zero, m_data->m_size));
    m_data->m_bound = true;
}

void Framebuffer::Unbind()
{
    ASSERT(m_data->m_bound, "trying to unbind an unbound framebuffer");

#if defined USE_D3D9 || defined _XBOX
#   if defined _XBOX
    if (FAILED(m_data->m_dev->Resolve(D3DRESOLVE_RENDERTARGET0, nullptr,
                                    m_data->m_texture, nullptr, 0, 0, nullptr,
                                    0, 0, nullptr)))
        Abort();
#   endif
    if (FAILED(m_data->m_dev->SetRenderTarget(0, m_data->m_back_surface)))
        Abort();
    m_data->m_back_surface->Release();
#else
#   if GL_VERSION_1_1 || GL_ES_VERSION_2_0
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#   else
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
#   endif
#endif

    g_renderer->SetViewport(m_data->m_saved_viewport);
    m_data->m_bound = false;
}

} /* namespace lol */

