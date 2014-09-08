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

#include <lol/image/image.h>
#include <lol/gpu/texture.h>

#include <stdint.h>

#include "entity.h"

namespace lol
{

class TileSetData;

class TileSet : public Entity
{
public:
    TileSet(char const *path);
    TileSet(char const *path, Image* image);
    virtual ~TileSet();

    /* Old style: path to PNG file */
    TileSet(char const *path, ivec2 size, ivec2 count);
    TileSet(char const *path, Image* image, ivec2 size, ivec2 count);

    /* Inherited from Entity */
    virtual char const *GetName();
protected:
    virtual void TickDraw(float seconds, Scene &scene);

public:
    /* New methods */
    ptrdiff_t AddTile(ibox2 rect);
    void AddTile(ivec2 count);
    ptrdiff_t GetTileCount() const;
    ivec2 GetTileSize(ptrdiff_t tileid) const;

    Texture * GetTexture();
    Texture const * GetTexture() const;
    Image * GetImage();
    Image const * GetImage() const;
    void SetPalette(TileSet* palette);
    TileSet* GetPalette();
    TileSet const * GetPalette() const;
    ivec2 GetTextureSize() const;
    void Bind();
    void Unbind();
    void BlitTile(uint32_t id, vec3 pos, int o, vec2 scale, float angle,
                  vec3 *vertex, vec2 *texture);

private:
    void Init(char const *path);
    void Init(char const *path, Image* image);

    TileSetData*    m_data;
    TileSet*        m_palette;
};

} /* namespace lol */

