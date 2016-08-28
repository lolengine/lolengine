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

    GLuint m_texture;
    GLint m_internal_format;
    GLenum m_gl_format, m_gl_type;
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
#if defined __native_client__ || defined HAVE_GLES_2X
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
}

TextureUniform Texture::GetTextureUniform() const
{
    TextureUniform ret;
    ret.m_flags = m_data->m_texture;
    return ret;
}

void Texture::Bind()
{
#if !defined HAVE_GLES_2X
    glEnable(GL_TEXTURE_2D);
#endif
    glBindTexture(GL_TEXTURE_2D, m_data->m_texture);
}

void Texture::SetData(void *data)
{
    glTexImage2D(GL_TEXTURE_2D, 0, m_data->m_internal_format,
                 m_data->m_size.x, m_data->m_size.y, 0,
                 m_data->m_gl_format, m_data->m_gl_type, data);
}

void Texture::SetSubData(ivec2 origin, ivec2 size, void *data)
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, origin.x, origin.y, size.x, size.y,
                    m_data->m_gl_format, m_data->m_gl_type, data);
}

void Texture::SetMagFiltering(TextureMagFilter filter)
{
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
}

void Texture::SetMinFiltering(TextureMinFilter filter)
{
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
}

void Texture::GenerateMipmaps()
{
    glBindTexture(GL_TEXTURE_2D, m_data->m_texture);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_data->m_texture);
    delete m_data;
}

} /* namespace lol */

