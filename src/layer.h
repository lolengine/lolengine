
#if !defined __LAYER_H__
#define __LAYER_H__

#include <cstdio>

class Layer
{
public:
    Layer(int w, int h, FILE *fp);
    ~Layer();

    unsigned int GetTile(int x, int y);

//private:
    int width, height;
    unsigned int *data;
};

#endif // __LAYER_H__

