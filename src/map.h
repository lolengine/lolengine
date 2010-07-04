
/*
 * The map object
 */

#if !defined __DH_MAP_H__
#define __DH_MAP_H__

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

#endif // __DH_MAP_H__

