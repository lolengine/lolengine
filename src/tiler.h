

/*
 * The tile manager
 */

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

