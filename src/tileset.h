//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
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

#include "asset.h"

class TileSetData;

class TileSet : public Asset
{
public:
    TileSet(char const *path);
    virtual ~TileSet();

    /* Inherited from Asset */
    virtual void TickRender(float delta_time);

    /* New implementations */
    char const *GetName();

    void BlitTile(uint32_t id, int x, int y);

private:
    TileSetData *data;
};

#endif // __DH_TILESET_H__

