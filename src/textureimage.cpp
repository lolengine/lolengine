//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#if defined _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
#   if defined USE_D3D9
#       define FAR
#       define NEAR
#       include <d3d9.h>
#   endif
#endif

#include "textureimage-private.h"

namespace lol
{

/*
 * TileSet implementation class
 */

TextureImageData* TextureImage::GetNewData()
{
    return new TextureImageData();
}

/*
 * Public TextureImage class
 */

TextureImage::TextureImage(char const *path)
    : m_data(GetNewData())
{
    Init(path);
}

TextureImage::TextureImage(char const *path, Image* image)
    : m_data(GetNewData())
{
    Init(path, image);
}

TextureImage::~TextureImage()
{
    delete m_data;
}

void TextureImage::Init(char const *path)
{
    Init(path, new Image(path));
}

void TextureImage::Init(char const *path, Image* image)
{
    m_data->m_name = String("<textureimage> ") + path;

    m_data->m_texture = nullptr;
    m_data->m_image = image;
    m_data->m_image_size = m_data->m_image->GetSize();
    m_data->m_texture_size = ivec2(PotUp(m_data->m_image_size.x),
                                   PotUp(m_data->m_image_size.y));

    m_drawgroup = DRAWGROUP_TEXTURE;
}

void TextureImage::TickDraw(float seconds, Scene &scene)
{
    super::TickDraw(seconds, scene);

    if (IsDestroying())
    {
        if (m_data->m_image)
        {
            delete m_data->m_image;
            m_data->m_image = nullptr;
        }
        else
        {
            delete m_data->m_texture;
            m_data->m_texture = nullptr;
        }
    }
    else if (m_data->m_image)
    {
        //Update texture is needed
        if (m_data->m_texture)
        {
            delete m_data->m_texture;
            m_data->m_texture = nullptr;
        }

        PixelFormat format = m_data->m_image->GetFormat();
        int planes = BytesPerPixel(format);

        int w = m_data->m_texture_size.x;
        int h = m_data->m_texture_size.y;

        uint8_t *pixels = (uint8_t *)m_data->m_image->Lock();
        bool resized = false;
        if (w != m_data->m_image_size.x || h != m_data->m_image_size.y)
        {
            uint8_t *tmp = new uint8_t[planes * w * h];
            for (int line = 0; line < m_data->m_image_size.y; line++)
                memcpy(tmp + planes * w * line,
                       pixels + planes * m_data->m_image_size.x * line,
                       planes * m_data->m_image_size.x);
            pixels = tmp;
            resized = false;
        }

        m_data->m_texture = new Texture(ivec2(w, h), format);
        m_data->m_texture->SetData(pixels);

        if (resized)
            delete[] pixels;
        delete m_data->m_image;
        m_data->m_image = nullptr;
    }
}

//-----------------------------------------------------------------------------
char const *TextureImage::GetName()
{
    return m_data->m_name.C();
}

void TextureImage::UpdateTexture(Image* image)
{
    m_data->m_image = image;
    m_data->m_image_size = m_data->m_image->GetSize();
    m_data->m_texture_size = ivec2(PotUp(m_data->m_image_size.x),
                                    PotUp(m_data->m_image_size.y));
}

Texture * TextureImage::GetTexture()
{
    return m_data->m_texture;
}

Texture const * TextureImage::GetTexture() const
{
    return m_data->m_texture;
}

Image * TextureImage::GetImage()
{
    return m_data->m_image;
}

Image const * TextureImage::GetImage() const
{
    return m_data->m_image;
}

ivec2 TextureImage::GetTextureSize() const
{
    return m_data->m_texture_size;
}

void TextureImage::Bind()
{
    if (!m_data->m_image && m_data->m_texture)
        m_data->m_texture->Bind();
}

void TextureImage::Unbind()
{
    ;
}

} /* namespace lol */

