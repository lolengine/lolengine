//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Video interface
// -------------------
// Helper GL functions to set up the scene.
//

#include <stdint.h>

namespace lol
{

struct DebugRenderMode
{
    enum Value
    {
        //Add your new rendermode at your convenience
        Default,
        Flat,
        Wireframe,
        Lighting,
        Normal,
        UV,

        Max
    }
    m_value;

    inline DebugRenderMode() : m_value(Default) {}
    inline DebugRenderMode(Value v) : m_value(v) {}
    inline DebugRenderMode(int v) : m_value((Value)v) {}
    inline operator Value() { return m_value; }
};

class Video
{
public:
    static void Setup(ivec2 size);
    static void Destroy();

    static ivec2 GetSize();

    static void SetDebugRenderMode(DebugRenderMode d);
    static DebugRenderMode GetDebugRenderMode();
    static void Capture(uint32_t *buffer);
};

} /* namespace lol */

