//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Map class
// -------------
// A Map object is a collection of Layers and other information (to be
// determined later).
//

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
    int GetWidth();
    int GetHeight();

private:
    MapData *data;
};

#endif // __DH_MAP_H__

