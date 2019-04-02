//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
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

#include <unordered_set>
#include <functional>

namespace lol
{

class audio
{
public:
    enum class format : uint8_t
    {
        unknown = 0,
        uint8, sint8,
        uint16le, uint16be,
        sint16le, sint16be,
        sint32le, sint32be,
        float32le, float32be,
    };

    static void init();

    // Set the number of audio tracks that can be mixed together
    static void set_tracks(int tracks);
    // Set the volume of a specific track
    static void set_volume(int track, int volume);
    static void mute_all();
    static void unmute_all();

    static int start_streaming(std::function<void(void *, int)> const &f,
                               format format = audio::format::sint16le,
                               int frequency = 22050,
                               int channels = 2);

    static void stop_streaming(int track);

private:
    audio() {}
    static std::unordered_set<std::shared_ptr<struct audio_streamer>> m_streamers;
};

} /* namespace lol */

