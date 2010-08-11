//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The TileSet class
// -----------------
// A TileSet contains the information necesary to blit tiles to the game
// screen.
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

