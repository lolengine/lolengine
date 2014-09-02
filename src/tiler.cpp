//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

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
    { }

private:
    Dict tilesets;
}
tilerdata;

static TilerData * const data = &tilerdata;

/*
 * Public Tiler class
 */

TileSet *Tiler::Register(String const &path, ivec2 size, ivec2 count)
{
    return Tiler::Register(path.C(), size, count);
}
TileSet *Tiler::Register(char const *path, ivec2 size, ivec2 count)
{
    int id = data->tilesets.MakeSlot(path);
    TileSet *tileset = (TileSet *)data->tilesets.GetEntity(id);

    if (!tileset)
    {
        tileset = new TileSet(path, size, count);
        data->tilesets.SetEntity(id, tileset);
    }

    return tileset;
}

TileSet *Tiler::Register(String const &path)
{
    return Tiler::Register(path.C());
}
TileSet *Tiler::Register(char const *path)
{
    int id = data->tilesets.MakeSlot(path);
    TileSet *tileset = (TileSet *)data->tilesets.GetEntity(id);

    if (!tileset)
    {
        tileset = new TileSet(path);
        data->tilesets.SetEntity(id, tileset);
    }

    return tileset;
}

TileSet *Tiler::Register(String const &path, Image* image, ivec2 size, ivec2 count)
{
    return Tiler::Register(path.C(), image, size, count);
}
TileSet *Tiler::Register(char const *path, Image* image, ivec2 size, ivec2 count)
{
    int id = data->tilesets.MakeSlot(path);
    TileSet *tileset = (TileSet *)data->tilesets.GetEntity(id);

    if (!tileset)
    {
        tileset = new TileSet(path, image, size, count);
        data->tilesets.SetEntity(id, tileset);
    }

    return tileset;
}

TileSet *Tiler::Register(String const &path, Image* image)
{
    return Tiler::Register(path.C(), image);
}
TileSet *Tiler::Register(char const *path, Image* image)
{
    int id = data->tilesets.MakeSlot(path);
    TileSet *tileset = (TileSet *)data->tilesets.GetEntity(id);

    if (!tileset)
    {
        tileset = new TileSet(path, image);
        data->tilesets.SetEntity(id, tileset);
    }

    return tileset;
}

void Tiler::Deregister(TileSet *tileset)
{
    data->tilesets.RemoveSlot(tileset);
}

} /* namespace lol */

