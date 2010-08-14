//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <cstdio>

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
#if !defined FINAL_RELEASE
    if (!destroy)
        fprintf(stderr, "ERROR: asset destructor called directly\n");
#endif
}

Asset::Group Asset::GetGroup()
{
    return GROUP_DEFAULT;
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

