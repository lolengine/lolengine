//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The DebugRecord class
// ---------------------
//

#if !defined __LOL_DEBUGRECORD_H__
#define __LOL_DEBUGRECORD_H__

#include "entity.h"

namespace lol
{

class DebugRecordData;

class DebugRecord : public Entity
{
public:
    DebugRecord(char const *path, float fps);
    virtual ~DebugRecord();

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

private:
    DebugRecordData *data;
};

} /* namespace lol */

#endif // __LOL_DEBUGRECORD_H__

