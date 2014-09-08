//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Ticker class
// ----------------
// The Ticker is a static class that registers entities and ticks them.
//

#include <stdint.h>

#include "entity.h"

namespace lol
{

class Ticker
{
public:
    static void Register(Entity *entity);
    static void Ref(Entity *entity);
    static int Unref(Entity *entity);

    static void Setup(float fps);
    static void TickDraw();
    static void StartBenchmark();
    static void StopBenchmark();
    static void StartRecording();
    static void StopRecording();
    static int GetFrameNum();

    static void SetState(Entity *entity, uint32_t state);
    static void SetStateWhenMatch(Entity *entity, uint32_t state,
                                  Entity *other_entity, uint32_t other_state);

    static void Shutdown();
    static int Finished();

private:
    Ticker() {}
};

} /* namespace lol */

