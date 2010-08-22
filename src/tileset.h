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

#include "entity.h"

class TileSetData;

class TileSet : public Entity
{
public:
    TileSet(char const *path);
    virtual ~TileSet();

protected:
    /* Inherited from Entity */
    virtual char const *GetName();
    virtual Group GetGroup();
    virtual void TickDraw(float deltams);

public:
    /* New methods */
    void BlitTile(uint32_t id, int x, int y, int z, int o);

private:
    TileSetData *data;
};

#endif // __DH_TILESET_H__

