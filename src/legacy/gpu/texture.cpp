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

#include "lolgl.h"

namespace lol
{

//
// The TextureData class
// ---------------------
//

class TextureData
{
    friend class Texture;

    ivec2 m_size;
    PixelFormat m_format;

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    GLuint m_texture;
    GLint m_internal_format;
    GLenum m_gl_format, m_gl_type;
#endif
    int m_bytes_per_elem;
};

//
// The Texture class
// -----------------
//

#define GET_CLAMPED(array, index) \
    array[std::max(0, std::min((int)(index), \
                   (int)sizeof(array) / (int)sizeof(*array)))]

Texture::Texture(ivec2 size, PixelFormat format)
  : m_data(new TextureData)
{
    m_data->m_size = size;
    m_data->m_format = format;

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    static struct
    {
        GLint internal_format;
        GLenum format, type;
        int bytes;
    }
    const gl_formats[] =
    {
        { 0, 0, 0, 0 }, /* Unknown */

        /* FIXME: this is all mixed up for the RGBA/ARGB combinations */
#if defined HAVE_GLES_2X
        { GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, 1 },
        { GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 3 },
        { GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 4 },
        /* FIXME: if GL_RGBA is not available, we should advertise
         * this format as "not available" on this platform. */
#else
        { GL_R8, GL_RED, GL_UNSIGNED_BYTE, 1 }, /* A8 */
        { GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 3 }, /* RGB_8 */
        /* Seems efficient for little endian textures */
        { GL_RGBA8, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, 4 }, /* ARGB_8 */
#endif
        { 0, 0, 0, 0 }, /* Y_F32 */
        { 0, 0, 0, 0 }, /* RGB_F32 */
        { 0, 0, 0, 0 }, /* RGBA_F32 */
    };

    m_data->m_internal_format = GET_CLAMPED(gl_formats, format).internal_format;
    m_data->m_gl_format = GET_CLAMPED(gl_formats, format).format;
    m_data->m_gl_type = GET_CLAMPED(gl_formats, format).type;
    m_data->m_bytes_per_elem = GET_CLAMPED(gl_formats, format).bytes;

    glGenTextures(1, &m_data->m_texture);
    glBindTexture(GL_TEXTURE_2D, m_data->m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#endif
}

TextureUniform Texture::GetTextureUniform() const
{
    TextureUniform ret;
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    ret.m_flags = m_data->m_texture;
#endif
    return ret;
}

void Texture::Bind()
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glBindTexture(GL_TEXTURE_2D, m_data->m_texture);
#endif
}

void Texture::SetData(void *data)
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glTexImage2D(GL_TEXTURE_2D, 0, m_data->m_internal_format,
                 m_data->m_size.x, m_data->m_size.y, 0,
                 m_data->m_gl_format, m_data->m_gl_type, data);
#endif
}

void Texture::SetSubData(ivec2 origin, ivec2 size, void *data)
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glTexSubImage2D(GL_TEXTURE_2D, 0, origin.x, origin.y, size.x, size.y,
                    m_data->m_gl_format, m_data->m_gl_type, data);
#endif
}

void Texture::SetMagFiltering(TextureMagFilter filter)
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glBindTexture(GL_TEXTURE_2D, m_data->m_texture);
    GLenum gl_filter;
    switch (filter)
    {
        case TextureMagFilter::LINEAR_TEXEL:
            gl_filter = GL_LINEAR;
            break;
        case TextureMagFilter::NEAREST_TEXEL:
        default:
            gl_filter = GL_NEAREST;
            break;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter);
#endif
}

void Texture::SetMinFiltering(TextureMinFilter filter)
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glBindTexture(GL_TEXTURE_2D, m_data->m_texture);
    GLenum gl_filter;
    switch (filter)
    {
        case TextureMinFilter::LINEAR_TEXEL_NO_MIPMAP:
            gl_filter = GL_LINEAR;
            break;
        case TextureMinFilter::NEAREST_TEXEL_NEAREST_MIPMAP:
            gl_filter = GL_NEAREST_MIPMAP_NEAREST;
            break;
        case TextureMinFilter::NEAREST_TEXEL_LINEAR_MIPMAP:
            gl_filter = GL_NEAREST_MIPMAP_LINEAR;
            break;
        case TextureMinFilter::LINEAR_TEXEL_NEAREST_MIPMAP:
            gl_filter = GL_LINEAR_MIPMAP_NEAREST;
            break;
        case TextureMinFilter::LINEAR_TEXEL_LINEAR_MIPMAP:
            gl_filter = GL_LINEAR_MIPMAP_LINEAR;
            break;
        case TextureMinFilter::NEAREST_TEXEL_NO_MIPMAP:
        default:
            gl_filter = GL_NEAREST;
            break;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter);
#endif
}

void Texture::GenerateMipmaps()
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glBindTexture(GL_TEXTURE_2D, m_data->m_texture);
    glGenerateMipmap(GL_TEXTURE_2D);
#endif
}

Texture::~Texture()
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glDeleteTextures(1, &m_data->m_texture);
#endif
    delete m_data;
}

} // namespace lol
