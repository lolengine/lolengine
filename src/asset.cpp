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
#if !FINAL_RELEASE
    state = STATE_IDLE;
#endif
    Ticker::Register(this);
}

Asset::~Asset()
{
#if !FINAL_RELEASE
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
#if !FINAL_RELEASE
    if (state != STATE_PRETICK_GAME)
        fprintf(stderr, "ERROR: invalid asset game tick\n");
    state = STATE_POSTTICK_GAME;
#endif
}

void Asset::TickRender(float delta_time)
{
#if !FINAL_RELEASE
    if (state != STATE_PRETICK_RENDER)
        fprintf(stderr, "ERROR: invalid asset render tick\n");
    state = STATE_POSTTICK_RENDER;
#endif
}

void Asset::Ref()
{
    ref++;
}

int Asset::Unref()
{
    return --ref;
}

