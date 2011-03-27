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

#if !defined __LOL_SCENE_H__
#define __LOL_SCENE_H__

#include <stdint.h>

namespace lol
{

class SceneData;

class Scene
{
public:
    Scene(float angle);
    ~Scene();

    static Scene *GetDefault();
    static void Reset();

    void AddTile(uint32_t code, int x, int y, int z, int o);
    void Render();

private:
    SceneData *data;
};

} /* namespace lol */

#endif // __LOL_SCENE_H__

