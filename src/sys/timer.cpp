//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdlib>
#include <stdint.h>

#include <chrono>

using namespace std::chrono;

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
        (void)GetSeconds(true);
    }

    float GetSeconds(bool reset)
    {
        steady_clock::time_point tp, tp0 = m_tp;
        tp = steady_clock::now();
        if (reset)
            m_tp = tp;
        return duration_cast<duration<float>>(tp - tp0).count();
    }

    static void WaitSeconds(float seconds)
    {
        if (seconds > 0.0f)
            std::this_thread::sleep_for(std::chrono::duration<float>(seconds));
    }

    steady_clock::time_point m_tp;
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

void Timer::Reset()
{
    (void)data->GetSeconds(true);
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

