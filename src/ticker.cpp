
/*
 * The ticker
 */

#include <cstdlib>
#include <cstdio>
#include <stdint.h>

#include "ticker.h"
#include "asset.h"

/*
 * Ticker implementation class
 */

static class TickerData
{
    friend class Ticker;

public:
    TickerData() :
        list(0),
        nassets(0)
    {
    }

    ~TickerData()
    {
        if (nassets)
            fprintf(stderr, "ERROR: still %i assets in ticker\n", nassets);
    }

private:
    Asset *list;
    int nassets;
}
tickerdata;

static TickerData * const data = &tickerdata;

/*
 * Ticker public class
 */

void Ticker::Register(Asset *asset)
{
    asset->next = data->list;
    data->list = asset;
    data->nassets++;
}

void Ticker::TickGame(float delta_time)
{
    /* Garbage collect objects that can be destroyed */
    for (Asset *asset = data->list, *prev = NULL;
         asset;
         prev = asset, asset = asset->next)
        if (asset->destroy)
        {
            if (prev)
                prev->next = asset->next;
            else
                data->list = asset->next;

            data->nassets--;
            delete asset;
        }

    /* Tick objects for the game loop */
    for (Asset *asset = data->list; asset; asset = asset->next)
        asset->TickGame(delta_time);
}

void Ticker::TickRender(float delta_time)
{
    /* Tick objects for the render loop */
    for (Asset *asset = data->list; asset; asset = asset->next)
        asset->TickRender(delta_time);
}

