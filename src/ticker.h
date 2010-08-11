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

    static void TickGame(float delta_time);
    static void TickRender(float delta_time);
};

#endif // __DH_TICKER_H__

