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

//
// The Timer class
// ---------------
//

namespace lol
{

class TimerData;

class Timer
{
public:
    Timer();
    ~Timer();

    float Get();
    float Poll();
    void Wait(float seconds);

private:
    TimerData *data;

    /* Copying timers is forbidden for now. */
    Timer(Timer const &t);
    Timer operator =(Timer const &t);
};

} /* namespace lol */

