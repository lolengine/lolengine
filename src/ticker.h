//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Ticker class
// ----------------
// The Ticker is a static class that registers entities and ticks them.
//

#if !defined __LOL_TICKER_H__
#define __LOL_TICKER_H__

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
    static void TickGame();
    static void TickDraw();
    static void ClampFps();
    static void StartBenchmark();
    static void StopBenchmark();
    static void StartRecording();
    static void StopRecording();
    static int GetFrameNum();

    static void Shutdown();
    static int Finished();
};

} /* namespace lol */

#endif // __LOL_TICKER_H__

