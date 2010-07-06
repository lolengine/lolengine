
/*
 * The layer object
 */

#if !defined __DH_LAYER_H__
#define __DH_LAYER_H__

#include <cstdio>

class Layer
{
public:
    Layer(int w, int h, int z, char const *base64);
    ~Layer();

    int GetZ();
    unsigned int GetTile(int x, int y);

private:
    int width, height, level;
    unsigned int *data;
};

#endif // __DH_LAYER_H__

