//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
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
    Scene();
    ~Scene();

    void AddTile(uint32_t code, int x, int y, int z);
    void Render();

private:
    SceneData *data;
};

#endif // __DH_SCENE_H__

