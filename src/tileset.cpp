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

#include <cstdlib>
#include <cstdio>
#include <cstring>

#if defined WIN32 && !defined _XBOX
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   if defined USE_D3D9
#       define FAR
#       define NEAR
#       include <d3d9.h>
#   endif
#endif

#include "core.h"
#include "lolgl.h"

using namespace std;

#if defined USE_D3D9
extern IDirect3DDevice9 *g_d3ddevice;
#elif defined _XBOX
extern D3DDevice *g_d3ddevice;
#endif

namespace lol
{

/*
 * TileSet implementation class
 */

class TileSetData
{
    friend class TileSet;

private:
    char *name, *path;
    int *tiles, ntiles;
    ivec2 size, isize, count;
    vec2 scale;
    float tx, ty;

    Image *img;
#if defined USE_D3D9
    IDirect3DTexture9 *m_tex;
#elif defined _XBOX
    D3DTexture *m_tex;
#else
    GLuint m_tex;
#endif
};

/*
 * Public TileSet class
 */

TileSet::TileSet(char const *path, ivec2 size, ivec2 count)
  : data(new TileSetData())
{
    data->name = (char *)malloc(10 + strlen(path) + 1);
    data->path = data->name + 10;
    sprintf(data->name, "<tileset> %s", path);

    data->tiles = NULL;
    data->m_tex = 0;
    data->img = new Image(path);
    data->isize = data->img->GetSize();

    if (count.x > 0 && count.y > 0)
    {
        data->count = count;
        data->size = data->isize / count;
    }
    else
    {
        if (size.x <= 0 || size.y <= 0)
            size = ivec2(32, 32);
        data->count.x = data->isize.x > size.x ? data->isize.x / size.x : 1;
        data->count.y = data->isize.y > size.y ? data->isize.y / size.y : 1;
        data->size = size;
    }

    data->tx = (float)data->size.x / PotUp(data->isize.x);
    data->ty = (float)data->size.y / PotUp(data->isize.y);

    data->ntiles = data->count.x * data->count.y;

    m_drawgroup = DRAWGROUP_BEFORE;
}

TileSet::~TileSet()
{
    free(data->tiles);
    free(data->name);
    delete data;
}

void TileSet::TickDraw(float seconds)
{
    Entity::TickDraw(seconds);

    if (IsDestroying())
    {
        if (data->img)
            delete data->img;
        else
#if defined USE_D3D9 || defined _XBOX
            /* FIXME: is it really the correct call? */
            data->m_tex->Release();
#else
            glDeleteTextures(1, &data->m_tex);
#endif
    }
    else if (data->img)
    {
#if defined USE_D3D9 || defined _XBOX
        D3DFORMAT format;
#else
        GLuint format;
#endif
        int planes;

        switch (data->img->GetFormat())
        {
        case Image::FORMAT_RGB:
#if defined USE_D3D9
           format = D3DFMT_R8G8B8;
#elif defined _XBOX
           format = D3DFMT_LIN_A8R8G8B8; /* FIXME */
#else
           format = GL_RGB;
#endif
           planes = 3;
           break;
        case Image::FORMAT_RGBA:
        default:
#if defined USE_D3D9
           format = D3DFMT_A8R8G8B8;
#elif defined _XBOX
            /* By default the X360 will swizzle the texture. Ask for linear. */
           format = D3DFMT_LIN_A8R8G8B8;
#else
           format = GL_RGBA;
#endif
           planes = 4;
           break;
        }

        int w = PotUp(data->isize.x);
        int h = PotUp(data->isize.y);

        uint8_t *pixels = (uint8_t *)data->img->GetData();
        if (w != data->isize.x || h != data->isize.y)
        {
            uint8_t *tmp = (uint8_t *)malloc(planes * w * h);
            for (int line = 0; line < data->isize.y; line++)
                memcpy(tmp + planes * w * line,
                       pixels + planes * data->isize.x * line,
                       planes * data->isize.x);
            pixels = tmp;
        }

#if defined USE_D3D9 || defined _XBOX
        D3DLOCKED_RECT rect;
        HRESULT hr;
#   if defined USE_D3D9
        hr = g_d3ddevice->CreateTexture(w, h, 1, D3DUSAGE_DYNAMIC, format,
                                        D3DPOOL_DEFAULT, &data->m_tex, NULL);
#   elif defined _XBOX
        hr = g_d3ddevice->CreateTexture(w, h, 1, D3DUSAGE_WRITEONLY, format,
                                        D3DPOOL_DEFAULT, &data->m_tex, NULL);
#   endif
        if (FAILED(hr))
            Abort();
#   if defined USE_D3D9
        hr = data->m_tex->LockRect(0, &rect, NULL, D3DLOCK_DISCARD);
#   else
        hr = data->m_tex->LockRect(0, &rect, NULL, 0);
#   endif
        if (FAILED(hr))
            Abort();
        for (int j = 0; j < h; j++)
            memcpy((uint8_t *)rect.pBits + j * rect.Pitch, pixels + w * j * 4, w * 4);
        hr = data->m_tex->UnlockRect(0);
        if (FAILED(hr))
            Abort();
#else
        glGenTextures(1, &data->m_tex);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, data->m_tex);

        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0,
                     format, GL_UNSIGNED_BYTE, pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#endif

        if (pixels != data->img->GetData())
            free(pixels);
        delete data->img;
        data->img = NULL;
    }
}

char const *TileSet::GetName()
{
    return data->name;
}

ivec2 TileSet::GetCount() const
{
    return data->count;
}

ivec2 TileSet::GetSize(int tileid) const
{
    (void)tileid;

    return data->size;
}

void TileSet::Bind()
{
    if (!data->img && data->m_tex)
    {
#if defined USE_D3D9 || defined _XBOX
        HRESULT hr = g_d3ddevice->SetTexture(0, data->m_tex);
        if (FAILED(hr))
            Abort();
#else
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, data->m_tex);
#endif
    }
}

void TileSet::Unbind()
{
    if (!data->img && data->m_tex)
    {
#if defined USE_D3D9 || defined _XBOX
        HRESULT hr = g_d3ddevice->SetTexture(0, NULL);
        if (FAILED(hr))
            Abort();
#else
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
#endif
    }
}

void TileSet::BlitTile(uint32_t id, vec3 pos, int o, vec2 scale,
                       float *vertex, float *texture)
{
    float tx = data->tx * ((id & 0xffff) % data->count.x);
    float ty = data->ty * ((id & 0xffff) / data->count.x);

    int dx = data->size.x * scale.x;
    int dy = o ? 0 : data->size.y * scale.y;
    int dz = o ? data->size.y * scale.y : 0;

    if (!data->img && data->m_tex)
    {
        float tmp[10];

        *vertex++ = pos.x + dx;
        *vertex++ = pos.y + dy;
        *vertex++ = pos.z + dz;
        *texture++ = tx + data->tx;
        *texture++ = ty;

        *vertex++ = tmp[0] = pos.x;
        *vertex++ = tmp[1] = pos.y + dy;
        *vertex++ = tmp[2] = pos.z + dz;
        *texture++ = tmp[3] = tx;
        *texture++ = tmp[4] = ty;

        *vertex++ = tmp[5] = pos.x + dx;
        *vertex++ = tmp[6] = pos.y;
        *vertex++ = tmp[7] = pos.z;
        *texture++ = tmp[8] = tx + data->tx;
        *texture++ = tmp[9] = ty + data->ty;

        *vertex++ = tmp[5];
        *vertex++ = tmp[6];
        *vertex++ = tmp[7];
        *texture++ = tmp[8];
        *texture++ = tmp[9];

        *vertex++ = tmp[0];
        *vertex++ = tmp[1];
        *vertex++ = tmp[2];
        *texture++ = tmp[3];
        *texture++ = tmp[4];

        *vertex++ = pos.x;
        *vertex++ = pos.y;
        *vertex++ = pos.z;
        *texture++ = tx;
        *texture++ = ty + data->ty;
    }
    else
    {
        memset(vertex, 0, 3 * sizeof(float));
        memset(texture, 0, 2 * sizeof(float));
    }
}

} /* namespace lol */

