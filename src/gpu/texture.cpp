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
// The TextureData class
// ---------------------
//

class TextureData
{
    friend class Texture;

    ivec2 m_size;

#if defined USE_D3D9
    IDirect3DTexture9 *m_tex;
#elif defined _XBOX
    D3DTexture *m_tex;
#else
    GLuint m_texid;
#endif
};

//
// The Texture class
// -----------------
//

/* FIXME: this is all hardcoded over the place */
#if __CELLOS_LV2__
static GLint const INTERNAL_FORMAT = GL_ARGB_SCE;
static GLenum const TEXTURE_FORMAT = GL_BGRA;
static GLenum const TEXTURE_TYPE = GL_UNSIGNED_INT_8_8_8_8_REV;
#elif defined __native_client__ || defined HAVE_GLES_2X
static GLint const INTERNAL_FORMAT = GL_RGBA;
static GLenum const TEXTURE_FORMAT = GL_RGBA;
static GLenum const TEXTURE_TYPE = GL_UNSIGNED_BYTE;
#else
/* Seems efficient for little endian textures */
static GLint const INTERNAL_FORMAT = GL_RGBA;
static GLenum const TEXTURE_FORMAT = GL_BGRA;
static GLenum const TEXTURE_TYPE = GL_UNSIGNED_INT_8_8_8_8_REV;
#endif

Texture::Texture(ivec2 size)
  : m_data(new TextureData)
{
    m_data->m_size = size;

#if defined USE_D3D9
    g_d3ddevice->CreateTexture(m_data->m_size.x, m_data->m_size.y, 1,
                               D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8,
                               D3DPOOL_SYSTEMMEM, &m_tex, NULL);
#elif defined _XBOX
    /* By default the X360 will swizzle the texture. Ask for linear. */
    g_d3ddevice->CreateTexture(m_data->m_size.x, m_data->m_size.y, 1,
                               D3DUSAGE_WRITEONLY, D3DFMT_LIN_A8R8G8B8,
                               D3DPOOL_DEFAULT, &m_tex, NULL);
#else
    glGenTextures(1, &m_data->m_texid);
    glBindTexture(GL_TEXTURE_2D, m_data->m_texid);

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

void Texture::Bind()
{
#if defined _XBOX || defined USE_D3D9
    g_d3ddevice->SetTexture(0, m_data->m_tex);
#else
#   if !defined HAVE_GLES_2X
    glEnable(GL_TEXTURE_2D);
#   endif
    glBindTexture(GL_TEXTURE_2D, m_data->m_texid);
#endif
}

void Texture::SetData(void *data)
{
#if defined _XBOX || defined USE_D3D9
    D3DLOCKED_RECT rect;
#   if defined _XBOX
    m_data->m_tex->LockRect(0, &rect, NULL, D3DLOCK_NOOVERWRITE);
#   else
    m_data->m_tex->LockRect(0, &rect, NULL,
                            D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE);
#   endif
    memcpy(rect.pBits, data, rect.Pitch * rect.Height);

    m_data->m_tex->UnlockRect(0);

#else
    glTexImage2D(GL_TEXTURE_2D, 0, INTERNAL_FORMAT,
                 m_data->m_size.x, m_data->m_size.y, 0,
                 TEXTURE_FORMAT, TEXTURE_TYPE, data);
#endif
}

void Texture::SetSubData(ivec2 origin, ivec2 size, void *data)
{
#if defined _XBOX || defined USE_D3D9
    D3DLOCKED_RECT rect;
#   if defined _XBOX
    m_data->m_tex->LockRect(0, &rect, NULL, D3DLOCK_NOOVERWRITE);
#   else
    m_data->m_tex->LockRect(0, &rect, NULL,
                            D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE);
#   endif
    for (int j = 0; j < size.y; j++)
    {
        uint8_t *dst = (uint8_t *)rect.pBits + (origin.y + j) * rect.Pitch;
        /* FIXME: the source or destination pitch isn't necessarily 4! */
        uint8_t *src = (uint8_t *)data + j * size.y * 4;
        memcpy(dst, src, size.y * 4);
    }

    m_data->m_tex->UnlockRect(0);

#else
    glTexSubImage2D(GL_TEXTURE_2D, 0, origin.x, origin.y, size.x, size.y,
                    TEXTURE_FORMAT, TEXTURE_TYPE, data);
#endif
}

Texture::~Texture()
{
#if defined USE_D3D9 || defined _XBOX
    m_data->m_tex->Release();
#else
    glDeleteTextures(1, &m_data->m_texid);
#endif

    delete m_data;
}

} /* namespace lol */

