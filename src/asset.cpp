
#include <cstdlib>

#include "asset.h"
#include "ticker.h"

/*
 * Public Asset class
 */

Asset::Asset() :
    next(0),
    ref(0),
    destroy(0)
{
    Ticker::Register(this);
}

Asset::~Asset()
{
}

void Asset::TickGame(float delta_time)
{

}

void Asset::TickRender(float delta_time)
{

}

void Asset::Ref()
{
    ref++;
}

int Asset::Unref()
{
    return --ref;
}

