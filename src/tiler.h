//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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
    static TileSet *Register(String const &path, ivec2 size, ivec2 count);
    static TileSet *Register(char const *path, ivec2 size, ivec2 count);
    static TileSet *Register(String const &path);
    static TileSet *Register(char const *path);
    static TileSet *Register(String const &path, Image* image, ivec2 size, ivec2 count);
    static TileSet *Register(char const *path, Image* image, ivec2 size, ivec2 count);
    static TileSet *Register(String const &path, Image* image);
    static TileSet *Register(char const *path, Image* image);
    static void Deregister(TileSet *);

private:
    Tiler() {}
};

} /* namespace lol */

