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

/*
#include <lol/image/image.h>
#include <lol/gpu/texture.h>

#include <stdint.h>

#include "engine/entity.h"
*/
#include "textureimage.h"

#include <vector> // std::vector

namespace lol
{

class TextureImageData;
class TileSetData;

class TileSet : public TextureImage
{
    typedef TextureImage super;

public:
    static TileSet *create(std::string const &path);
    static TileSet *create(std::string const &path, image* img);
    static TileSet *create(std::string const &path, image* img, std::vector<ibox2>& tiles);

    /* Old style: path to PNG file */
    static TileSet *create(std::string const &path, ivec2 size, ivec2 count);
    static TileSet *create(std::string const &path, image* img, ivec2 size, ivec2 count);

    static void destroy(TileSet *);

    virtual ~TileSet();

private:
    TileSet(std::string const &path);
    TileSet(std::string const &path, image *img);

protected:
    virtual void Init(std::string const &path, ResourceCodecData* loaded_data);
    virtual void Init(std::string const &path, image* img);

public:
    /* Inherited from entity */
    virtual std::string GetName() const;

    /* New methods */
    void clear_all();
    int define_tile(ibox2 rect);
    void define_tile(ivec2 count);
    void define_tiles_by_box(std::vector<ibox2>& tiles);
    int GetTileCount() const;
    ivec2 GetTileSize(int tileid) const;
    ibox2 GetTilePixel(int tileid) const;
    box2 GetTileTexel(int tileid) const;

    void SetPalette(TileSet* palette);
    TileSet* GetPalette();
    TileSet const * GetPalette() const;
    void BlitTile(uint32_t id, mat4 model, vec3 *vertex, vec2 *texture);

protected:
    TileSetData *m_tileset_data;
    TileSet *m_palette;
};

} /* namespace lol */

