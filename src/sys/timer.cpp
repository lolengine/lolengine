//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <stdint.h>

#if __linux__ || __native_client__ || __APPLE__ \
 || (HAVE_GETTIMEOFDAY && HAVE_USLEEP && HAVE_SYS_TIME_H && HAVE_UNISTD_H)
#   include <sys/time.h>
#   include <unistd.h>
#elif _XBOX
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#   include <xbox.h>
#elif _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#elif __CELLOS_LV2__
#   include <sys/sys_time.h>
#   include <sys/timer.h>
#   include <sys/time_util.h>
#elif HAVE_SDL_SDL_H
#   include <SDL/SDL.h>
#else
#   include <SDL.h>
#endif

#include <lol/main.h>

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
        Init();
        (void)GetSeconds(true);
    }

#if __linux__ || __native_client__ || __APPLE__ \
 || (HAVE_GETTIMEOFDAY && HAVE_USLEEP)
    inline void Init()
    {
        m_tv.tv_usec = 0;
        m_tv.tv_sec = 0;
    }

    float GetSeconds(bool reset)
    {
        struct timeval tv, tv0 = m_tv;
        gettimeofday(&tv, nullptr);
        if (reset)
            m_tv = tv;
        return 1e-6f * (tv.tv_usec - tv0.tv_usec) + (tv.tv_sec - tv0.tv_sec);
    }

    static void WaitSeconds(float seconds)
    {
        if (seconds > 0.0f)
            usleep((int)(seconds * 1e6f));
    }

    struct timeval m_tv;

#elif _WIN32
    inline void Init()
    {
        m_cycles.QuadPart = 0;
    }

    float GetSeconds(bool reset)
    {
        static float secs_per_cycle = GetSecondsPerCycle();
        LARGE_INTEGER cycles, cycles0 = m_cycles;
        QueryPerformanceCounter(&cycles);
        if (reset)
            m_cycles = cycles;
        return secs_per_cycle * (cycles.QuadPart - cycles0.QuadPart);
    }

    static void WaitSeconds(float seconds)
    {
        if (seconds > 5e-4f)
            Sleep((int)(seconds * 1e3f + 0.5f));
    }

    static float GetSecondsPerCycle()
    {
        LARGE_INTEGER tmp;
        QueryPerformanceFrequency(&tmp);
        return 1.f / tmp.QuadPart;
    }

    LARGE_INTEGER m_cycles;

#elif __CELLOS_LV2__
    inline void Init()
    {
        m_cycles = 0;
    }

    float GetSeconds(bool reset)
    {
        static float secs_per_cycle = GetSecondsPerCycle();
        uint64_t cycles, cycles0 = m_cycles;
        SYS_TIMEBASE_GET(cycles);
        if (reset)
            m_cycles = cycles;
        return secs_per_cycle * (cycles - cycles0);
    }

    static void WaitSeconds(float seconds)
    {
        if (seconds > 0.0f)
            sys_timer_usleep((int)(seconds * 1e6f));
    }

    static float GetSecondsPerCycle()
    {
        return 1.f / sys_time_get_timebase_frequency();
    }

    uint64_t m_cycles;

#else
    inline void Init()
    {
        m_ticks = 0;
    }

    float GetSeconds(bool reset)
    {
        static bool initialised = Init();

        /* The crappy SDL fallback */
        Uint32 ticks = SDL_GetTicks(), ticks0 = m_ticks;
        if (reset)
            m_ticks = ticks;
        return 1e-3f * (ticks - ticks0);
    }

    static void WaitSeconds(float seconds)
    {
        if (seconds > 5e-4f)
            SDL_Delay((int)(seconds * 1e3f + 0.5f));
    }

    static bool Init()
    {
        SDL_Init(SDL_INIT_TIMER);
        return true;
    }

    Uint32 m_ticks;
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
    return data->GetSeconds(true);
}

float Timer::Poll()
{
    return data->GetSeconds(false);
}

void Timer::Wait(float seconds)
{
    float secs_elapsed = data->GetSeconds(false);
    data->WaitSeconds(seconds - secs_elapsed);
}

} /* namespace lol */

