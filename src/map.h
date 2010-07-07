
/*
 * The map object
 */

#if !defined __DH_MAP_H__
#define __DH_MAP_H__

#include <cstdio>

#include "layer.h"

class MapData;

class Map
{
public:
    Map(char const *path);
    ~Map();

    void Draw();

private:
    MapData *data;
};

#endif // __DH_MAP_H__

