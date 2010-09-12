//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
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
    DebugRecord(char const *path);
    virtual ~DebugRecord();

protected:
    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);

private:
    DebugRecordData *data;
};

#endif // __DH_DEBUGRECORD_H__

