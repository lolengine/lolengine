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

class Timer
{
private:

    float GetSeconds(bool reset)
    {
        std::chrono::steady_clock::time_point
            tp = std::chrono::steady_clock::now(),
            tp0 = m_tp;

        if (reset)
            m_tp = tp;

        return std::chrono::duration_cast<std::chrono::duration<float>>(tp - tp0).count();
    }

public:
    Timer()
        : m_tp()
    {
        (void)GetSeconds(true);
    }

    ~Timer()
    {
    }

    void Reset()
    {
        (void)GetSeconds(true);
    }

    float Get()
    {
        return GetSeconds(true);
    }

    float Poll()
    {
        return GetSeconds(false);
    }

    void Wait(float seconds)
    {
        if (seconds > 0.0f)
        {
            float secs_elapsed = GetSeconds(false);
            std::this_thread::sleep_for(std::chrono::duration<float>(seconds - secs_elapsed));
        }
    }

private:
    std::chrono::steady_clock::time_point m_tp;
};

} /* namespace lol */

