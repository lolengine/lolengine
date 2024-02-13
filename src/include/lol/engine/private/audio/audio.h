//
//  Lol Engine
//
//  Copyright © 2010–2024 Sam Hocevar <sam@hocevar.net>
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
// ———————————————————
// Helper functions to set up the audio device.
//

#include <functional>
#include <lol/audio/stream>
#include <unordered_map>

namespace lol::audio
{

void init();
void shutdown();

template<typename S, typename T = S::sample_type>
inline int start_stream(std::shared_ptr<S> s0)
{
    std::shared_ptr<stream<float>> s = make_adapter<float>(s0, 2, 48000);
    return start_stream(s);
}

template<>
int start_stream(std::shared_ptr<stream<float>> stream);

void stop_stream(int id);

} // namespace lol
