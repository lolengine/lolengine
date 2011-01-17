//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

/*
 * Tiler implementation class
 */

static class TilerData
{
    friend class Tiler;

public:
    Dict tilesets;
}
tilerdata;

static TilerData * const data = &tilerdata;

/*
 * Public Tiler class
 */

int Tiler::Register(char const *path, int size)
{
    int id = data->tilesets.MakeSlot(path);

    if (!data->tilesets.GetEntity(id))
    {
        TileSet *tileset = new TileSet(path, size);
        data->tilesets.SetEntity(id, tileset);
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
    tileset->BlitTile(code & 0xffff, x, y, z, o);
}

