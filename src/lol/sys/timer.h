//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

#include <chrono>
#include <thread>

//
// The Timer class
// ---------------
//

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
        std::chrono::steady_clock::time_point tp, tp0 = m_tp;
        tp = std::chrono::steady_clock::now();
        if (reset)
            m_tp = tp;
        return std::chrono::duration_cast<std::chrono::duration<float>>(tp - tp0).count();
    }

    static void WaitSeconds(float seconds)
    {
        if (seconds > 0.0f)
            std::this_thread::sleep_for(std::chrono::duration<float>(seconds));
    }

    std::chrono::steady_clock::time_point m_tp;
};

class Timer
{
public:
    Timer()
      : data(new TimerData())
    {
    }

    ~Timer()
    {
        delete data;
    }

    void Reset()
    {
        (void)data->GetSeconds(true);
    }

    float Get()
    {
        return data->GetSeconds(true);
    }

    float Poll()
    {
        return data->GetSeconds(false);
    }

    void Wait(float seconds)
    {
        float secs_elapsed = data->GetSeconds(false);
        data->WaitSeconds(seconds - secs_elapsed);
    }

private:
    TimerData *data;

    /* Copying timers is forbidden for now. */
    Timer(Timer const &t);
    Timer operator =(Timer const &t);
};

} /* namespace lol */

