//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Layer class
// ---------------
// A Layer object is a subset of a Map.
//

#if !defined __LOL_LAYER_H__
#define __LOL_LAYER_H__

#include <stdint.h>

namespace lol
{

class Layer
{
public:
    Layer(int w, int h, int z, int o, uint32_t *data);
    ~Layer();

    int GetZ();

    void Render(int x, int y, int z);

private:
    int width, height, altitude, orientation;
    uint32_t *data;
};

} /* namespace lol */

#endif // __LOL_LAYER_H__

