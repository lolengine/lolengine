//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Video interface
// -------------------
// Helper GL functions to set up the scene.
//

#if !defined __LOL_VIDEO_H__
#define __LOL_VIDEO_H__

#include <stdint.h>

namespace lol
{

struct ClearMask
{
    enum Value
    {
        /* Note: D3D9 doesn't appear to support the accumulation buffer,
         * and it is a deprecated OpenGL feature. No reasone to support it. */
        Color   = 1 << 0,
        Depth   = 1 << 1,
        Stencil = 1 << 2,

        All     = 0xffffffff
    }
    m_value;

    inline ClearMask(Value v) { m_value = v; }
    inline ClearMask(uint64_t i) { m_value = (Value)i; }
    inline operator Value() { return m_value; }
};

class Video
{
public:
    static void Setup(ivec2 size);
    static void Destroy();
    static void SetFov(float theta);
    static void SetDepth(bool set);
    static void SetClearColor(vec4 color);
    static void SetClearDepth(float f);
    static void Clear(ClearMask m);
    static void Capture(uint32_t *buffer);
    static ivec2 GetSize();
};

} /* namespace lol */

#endif // __LOL_VIDEO_H__

