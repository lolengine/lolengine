//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <stdint.h>

#include "core.h"

namespace lol
{

/*
 * Emcee implementation class
 */

static Thread *GameThread, *DrawThread;

static Queue gametick, drawtick;

static void *GameThreadMain(void *data)
{
    while (!Ticker::Finished())
    for (;;)
    {
        int tick = gametick.Pop();
        if (!tick)
            break;

        Ticker::TickGame();

        drawtick.Push(1);
    }

    drawtick.Push(0);
}

static void *DrawThreadMain(void *data)
{
    for (;;)
    {
        int tick = drawtick.Pop();
        if (!tick)
            break;

        Ticker::TickDraw();
    }
}

void Emcee::Setup()
{
    GameThread = new Thread(GameThreadMain, NULL);
    DrawThread = new Thread(DrawThreadMain, NULL);
}

void Emcee::Shutdown()
{
}

void Emcee::SetState(Entity *entity, uint32_t state)
{

}

void Emcee::SetStateWhenMatch(Entity *entity, uint32_t state,
                              Entity *other_entity, uint32_t other_state)
{

}

} /* namespace lol */

