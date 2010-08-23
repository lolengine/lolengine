//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Ticker class
// ----------------
// The Ticker is a static class that registers entities and ticks them.
//

#if !defined __DH_TICKER_H__
#define __DH_TICKER_H__

#include <stdint.h>

#include "entity.h"

class Ticker
{
public:
    static void Register(Entity *entity);
    static void Ref(Entity *entity);
    static int Unref(Entity *entity);

    static void TickGame();
    static void TickDraw();
    static void ClampFps(float deltams);
    static int GetFrameNum();

    static void Shutdown();
    static int Finished();
};

#endif // __DH_TICKER_H__

