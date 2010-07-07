
/*
 * The tile manager
 */

#if !defined __DH_TILESET_H__
#define __DH_TILESET_H__

class TileSetData;

class TileSet
{
public:
    TileSet(char const *path);
    ~TileSet();

    void Ref();
    int Unref();

    char const *GetName();

    void AddTile(int n, int x, int y, int z);

    void Render();

private:
    TileSetData *data;
};

#endif // __DH_TILESET_H__

