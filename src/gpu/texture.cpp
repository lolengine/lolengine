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

#include <lol/main.h>
#include "lolgl.h"

#if defined _WIN32 && defined USE_D3D9
#   define FAR
#   define NEAR
#   include <d3d9.h>
#endif

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

#if defined USE_D3D9
    IDirect3DDevice9 *m_dev;
    IDirect3DTexture9 *m_texture;
    D3DTEXTUREFILTERTYPE m_mag_filter;
    D3DTEXTUREFILTERTYPE m_min_filter;
    D3DTEXTUREFILTERTYPE m_mip_filter;
#elif defined _XBOX
    D3DDevice *m_dev;
    D3DTexture *m_texture;
    D3DTEXTUREFILTERTYPE m_mag_filter;
    D3DTEXTUREFILTERTYPE m_min_filter;
    D3DTEXTUREFILTERTYPE m_mip_filter;
#else
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

#if defined USE_D3D9 || defined _XBOX
#   if defined USE_D3D9
    m_data->m_dev = (IDirect3DDevice9 *)g_renderer->GetDevice();
#   elif defined _XBOX
    m_data->m_dev = (D3DDevice *)g_renderer->GetDevice();
#   endif

    static struct
    {
        D3DFORMAT format;
        int bytes;
    }
    const d3d_formats[] =
    {
        /* Unknown */
        { D3DFMT_UNKNOWN, 0 },

        /* FIXME: this is all mixed up for the RGBA/ARGB combinations */
#   if defined USE_D3D9
        { D3DFMT_L8, 1 },       /* Y_8 */
        { D3DFMT_R8G8B8, 3 },   /* RGB_8 */
        { D3DFMT_A8R8G8B8, 4 }, /* RGBA_8 */
        { D3DFMT_UNKNOWN, 0 },  /* Y_F32 */
        { D3DFMT_UNKNOWN, 0 },  /* RGB_F32 */
        { D3DFMT_UNKNOWN, 0 },  /* RGBA_F32 */
#   else
        { D3DFMT_LIN_L8, 1 },
        { D3DFMT_UNKNOWN, 0 },
        /* By default the X360 will swizzle the texture. Ask for linear. */
        { D3DFMT_LIN_A8R8G8B8, 4 },
        { D3DFMT_UNKNOWN, 0 },  /* Y_F32 */
        { D3DFMT_UNKNOWN, 0 },  /* RGB_F32 */
        { D3DFMT_UNKNOWN, 0 },  /* RGBA_F32 */
#   endif
    };

    D3DFORMAT d3d_format = GET_CLAMPED(d3d_formats, format).format;
    ASSERT(d3d_format != D3DFMT_UNKNOWN,
           "unsupported texture format %d\n", format);
#   if defined USE_D3D9
    int d3d_usage = D3DUSAGE_DYNAMIC;
#   else
    int d3d_usage = D3DUSAGE_WRITEONLY;
#   endif

    m_data->m_dev->CreateTexture(m_data->m_size.x, m_data->m_size.y, 1,
                                 d3d_usage, d3d_format,
                                 D3DPOOL_DEFAULT, &m_data->m_texture, nullptr);
    m_data->m_bytes_per_elem = GET_CLAMPED(d3d_formats, format).bytes;
#else
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
#if __CELLOS_LV2__
        { GL_LUMINANCE8, GL_LUMINANCE, GL_UNSIGNED_BYTE, 1 },
        { GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 3 },
        { GL_ARGB_SCE, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, 4 },
#elif defined __native_client__ || defined HAVE_GLES_2X
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

#   if defined __CELLOS_LV2__
    /* We need this hint because by default the storage type is
     * GL_TEXTURE_SWIZZLED_GPU_SCE. */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_ALLOCATION_HINT_SCE,
                    GL_TEXTURE_TILED_GPU_SCE);
#   endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#endif
}

TextureUniform Texture::GetTextureUniform() const
{
    TextureUniform ret;
#if defined USE_D3D9 || defined _XBOX
    ret.m_flags = (uint64_t)(uintptr_t)m_data->m_texture;
    ret.m_attrib = m_data->m_mag_filter;
    ret.m_attrib |= m_data->m_min_filter << 8;
    ret.m_attrib |= m_data->m_mip_filter << 16;
#else
    ret.m_flags = m_data->m_texture;
#endif
    return ret;
}

void Texture::Bind()
{
#if defined _XBOX || defined USE_D3D9
    m_data->m_dev->SetTexture(0, m_data->m_texture);
#else
#   if !defined HAVE_GLES_2X
    glEnable(GL_TEXTURE_2D);
#   endif
    glBindTexture(GL_TEXTURE_2D, m_data->m_texture);
#endif
}

void Texture::SetData(void *data)
{
#if defined _XBOX || defined USE_D3D9
    D3DLOCKED_RECT rect;
#   if defined USE_D3D9
    m_data->m_texture->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD);
#   else
    m_data->m_texture->LockRect(0, &rect, nullptr, 0);
#   endif

    memcpy(rect.pBits, data, rect.Pitch * m_data->m_size.y);

    m_data->m_texture->UnlockRect(0);

#else
    glTexImage2D(GL_TEXTURE_2D, 0, m_data->m_internal_format,
                 m_data->m_size.x, m_data->m_size.y, 0,
                 m_data->m_gl_format, m_data->m_gl_type, data);
#endif
}

void Texture::SetSubData(ivec2 origin, ivec2 size, void *data)
{
#if defined _XBOX || defined USE_D3D9
    D3DLOCKED_RECT rect;
    m_data->m_texture->LockRect(0, &rect, nullptr, 0);

    int stride = size.x * m_data->m_bytes_per_elem;
    for (int j = 0; j < size.y; j++)
    {
        uint8_t *dst = (uint8_t *)rect.pBits + (origin.y + j) * rect.Pitch;
        uint8_t *src = (uint8_t *)data + j * stride;
        memcpy(dst, src, stride);
    }

    m_data->m_texture->UnlockRect(0);

#else
    glTexSubImage2D(GL_TEXTURE_2D, 0, origin.x, origin.y, size.x, size.y,
                    m_data->m_gl_format, m_data->m_gl_type, data);
#endif
}

void Texture::SetMagFiltering(TextureMagFilter filter)
{
#if defined _XBOX || defined USE_D3D9
    // In DirectX, texture filtering is a per-texture-unit state
    switch (filter)
    {
        case TextureMagFilter::LINEAR_TEXEL:
            m_data->m_mag_filter = D3DTEXF_LINEAR;
            break;
        case TextureMagFilter::NEAREST_TEXEL:
        default:
            m_data->m_mag_filter = D3DTEXF_POINT;
            break;
    }
#else
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
#if defined _XBOX || defined USE_D3D9
    // In DirectX, texture filtering is a per-texture-unit state
#define F(x, y) \
    m_data->m_min_filter = x; m_data->m_mip_filter = y;
    switch (filter)
    {
        case TextureMinFilter::LINEAR_TEXEL_NO_MIPMAP:
            F(D3DTEXF_POINT, D3DTEXF_NONE);
            break;
        case TextureMinFilter::NEAREST_TEXEL_NEAREST_MIPMAP:
            F(D3DTEXF_POINT, D3DTEXF_POINT);
            break;
        case TextureMinFilter::LINEAR_TEXEL_NEAREST_MIPMAP:
            F(D3DTEXF_LINEAR, D3DTEXF_POINT);
            break;
        case TextureMinFilter::NEAREST_TEXEL_LINEAR_MIPMAP:
            F(D3DTEXF_POINT, D3DTEXF_LINEAR);
            break;
        case TextureMinFilter::LINEAR_TEXEL_LINEAR_MIPMAP:
            F(D3DTEXF_LINEAR, D3DTEXF_LINEAR);
            break;
        case TextureMinFilter::NEAREST_TEXEL_NO_MIPMAP:
        default:
            F(D3DTEXF_POINT, D3DTEXF_NONE);
            break;
    }
#undef F

#else
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
#if defined USE_D3D9
    m_data->m_texture->->GenerateMipSubLevels();
#elif defined _XBOX
    /* FIXME: No direct mipmap generation support on X360 */
#elif defined __CELLOS_LV2__
    glBindTexture(GL_TEXTURE_2D, m_data->m_texture);
    glGenerateMipmapOES(GL_TEXTURE_2D);
#else
    glBindTexture(GL_TEXTURE_2D, m_data->m_texture);
    glGenerateMipmap(GL_TEXTURE_2D);
#endif
}

Texture::~Texture()
{
#if defined USE_D3D9 || defined _XBOX
    m_data->m_texture->Release();
#else
    glDeleteTextures(1, &m_data->m_texture);
#endif

    delete m_data;
}

} /* namespace lol */

