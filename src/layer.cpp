
#include "layer.h"

Layer::Layer(char const *name, int w, int h, FILE *fp)
{
    width = w;
    height = h;
    data = new unsigned int[w * h];

    if (sscanf(name, "Ground %d", &z) == 1)
        ;
    else if (sscanf(name, "Ground Decal %d", &z) == 1)
        ;
    else if (sscanf(name, "Object %d", &z) == 1)
        z++;
    else if (sscanf(name, "Wall %d", &z) == 1)
        z++;
    else if (sscanf(name, "Wall Decal %d", &z) == 1)
        z++;
    else
        z = -1;

    fread(data, sizeof(unsigned int), width * height, fp);
    for (int n = 0; n < width * height; n++)
    {
        unsigned int i = data[n];
        // XXX: endianness swapping might be necessary here
        data[n] = i ? i - 1 : 0;
    }
}

Layer::~Layer()
{
    delete data;
}

int Layer::GetZ()
{
    return z;
}

unsigned int Layer::GetTile(int x, int y)
{
    return data[y * width + x];
}

