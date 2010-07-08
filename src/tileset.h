
/*
 * The tile manager
 */

#if !defined __DH_TILESET_H__
#define __DH_TILESET_H__

#include <stdint.h>

class TileSetData;

class TileSet
{
public:
    TileSet(char const *path);
    ~TileSet();

    void Ref();
    int Unref();

    char const *GetName();

    void BlitTile(uint32_t id, int x, int y);

private:
    TileSetData *data;
};

#endif // __DH_TILESET_H__

