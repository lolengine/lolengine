
/*
 * The tile manager
 */

#if !defined __DH_TILESET_H__
#define __DH_TILESET_H__

#include <stdint.h>

#include "asset.h"

class TileSetData;

class TileSet : public Asset
{
public:
    TileSet(char const *path);
    ~TileSet();

    char const *GetName();

    void BlitTile(uint32_t id, int x, int y);

private:
    TileSetData *data;
};

#endif // __DH_TILESET_H__

