//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The DebugRecord class
// ---------------------
//

#if !defined __DH_DEBUGRECORD_H__
#define __DH_DEBUGRECORD_H__

#include "entity.h"

class DebugRecordData;

class DebugRecord : public Entity
{
public:
    DebugRecord(char const *path, float fps);
    virtual ~DebugRecord();

protected:
    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);

private:
    DebugRecordData *data;
};

#endif // __DH_DEBUGRECORD_H__

