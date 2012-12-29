//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <stdint.h>

#if defined __linux__ || defined __native_client__ || defined __APPLE__
#   include <sys/time.h>
#   include <unistd.h>
#elif defined _XBOX
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#   include <xbox.h>
#elif defined _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#elif defined __CELLOS_LV2__
#   include <sys/sys_time.h>
#   include <sys/timer.h>
#   include <sys/time_util.h>
#elif defined HAVE_SDL_SDL_H
#   include <SDL/SDL.h>
#else
#   include <SDL.h>
#endif

#include "core.h"

namespace lol
{

/*
 * Timer implementation class
 */

class TimerData
{
    friend class Timer;

private:
    TimerData()
    {
#if defined __linux__ || defined __native_client__ || defined __APPLE__
        gettimeofday(&tv0, NULL);
#elif defined _WIN32
        QueryPerformanceCounter(&cycles0);
#elif defined __CELLOS_LV2__
        SYS_TIMEBASE_GET(cycles0);
#else
        SDL_Init(SDL_INIT_TIMER);
        ticks0 = SDL_GetTicks();
#endif
    }

    float GetOrWait(float seconds, bool update)
    {
        float secs_elapsed, secs_towait;
#if defined __linux__ || defined __native_client__ || defined __APPLE__
        struct timeval tv;
        gettimeofday(&tv, NULL);
        secs_elapsed = 1e-6f * (tv.tv_usec - tv0.tv_usec)
                     + (tv.tv_sec - tv0.tv_sec);
        if (update)
            tv0 = tv;
        secs_towait = seconds - secs_elapsed;
        if (secs_towait > 0.0f)
            usleep((int)(secs_towait * 1e6f));
#elif defined _WIN32
        LARGE_INTEGER cycles;
        QueryPerformanceCounter(&cycles);
        static float secs_per_cycle = GetSecondsPerCycle();
        secs_elapsed = secs_per_cycle * (cycles.QuadPart - cycles0.QuadPart);
        if (update)
            cycles0 = cycles;
        secs_towait = seconds - secs_elapsed;
        if (secs_towait > 5e-4f)
            Sleep((int)(secs_towait * 1e3f + 0.5f));
#elif defined __CELLOS_LV2__
        uint64_t cycles;
        SYS_TIMEBASE_GET(cycles);
        static float secs_per_cycle = GetSecondsPerCycle();
        secs_elapsed = secs_per_cycle * (cycles - cycles0);
        if (update)
            cycles0 = cycles;
        secs_towait = seconds - secs_elapsed;
        if (secs_towait > 0.0f)
            sys_timer_usleep((int)(secs_towait * 1e6f));
#else
        /* The crappy SDL fallback */
        Uint32 ticks = SDL_GetTicks();
        secs_elapsed = 1e-3f * (ticks - ticks0);
        if (update)
            ticks0 = ticks;
        secs_towait = seconds - secs_elapsed;
        if (secs_towait > 5e-4f)
            SDL_Delay((int)(secs_towait * 1e3f + 0.5f));
#endif
        return secs_elapsed;
    }

    static float GetSecondsPerCycle()
    {
#if defined __linux__ || defined __native_client__ || defined __APPLE__
        return 1e-3f;
#elif defined _WIN32
        LARGE_INTEGER tmp;
        QueryPerformanceFrequency(&tmp);
        return 1.f / tmp.QuadPart;
#elif defined __CELLOS_LV2__
        return 1.f / sys_time_get_timebase_frequency();
#else
        return 1e-3f;
#endif
    }

#if defined __linux__ || defined __native_client__ || defined __APPLE__
    struct timeval tv0;
#elif defined _WIN32
    LARGE_INTEGER cycles0;
#elif defined __CELLOS_LV2__
    uint64_t cycles0;
#else
    Uint32 ticks0;
#endif
};

/*
 * Timer public class
 */

Timer::Timer()
  : data(new TimerData())
{
}

Timer::~Timer()
{
    delete data;
}

float Timer::Get()
{
    return data->GetOrWait(0.0f, true);
}

float Timer::Poll()
{
    return data->GetOrWait(0.0f, false);
}

void Timer::Wait(float seconds)
{
    (void)data->GetOrWait(seconds, false);
}

} /* namespace lol */

