
/*
 * The layer object
 */

#if !defined __DH_LAYER_H__
#define __DH_LAYER_H__

#include <cstdio>

class Layer
{
public:
    Layer(char const *name, int w, int h, FILE *fp);
    ~Layer();

    int GetZ();
    unsigned int GetTile(int x, int y);

private:
    int width, height, z;
    unsigned int *data;
};

#endif // __DH_LAYER_H__

