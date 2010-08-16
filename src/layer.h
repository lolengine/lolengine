//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Layer class
// ---------------
// A Layer object is a subset of a Map.
//

#if !defined __DH_LAYER_H__
#define __DH_LAYER_H__

#include <stdint.h>

#include "scene.h"

class Layer
{
public:
    Layer(int w, int h, int z, int o, uint32_t *data);
    ~Layer();

    int GetZ();

    void Render(Scene *scene, int x, int y, int z);

private:
    int width, height, altitude, orientation;
    uint32_t *data;
};

#endif // __DH_LAYER_H__

