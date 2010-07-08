
/*
 * The map object
 */

#if !defined __DH_MAP_H__
#define __DH_MAP_H__

#include "scene.h"

class MapData;

class Map
{
public:
    Map(char const *path);
    ~Map();

    void Render(Scene *scene, int x, int y, int z);

private:
    MapData *data;
};

#endif // __DH_MAP_H__

