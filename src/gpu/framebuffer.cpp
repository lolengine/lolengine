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

#include <lol/msg>
#include <cassert>

// FIXME: fine-tune this define
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
#include "lolgl.h"
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

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    GLuint m_fbo, m_texture, m_depth;
#endif
};

//
// The FramebufferFormat struct
// ----------------------
//

uint32_t FramebufferFormat::GetFormat()
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    switch (m_format)
    {
#if defined HAVE_GLES_2X
    /* FIXME: incomplete */
    case RGBA_8:
    case RGBA_8_I:
    case RGBA_8_UI:     return GL_RGBA;
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
#ifdef GL_RGB8I
    case RGB_8_I:       return GL_RGB8I;
    case RGB_8_UI:      return GL_RGB8UI;
#endif

    case RGB_16:        return GL_RGB16;
#ifdef GL_RGB16I
    case RGB_16_I:      return GL_RGB16I;
    case RGB_16_UI:     return GL_RGB16UI;
    case RGB_16_F:      return GL_RGB16F;
#endif

#ifdef GL_RGB32I
    case RGB_32_I:      return GL_RGB32I;
    case RGB_32_UI:     return GL_RGB32UI;
    case RGB_32_F:      return GL_RGB32F;
#endif

    case RGBA_8:        return GL_RGBA8;
#ifdef GL_RGBA8I
    case RGBA_8_I:      return GL_RGBA8I;
    case RGBA_8_UI:     return GL_RGBA8UI;
#endif

    case RGBA_16:       return GL_RGBA16;
#ifdef GL_RGBA16I
    case RGBA_16_I:     return GL_RGBA16I;
    case RGBA_16_UI:    return GL_RGBA16UI;
    case RGBA_16_F:     return GL_RGBA16F;
#endif

#ifdef GL_RGBA32I
    case RGBA_32_I:     return GL_RGBA32I;
    case RGBA_32_UI:    return GL_RGBA32UI;
    case RGBA_32_F:     return GL_RGBA32F;
#endif
#endif
    default:
        assert(false);
        return 0;
    }
#else
    return 0;
#endif
}

uint32_t FramebufferFormat::GetFormatOrder()
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    switch (m_format)
    {
#if defined HAVE_GLES_2X
    /* FIXME: incomplete */
    case R_8:   case RG_8:   case RGB_8:   case RGBA_8:
    case R_8_I: case RG_8_I: case RGB_8_I: case RGBA_8_I:
        return GL_BYTE;
    case R_8_UI: case RG_8_UI: case RGB_8_UI: case RGBA_8_UI:
        return GL_UNSIGNED_BYTE;
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
#   if defined GL_BGRA && !defined __APPLE__ && !defined __NX__
        return (m_invert_rgb)?(GL_BGRA):(GL_RGBA);
#   else
        return GL_RGBA;
#   endif
#endif
    default:
        assert(false);
        return 0;
    }
#else
    return 0;
#endif
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
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
#if defined GL_VERSION_4_2
    GLenum internal_format = fbo_format.GetFormat();
    GLenum depth = GL_DEPTH_COMPONENT16; /* for WebGL */
#elif defined GL_ES_VERSION_2_0
    /* In OpenGL ES, internal format and format must match. */
    GLenum internal_format = fbo_format.GetFormat();
    GLenum format = fbo_format.GetFormat();
    GLenum depth = GL_DEPTH_COMPONENT16; /* for WebGL */
#elif defined GL_VERSION_1_1
    GLenum internal_format = fbo_format.GetFormat();
    GLenum format = fbo_format.GetFormatOrder();
    GLenum depth = GL_DEPTH_COMPONENT;
#else
    /* In OpenGL ES, internal format and format must match. */
    GLenum internal_format = fbo_format.GetFormat();
    GLenum format = fbo_format.GetFormat();
#endif
    GLenum wrapmode = GL_CLAMP_TO_EDGE;
    GLenum filtering = GL_LINEAR;

#if defined GL_VERSION_1_1 || defined GL_ES_VERSION_2_0
    glGenFramebuffers(1, &m_data->m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_data->m_fbo);
#else
    glGenFramebuffersOES(1, &m_data->m_fbo);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_data->m_fbo);
#endif

    glGenTextures(1, &m_data->m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_data->m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
#if defined GL_VERSION_4_2
    glTexStorage2D(GL_TEXTURE_2D, 1, internal_format, size.x, size.y);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size.x, size.y, 0,
                 format, GL_UNSIGNED_BYTE, nullptr);
#endif

#if defined GL_VERSION_1_1 || defined GL_ES_VERSION_2_0
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_data->m_texture, 0);
#else
    glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, m_data->m_texture, 0);
#endif

    m_data->m_depth = GL_INVALID_ENUM;
#if defined GL_VERSION_1_1 || defined GL_ES_VERSION_2_0
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
#endif

    glBindTexture(GL_TEXTURE_2D, 0);

#if defined GL_VERSION_1_1 || defined GL_ES_VERSION_2_0
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        msg::error("invalid framebuffer status 0x%x", status);
#endif

#if defined GL_VERSION_1_1 || defined GL_ES_VERSION_2_0
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#else
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
#endif
#endif // defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
}

Framebuffer::~Framebuffer()
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
#if defined GL_VERSION_1_1 || defined GL_ES_VERSION_2_0
    glDeleteFramebuffers(1, &m_data->m_fbo);
#else
    glDeleteFramebuffersOES(1, &m_data->m_fbo);
#endif
    glDeleteTextures(1, &m_data->m_texture);
#if defined GL_VERSION_1_1 || defined GL_ES_VERSION_2_0
    if (m_data->m_depth != GL_INVALID_ENUM)
        glDeleteRenderbuffers(1, &m_data->m_depth);
#endif
#endif // defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    delete m_data;
}

TextureUniform Framebuffer::GetTextureUniform() const
{
    TextureUniform ret;
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    ret.m_flags = m_data->m_texture;
#endif
    return ret;
}

ivec2 Framebuffer::GetSize() const
{
    return m_data->m_size;
}

old_image Framebuffer::GetImage() const
{
    old_image ret(m_data->m_size);

    u8vec4 *buffer = ret.lock<PixelFormat::RGBA_8>();
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glReadPixels(0, 0, m_data->m_size.x, m_data->m_size.y,
                 GL_RGBA, GL_UNSIGNED_BYTE, buffer);
#endif
    ret.unlock(buffer);

    return ret;
}

void Framebuffer::Bind()
{
    if (m_data->m_bound)
        msg::error("trying to bind an already bound framebuffer");

#if defined GL_VERSION_1_1 || defined GL_ES_VERSION_2_0
    glBindFramebuffer(GL_FRAMEBUFFER, m_data->m_fbo);
#elif defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_data->m_fbo);
#endif

    /* FIXME: this should be done in the RenderContext object
     * instead, maybe by getting rid of Framebuffer::Bind() and
     * creating RenderContext::SetFramebuffer() instead. */
    auto renderer = Scene::GetScene(0).get_renderer();
    m_data->m_saved_viewport = renderer->viewport();
    renderer->viewport(ibox2(ivec2::zero, m_data->m_size));
    m_data->m_bound = true;
}

void Framebuffer::Unbind()
{
    if (!m_data->m_bound)
        msg::error("trying to unbind an unbound framebuffer");

#if defined GL_VERSION_1_1 || defined GL_ES_VERSION_2_0
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#elif defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
#endif

    auto renderer = Scene::GetScene(0).get_renderer();
    renderer->viewport(m_data->m_saved_viewport);
    m_data->m_bound = false;
}

} // namespace lol
