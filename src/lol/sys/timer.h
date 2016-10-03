//
//  Lol Engine
//
//  Copyright © 2010—2016 Sam Hocevar <sam@hocevar.net>
//            © 2016 Guillaume Bittoun <guillaume.bittoun@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
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

class Timer
{
public:
    Timer()
    {
        (void)get_seconds(true);
    }

    void Reset()
    {
        (void)get_seconds(true);
    }

    float Get()
    {
        return get_seconds(true);
    }

    float Poll()
    {
        return get_seconds(false);
    }

    void Wait(float seconds)
    {
        if (seconds > 0.0f)
        {
            float secs_elapsed = get_seconds(false);
            std::this_thread::sleep_for(std::chrono::duration<float>(seconds - secs_elapsed));
        }
    }

private:
    std::chrono::steady_clock::time_point m_tp;

    float get_seconds(bool reset)
    {
        auto tp = std::chrono::steady_clock::now(), tp0 = m_tp;

        if (reset)
            m_tp = tp;

        return std::chrono::duration_cast<std::chrono::duration<float>>(tp - tp0).count();
    }
};

} /* namespace lol */

