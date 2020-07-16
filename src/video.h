//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The video interface
// ———————————————————
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

class video
{
public:
    static void init(ivec2 size);
    static void release();

    static void resize(ivec2 size);
    static ivec2 size();

    static void SetDebugRenderMode(DebugRenderMode d);
    static DebugRenderMode GetDebugRenderMode();
    static void capture(uint32_t *buffer);
};

} // namespace lol
