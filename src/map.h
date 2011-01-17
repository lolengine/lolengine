//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
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

