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
// The DebugFps class
// ------------------
//

#if !defined __LOL_DEBUGFPS_H__
#define __LOL_DEBUGFPS_H__

#include "entity.h"

namespace lol
{

class DebugFpsData;

class DebugFps : public Entity
{
public:
    DebugFps(int x, int y);
    virtual ~DebugFps();

protected:
    virtual void TickGame(float seconds);

private:
    DebugFpsData *data;
};

} /* namespace lol */

#endif // __LOL_DEBUGFPS_H__

