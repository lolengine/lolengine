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
// The World class
// ---------------
//

namespace lol
{

class World : public Lolua::Loader
{
public:
    World();
    virtual ~World();
};

extern World g_world;

} /* namespace lol */

