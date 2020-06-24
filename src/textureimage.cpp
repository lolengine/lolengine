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

#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
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

TextureImage::TextureImage(std::string const &path)
    : m_data(GetNewData())
{
    Init(path);
}

TextureImage::TextureImage(std::string const &path, old_image* img)
    : m_data(GetNewData())
{
    Init(path, img);
}

TextureImage::~TextureImage()
{
    delete m_data;
}

void TextureImage::Init(std::string const &path)
{
    Init(path, ResourceLoader::Load(path));
}

void TextureImage::Init(std::string const &path, ResourceCodecData* loaded_data)
{
    //Load image if available
    auto image_data = dynamic_cast<ResourceImageData*>(loaded_data);
    if (image_data != nullptr)
    {
        Init(path, new old_image(*image_data->m_image));
    }

    delete image_data;
}

void TextureImage::Init(std::string const &path, old_image* img)
{
    m_data->m_name = "<textureimage> " + path;

    m_data->m_texture = nullptr;
    m_data->m_image = img;
    m_data->m_image_size = m_data->m_image->size();
    m_data->m_texture_size = ivec2(lol::bit_ceil(m_data->m_image_size.x),
                                   lol::bit_ceil(m_data->m_image_size.y));

    m_drawgroup = tickable::group::draw::texture;
}

void TextureImage::tick_draw(float seconds, Scene &scene)
{
    super::tick_draw(seconds, scene);

    if (has_flags(entity::flags::destroying))
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

        PixelFormat format = m_data->m_image->format();
        int planes = BytesPerPixel(format);

        int w = m_data->m_texture_size.x;
        int h = m_data->m_texture_size.y;

        uint8_t *pixels = (uint8_t *)m_data->m_image->lock();
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
        /* FIXME: no unlock? */

        m_data->m_texture = new Texture(ivec2(w, h), format);
        m_data->m_texture->SetData(pixels);

        if (resized)
            delete[] pixels;
        delete m_data->m_image;
        m_data->m_image = nullptr;
    }
}

//-----------------------------------------------------------------------------
std::string TextureImage::GetName() const
{
    return m_data->m_name;
}

void TextureImage::UpdateTexture(old_image* img)
{
    m_data->m_image = img;
    m_data->m_image_size = m_data->m_image->size();
    m_data->m_texture_size = ivec2(lol::bit_ceil(m_data->m_image_size.x),
                                   lol::bit_ceil(m_data->m_image_size.y));
}

Texture * TextureImage::GetTexture()
{
    return m_data->m_texture;
}

Texture const * TextureImage::GetTexture() const
{
    return m_data->m_texture;
}

old_image * TextureImage::GetImage()
{
    return m_data->m_image;
}

old_image const * TextureImage::GetImage() const
{
    return m_data->m_image;
}

ivec2 TextureImage::GetImageSize() const
{
    return m_data->m_image_size;
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

