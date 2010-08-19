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

#include "asset.h"

class DebugRecordData;

class DebugRecord : public Asset
{
public:
    DebugRecord(char const *path);
    virtual ~DebugRecord();

protected:
    virtual Group GetGroup();
    virtual void TickGame(float delta_time);
    virtual void TickRender(float delta_time);

private:
    DebugRecordData *data;
};

#endif // __DH_DEBUGRECORD_H__

