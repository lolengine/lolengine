
#include "layer.h"

Layer::Layer(int w, int h, int in_z, uint32_t *in_data)
{
    width = w;
    height = h;
    z = in_z;
    data = in_data;

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
    return z;
}

unsigned int Layer::GetTile(int x, int y)
{
    return data[y * width + x];
}

