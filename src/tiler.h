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

#pragma once

//
// The Tiler class
// ---------------
// The Tiler is a static class that manages tilesets.
//

#include <stdint.h>

#include "tileset.h"

namespace lol
{

class Tiler
{
public:
    static TileSet *Register(std::string const &path, ivec2 size, ivec2 count);
    static TileSet *Register(std::string const &path);
    static TileSet *Register(std::string const &path, image* img, ivec2 size, ivec2 count);
    static TileSet *Register(std::string const &path, image* img);
    static void Deregister(TileSet *);

private:
    Tiler() {}
};

} /* namespace lol */

