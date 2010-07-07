
/*
 * The scene object
 */

#if !defined __DH_SCENE_H__
#define __DH_SCENE_H__

#include <cstdio>
#include <stdint.h>

class SceneData;

class Scene
{
public:
    Scene();
    ~Scene();

private:
    SceneData *data;
};

#endif // __DH_SCENE_H__

