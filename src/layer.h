
/*
 * The layer object
 */

#if !defined __DH_LAYER_H__
#define __DH_LAYER_H__

#include <stdint.h>

#include "scene.h"

class Layer
{
public:
    Layer(int w, int h, int z, uint32_t *data);
    ~Layer();

    int GetZ();

    void Render(Scene *scene, int x, int y, int z);

private:
    int width, height, altitude;
    uint32_t *data;
};

#endif // __DH_LAYER_H__

