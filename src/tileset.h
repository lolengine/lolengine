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

#if !defined __LOL_TILESET_H__
#define __LOL_TILESET_H__

#include <stdint.h>

#include "entity.h"

namespace lol
{

class TileSetData;

class TileSet : public Entity
{
public:
    TileSet(char const *path, ivec2 size, ivec2 count, float dilate);
    virtual ~TileSet();

protected:
    /* Inherited from Entity */
    virtual char const *GetName();
    virtual void TickDraw(float deltams);

public:
    /* New methods */
    ivec2 GetCount() const;
    ivec2 GetSize(int tileid) const;
    void Bind();
    void BlitTile(uint32_t id, vec3 pos, int o,
                  float *vertex, float *texture);

private:
    TileSetData *data;
};

} /* namespace lol */

#endif // __LOL_TILESET_H__

