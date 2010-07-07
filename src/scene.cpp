
#include "scene.h"

/*
 * Scene implementation class
 */

class SceneData
{
    friend class Scene;

private:
    int dummy;
};

/*
 * Public Scene class
 */

Scene::Scene()
{
    data = new SceneData();
}

Scene::~Scene()
{
    delete data;
}

