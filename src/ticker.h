
/*
 * The tick manager
 */

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

