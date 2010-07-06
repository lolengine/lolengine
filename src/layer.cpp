
#include "layer.h"

Layer::Layer(int w, int h, int z, char const *base64)
{
    width = w;
    height = h;
    level = z;
    data = new unsigned int[w * h];

#if 0
    fread(data, sizeof(unsigned int), width * height, fp);
    for (int n = 0; n < width * height; n++)
    {
        unsigned int i = data[n];
        // XXX: endianness swapping might be necessary here
        data[n] = i ? i - 1 : 0;
    }
#endif
}

Layer::~Layer()
{
    delete data;
}

int Layer::GetZ()
{
    return level;
}

unsigned int Layer::GetTile(int x, int y)
{
    return data[y * width + x];
}

