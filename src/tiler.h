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

#if !defined __DH_TILER_H__
#define __DH_TILER_H__

#include <stdint.h>

class Tiler
{
public:
    static int Register(char const *path, int2 size, int2 count, float dilate);
    static void Deregister(int id);

    static int2 GetSize(int id);
    static int2 GetCount(int id);
    static void BlitTile(uint32_t code, int x, int y, int z, int o);
};

#endif // __DH_TILER_H__

