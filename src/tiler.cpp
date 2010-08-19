//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "tiler.h"
#include "tileset.h"

#if defined WIN32
#   define strcasecmp _stricmp
#endif

/*
 * Tiler implementation class
 */

static class TilerData
{
    friend class Tiler;

public:
    TilerData() :
        tilesets(0),
        ntilesets(0)
    {
        /* Nothing to do */
    }

    ~TilerData()
    {
        if (ntilesets)
            fprintf(stderr, "ERROR: still %i tilesets in tiler\n", ntilesets);
        free(tilesets);
    }

private:
    TileSet **tilesets;
    int ntilesets;
}
tilerdata;

static TilerData * const data = &tilerdata;

/*
 * Public Tiler class
 */

int Tiler::Register(char const *path)
{
    int id, empty = -1;

    /* If the tileset is already registered, remember its ID. Look for an
     * empty slot at the same time. */
    for (id = 0; id < data->ntilesets; id++)
    {
        TileSet *t = data->tilesets[id];
        if (!t)
            empty = id;
        else if (!strcasecmp(path, t->GetName()))
            break;
    }

    /* If this is a new tileset, create a new one. */
    if (id == data->ntilesets)
    {
        if (empty == -1)
        {
            empty = data->ntilesets++;
            data->tilesets = (TileSet **)realloc(data->tilesets,
                                         data->ntilesets * sizeof(TileSet *));
        }

        data->tilesets[empty] = new TileSet(path);
        id = empty;
    }

    data->tilesets[id]->Ref();
    return id + 1; /* ID 0 is for the empty tileset */
}

void Tiler::Deregister(int id)
{
    --id; /* ID 0 is for the empty tileset */

    if (data->tilesets[id]->Unref() == 0)
        data->tilesets[id] = NULL;
}

void Tiler::BlitTile(uint32_t code, int x, int y, int z, int o)
{
    int id = (code >> 16) - 1; /* ID 0 is for the empty tileset */

    data->tilesets[id]->BlitTile(code & 0xffff, x, y, z, o);
}

