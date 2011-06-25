//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Tiler class
// ---------------
// The Tiler is a static class that manages tilesets.
//

#if !defined __LOL_TILER_H__
#define __LOL_TILER_H__

#include <stdint.h>

namespace lol
{

class Tiler
{
public:
    static int Register(char const *path, vec2i size, vec2i count,
                        float dilate);
    static void Deregister(int id);

    static vec2i GetCount(int id);
    static vec2i GetSize(int id, int tileid);
    static void Bind(uint32_t code);
    static void BlitTile(uint32_t code, int x, int y, int z, int o,
                         float *vertex, float *texture);
};

} /* namespace lol */

#endif // __LOL_TILER_H__

