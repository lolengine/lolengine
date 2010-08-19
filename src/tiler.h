//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
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
    static int Register(char const *path);
    static void Deregister(int id);

    static void BlitTile(uint32_t code, int x, int y, int z, int o);
};

#endif // __DH_TILER_H__

