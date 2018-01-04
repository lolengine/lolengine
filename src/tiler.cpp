//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
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

TileSet *Tiler::Register(std::string const &path, ivec2 size, ivec2 count)
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

TileSet *Tiler::Register(std::string const &path)
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

TileSet *Tiler::Register(std::string const &path, Image* image, ivec2 size, ivec2 count)
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

TileSet *Tiler::Register(std::string const &path, Image* image)
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

