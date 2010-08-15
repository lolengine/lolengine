//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Ticker class
// ----------------
// The Ticker is a static class that registers assets and ticks them.
//

#if !defined __DH_TICKER_H__
#define __DH_TICKER_H__

#include <stdint.h>

#include "asset.h"

class Ticker
{
public:
    static void Register(Asset *asset);

    static void TickGame();
    static void TickRender();
    static void ClampFps(float fps);
};

#endif // __DH_TICKER_H__

