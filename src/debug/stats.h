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
// The DebugStats class
// --------------------
//

#include "engine/entity.h"

namespace lol
{

class DebugStatsData;

class DebugStats : public Entity
{
public:
    DebugStats(char const *path);
    virtual ~DebugStats();

protected:
    virtual void TickGame(float seconds);

private:
    DebugStatsData *data;
};

} /* namespace lol */

