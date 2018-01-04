//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
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

namespace lol
{

class TextureImageData
{
    friend class TextureImage;

    //T'was protected .... should it be ?
public:
    std::string m_name;

    /* Pixels, then texture coordinates */
    ivec2 m_image_size, m_texture_size;

    Image *m_image = nullptr;
    Texture *m_texture = nullptr;
};

} /* namespace lol */
