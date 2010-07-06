
/*
 * The tile manager
 */

#if !defined __DH_TILER_H__
#define __DH_TILER_H__

class TilesetData;

class Tileset
{
public:
    Tileset();
    ~Tileset();

    void AddTile(int n, int x, int y, int z);

    void Render();

private:
    TilesetData *data;
};

#endif // __DH_TILER_H__

