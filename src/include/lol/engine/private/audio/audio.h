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

#include <functional>

namespace lol::audio
{

enum class format : uint8_t
{
    unknown = 0,
    // 8-bit, unsigned or signed
    uint8, sint8,
    // 16-bit, unsigned or signed, little endian or big endian
    uint16le, uint16be,
    sint16le, sint16be,
    // 24-bit signed, little endian
    sint24le,
    // 32-bit signed, little endian or big endian
    sint32le, sint32be,
    // 32-bit floats, little endian or big endian
    float32le, float32be,
};

void init();
void shutdown();

int start(std::function<void(void *, size_t)> f,
          format format, int frequency, int channels);
void stop(int stream);

} // namespace lol
