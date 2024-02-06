//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
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

#include <lol/engine/tickable.h>

#include <stdint.h> // uint32_t
#include <memory>   // std::shared_ptr

namespace lol
{

class ticker
{
public:
    static void setup(float fps);
    static void tick_draw();
    static void teardown();

    static void add(std::shared_ptr<tickable> entity);
    static void remove(std::shared_ptr<tickable> entity);

    // The old API
    static void Register(class entity *entity);
    static void Ref(class entity *entity);
    static int Unref(class entity *entity);

    static void StartBenchmark();
    static void StopBenchmark();
    static void StartRecording();
    static void StopRecording();
    static int GetFrameNum();

    static void SetState(class entity *entity, uint32_t state);
    static void SetStateWhenMatch(class entity *entity, uint32_t state,
                                  class entity *other_entity, uint32_t other_state);

    static void Shutdown();
    static int Finished();

private:
    ticker() {}
};

// The old API
typedef ticker Ticker;

} /* namespace lol */

