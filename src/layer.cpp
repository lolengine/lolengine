//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>

#include "core.h"

using namespace std;

namespace lol
{

Layer::Layer(int w, int h, int z, int o, uint32_t *in_data)
{
    width = w;
    height = h;
    altitude = z;
    orientation = o;
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
    free(data);
}

void Layer::Render(int /* x */, int /* y */, int /* z */)
{
    static int error = 1;
    if (error && !(error = 0))
        Log::Error("FIXME: Layer::Render no longer works\n");
#if 0
    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++)
            if (data[j * width + i])
                scene.AddTile(data[j * width + i],
                              vec3(x + i * 32,
                                   y + j * 32 - altitude,
                                   altitude + z),
                              orientation);
#endif
}

int Layer::GetZ()
{
    return altitude;
}

} /* namespace lol */

