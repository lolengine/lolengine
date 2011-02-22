//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The TileSet class
// -----------------
// A TileSet is a collection of tiles stored in a texture. Texture uploading
// and freeing is done in the render tick method. When the refcount drops to
// zero, the texture is freed.
//

#if !defined __DH_TILESET_H__
#define __DH_TILESET_H__

#include <stdint.h>

#include "entity.h"

namespace lol
{

class TileSetData;

class TileSet : public Entity
{
public:
    TileSet(char const *path, vec2i size, vec2i count, float dilate);
    virtual ~TileSet();

protected:
    /* Inherited from Entity */
    virtual char const *GetName();
    virtual void TickDraw(float deltams);

public:
    /* New methods */
    vec2i GetSize() const;
    vec2i GetCount() const;
    void Bind();
    void BlitTile(uint32_t id, int x, int y, int z, int o,
                  float *vertex, float *texture);

private:
    TileSetData *data;
};

} /* namespace lol */

#endif // __DH_TILESET_H__

