
#include "layer.h"

Layer::Layer(int w, int h, FILE *fp)
{
    width = w;
    height = h;
    data = new unsigned int[w * h];

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

unsigned int Layer::GetTile(int x, int y)
{
    return data[y * width + x];
}

