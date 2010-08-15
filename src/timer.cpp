//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <cstdio>
#include <stdint.h>

#if defined __linux__
#   include <sys/time.h>
#   include <unistd.h>
#elif defined _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   include <SDL.h> // FIXME: this should not be needed
#else
#   include <SDL.h>
#endif

#include "timer.h"

/*
 * Timer implementation class
 */

class TimerData
{
    friend class Timer;

private:
    TimerData()
    {
#if defined __linux__
        gettimeofday(&tv0, NULL);
#elif defined _WIN32
        LARGE_INTEGER tmp;
        QueryPerformanceFrequency(&tmp);
        seconds_per_cycle = 1.0f / tmp.QuadPart;
        QueryPerformanceCounter(&cycles0);
#else
        SDL_Init(SDL_INIT_TIMER);
        ticks0 = SDL_GetTicks();
#endif
    }

    float GetSeconds(bool update)
    {
        float ret;
#if defined __linux__
        struct timeval tv;
        gettimeofday(&tv, NULL);
        ret = 1e-6f * (tv.tv_usec - tv0.tv_usec) + (tv.tv_sec - tv0.tv_sec);
        if (update)
            tv0 = tv;
#elif defined _WIN32
        LARGE_INTEGER cycles;
        QueryPerformanceCounter(&cycles);
        ret = seconds_per_cycle * (cycles.QuadPart - cycles0.QuadPart);
        if (update)
            cycles0 = cycles;
#else
        Uint32 ticks = SDL_GetTicks();
        ret = 1e-3f * (ticks - ticks0);
        if (update)
            ticks0 = ticks;
#endif
        return ret;
    }

    void WaitSeconds(float seconds)
    {
#if defined __linux__
        usleep((int)(seconds * 1000000.0f));
#elif defined _WIN32
        /* FIXME: use native Win32 stuff */
        SDL_Delay((int)(seconds * 1000.0f + 0.5f));
#else
        SDL_Delay((int)(seconds * 1000.0f + 0.5f));
#endif
    }

#if defined __linux__
    struct timeval tv0;
#elif defined _WIN32
    float seconds_per_cycle;
    LARGE_INTEGER cycles0;
#else
    Uint32 ticks0;
#endif
};

/*
 * Timer public class
 */

Timer::Timer()
{
    data = new TimerData();
}

Timer::~Timer()
{
    delete data;
}

float Timer::GetSeconds()
{
    return data->GetSeconds(true);
}

void Timer::WaitSeconds(float seconds)
{
    float sleep = seconds - data->GetSeconds(false);
    if (sleep > 1e-4f)
        data->WaitSeconds(sleep);
}

