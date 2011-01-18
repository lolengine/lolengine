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
// The Scene class
// ---------------
//

#if !defined __DH_SCENE_H__
#define __DH_SCENE_H__

#include <stdint.h>

class SceneData;

class Scene
{
public:
    Scene(float angle);
    ~Scene();

    void AddTile(uint32_t code, int x, int y, int z, int o);
    void Render();

private:
    SceneData *data;
};

#endif // __DH_SCENE_H__

