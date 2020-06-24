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

#pragma once

//
// The TileSet class
// -----------------
// A TileSet is a collection of tiles stored in a texture. Texture uploading
// and freeing is done in the render tick method. When the refcount drops to
// zero, the texture is freed.
//

#include <lol/image/resource.h>
#include <lol/image/image.h>
#include <lol/gpu/texture.h>

#include <stdint.h>

#include "engine/entity.h"

namespace lol
{

class TextureImageData;

class TextureImage : public entity
{
    typedef entity super;
protected:
    virtual TextureImageData* GetNewData();

public:
    TextureImage(std::string const &path);
    TextureImage(std::string const &path, old_image* img);
    virtual ~TextureImage();

protected:
    void Init(std::string const &path);
    virtual void Init(std::string const &path, ResourceCodecData* loaded_data);
    virtual void Init(std::string const &path, old_image* img);

protected:
    virtual void tick_draw(float seconds, Scene &scene);

public:
    /* Inherited from entity */
    virtual std::string GetName() const;

    void UpdateTexture(old_image* img);
    Texture * GetTexture();
    Texture const * GetTexture() const;
    old_image * GetImage();
    old_image const * GetImage() const;
    ivec2 GetImageSize() const;
    ivec2 GetTextureSize() const;
    void Bind();
    void Unbind();

protected:
    TextureImageData* m_data = nullptr;
};

} /* namespace lol */

