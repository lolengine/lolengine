//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The LevelMap class
// ------------------
// A LevelMap object is a collection of Layers and other information (to be
// determined later).
//

#if !defined __LOL_LEVELMAP_H__
#define __LOL_LEVELMAP_H__

namespace lol
{

class LevelMapData;

class LevelMap
{
public:
    LevelMap(char const *path);
    ~LevelMap();

    void Render(int x, int y, int z);
    int GetWidth();
    int GetHeight();

private:
    LevelMapData *data;
};

} /* namespace lol */

#endif // __LOL_MAP_H__

