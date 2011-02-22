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

namespace lol
{

/*
 * Tiler implementation class
 */

static class TilerData
{
    friend class Tiler;

public:
    TilerData()
#if !LOL_RELEASE
      : lasterror(-1)
#endif
    { }

private:
    Dict tilesets;
#if !LOL_RELEASE
    int lasterror;
#endif
}
tilerdata;

static TilerData * const data = &tilerdata;

/*
 * Public Tiler class
 */

int Tiler::Register(char const *path, vec2i size, vec2i count, float dilate)
{
    int id = data->tilesets.MakeSlot(path);

    if (!data->tilesets.GetEntity(id))
    {
        TileSet *tileset = new TileSet(path, size, count, dilate);
        data->tilesets.SetEntity(id, tileset);
#if !LOL_RELEASE
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

vec2i Tiler::GetSize(int id)
{
    TileSet *tileset = (TileSet *)data->tilesets.GetEntity(id - 1);
#if !LOL_RELEASE
    if (!tileset)
    {
        fprintf(stderr, "ERROR: getting size for null tiler #%i\n", id);
        return 0;
    }
#endif
    return tileset->GetSize();
}

vec2i Tiler::GetCount(int id)
{
    TileSet *tileset = (TileSet *)data->tilesets.GetEntity(id - 1);
#if !LOL_RELEASE
    if (!tileset)
    {
        fprintf(stderr, "ERROR: getting count for null tiler #%i\n", id);
        return 0;
    }
#endif
    return tileset->GetCount();
}

void Tiler::Bind(uint32_t code)
{
    int id = (code >> 16) - 1; /* ID 0 is for the empty tileset */

    TileSet *tileset = (TileSet *)data->tilesets.GetEntity(id);
#if !LOL_RELEASE
    if (!tileset)
    {
        if (id != data->lasterror)
            fprintf(stderr, "ERROR: binding null tiler #%i\n", id);
        data->lasterror = id;
        return;
    }
#endif
    tileset->Bind();
}

void Tiler::BlitTile(uint32_t code, int x, int y, int z, int o,
                     float *vertex, float *texture)
{
    int id = (code >> 16) - 1; /* ID 0 is for the empty tileset */

    TileSet *tileset = (TileSet *)data->tilesets.GetEntity(id);
#if !LOL_RELEASE
    if (!tileset)
    {
        if (id != data->lasterror)
            fprintf(stderr, "ERROR: blitting to null tiler #%i\n", id);
        data->lasterror = id;
        return;
    }
#endif
    tileset->BlitTile(code & 0xffff, x, y, z, o, vertex, texture);
}

} /* namespace lol */

