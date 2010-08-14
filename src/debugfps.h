//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The DebugFps class
// ------------------
//

#if !defined __DH_DEBUGFPS_H__
#define __DH_DEBUGFPS_H__

#include "asset.h"

class DebugFpsData;

class DebugFps : public Asset
{
public:
    DebugFps();
    virtual ~DebugFps();

protected:
    virtual Group GetGroup();
    virtual void TickRender(float delta_time);

private:
    DebugFpsData *data;
};

#endif // __DH_DEBUGFPS_H__

