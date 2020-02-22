//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
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
// The timer class
// ---------------
//

namespace lol
{

class timer
{
public:
    inline timer() { (void)get_seconds(true); }

    inline void reset() { (void)get_seconds(true); }
    inline float get() { return get_seconds(true); }
    inline float poll() { return get_seconds(false); }

    void wait(float seconds)
    {
        if (seconds > 0.0f)
        {
            float secs_elapsed = get_seconds(false);
            std::this_thread::sleep_for(std::chrono::duration<float>(seconds - secs_elapsed));
        }
    }

private:
    std::chrono::steady_clock::time_point m_tp;

    float get_seconds(bool do_reset)
    {
        auto tp = std::chrono::steady_clock::now(), tp0 = m_tp;

        if (do_reset)
            m_tp = tp;

        return std::chrono::duration_cast<std::chrono::duration<float>>(tp - tp0).count();
    }
};

} /* namespace lol */

