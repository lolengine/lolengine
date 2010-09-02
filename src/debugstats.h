//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The DebugStats class
// --------------------
//

#if !defined __DH_DEBUGSTATS_H__
#define __DH_DEBUGSTATS_H__

#include "entity.h"

class DebugStatsData;

class DebugStats : public Entity
{
public:
    DebugStats(char const *path);
    virtual ~DebugStats();

protected:
    virtual Group GetGroup();
    virtual void TickGame(float deltams);

private:
    DebugStatsData *data;
};

#endif // __DH_DEBUGSTATS_H__

