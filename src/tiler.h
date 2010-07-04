
/*
 * The tile manager
 */

#if !defined __DH_TILER_H__
#define __DH_TILER_H__

class TilerData;

class Tiler
{
public:
    Tiler();
    ~Tiler();

    void AddTile(int n, int x, int y, int z);

    void Render();

private:
    TilerData *data;
};

#endif // __DH_TILER_H__

