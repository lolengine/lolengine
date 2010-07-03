
#include <cstdio>

#include "layer.h"
#include "tiler.h"

class Map
{
public:
    Map(char const *path);
    ~Map();

    void Draw(Tiler *tiler);

private:
    Layer **layers;
    int nlayers;
};

