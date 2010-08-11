
/*
 * The ticker
 */

#include <stdint.h>
#include <stdlib.h>

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
        assets(0),
        nassets(0)
    {
        /* Nothing to do */
    }

    ~TickerData()
    {
        free(assets);
    }

private:
    Asset **assets;
    int nassets;
}
tickerdata;

static TickerData * const data = &tickerdata;

/*
 * Ticker public class
 */

void Ticker::Register(Asset *asset)
{
    int tmp = data->nassets++;
    data->assets = (Asset **)realloc(data->assets,
                                     data->nassets * sizeof(Asset *));
    data->assets[tmp] = asset;

    asset->index = tmp;
}

void Ticker::TickGame(float delta_time)
{
    for (int i = 0; i < data->nassets; i++)
        data->assets[i]->TickGame(delta_time);
}

void Ticker::TickRender(float delta_time)
{
    for (int i = 0; i < data->nassets; i++)
        data->assets[i]->TickRender(delta_time);
}

