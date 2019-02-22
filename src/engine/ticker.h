//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The ticker class
// ————————————————
// The ticker is a static class that registers entities and ticks them.
//

#include <cstdint>

#include "engine/entity.h"

namespace lol
{

class ticker
{
public:
    static void Register(Entity *entity);
    static void Ref(Entity *entity);
    static int Unref(Entity *entity);

    static void Setup(float fps);
    static void tick_draw();
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
    ticker() {}
};

typedef ticker Ticker;

} /* namespace lol */

