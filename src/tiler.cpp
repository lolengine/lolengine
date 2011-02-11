//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include "core.h"

/*
 * Tiler implementation class
 */

static class TilerData
{
    friend class Tiler;

public:
    TilerData()
#if !FINAL_RELEASE
      : lasterror(-1)
#endif
    { }

private:
    Dict tilesets;
#if !FINAL_RELEASE
    int lasterror;
#endif
}
tilerdata;

static TilerData * const data = &tilerdata;

/*
 * Public Tiler class
 */

int Tiler::Register(char const *path, int2 size, int2 count, float dilate)
{
    int id = data->tilesets.MakeSlot(path);

    if (!data->tilesets.GetEntity(id))
    {
        TileSet *tileset = new TileSet(path, size, count, dilate);
        data->tilesets.SetEntity(id, tileset);
#if !FINAL_RELEASE
        if (id == data->lasterror)
            data->lasterror = -1;
#endif
    }

    return id + 1; /* ID 0 is for the empty tileset */
}

void Tiler::Deregister(int id)
{
    data->tilesets.RemoveSlot(id - 1); /* ID 0 is for the empty tileset */
}

void Tiler::BlitTile(uint32_t code, int x, int y, int z, int o)
{
    int id = (code >> 16) - 1; /* ID 0 is for the empty tileset */

    TileSet *tileset = (TileSet *)data->tilesets.GetEntity(id);
#if !FINAL_RELEASE
    if (!tileset)
    {
        if (id != data->lasterror)
            fprintf(stderr, "ERROR: blitting to null tiler #%i\n", id);
        data->lasterror = id;
        return;
    }
#endif
    tileset->BlitTile(code & 0xffff, x, y, z, o);
}

