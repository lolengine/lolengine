//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The World class
// ---------------
//

#if !defined __LOL_WORLD_H__
#define __LOL_WORLD_H__

namespace lol
{

class World
{
public:
    bool ExecLua(String const &lua);
    double GetLuaNumber(String const &var);

//private:
    World();
    ~World();
};

extern World g_world;

} /* namespace lol */

#endif // __LOL_WORLD_H__

