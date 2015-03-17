//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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
    String m_name;

    /* Pixels, then texture coordinates */
    ivec2 m_image_size, m_texture_size;

    Image *m_image = nullptr;
    Texture *m_texture = nullptr;
};

} /* namespace lol */
