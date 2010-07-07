
/*
 * The layer object
 */

#if !defined __DH_LAYER_H__
#define __DH_LAYER_H__

#include <cstdio>
#include <stdint.h>

class Layer
{
public:
    Layer(int w, int h, int z, uint32_t *data);
    ~Layer();

    int GetZ();
    unsigned int GetTile(int x, int y);

    void Draw();

private:
    int width, height, z;
    uint32_t *data;
};

#endif // __DH_LAYER_H__

