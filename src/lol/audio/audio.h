//
//  Lol Engine
//
//  Copyright © 2010—2016 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The audio interface
// -------------------
// Helper functions to set up the audio device.
//

#include <stdint.h>

namespace lol
{

class audio
{
public:
    static void init();

    static void set_channels(int channels);
    static void set_volume(int channel,int volume);
    static void mute_all();
    static void unmute_all();

private:
    audio() {}
};

} /* namespace lol */

