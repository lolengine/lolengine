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

#if !defined __DH_VIDEO_H__
#define __DH_VIDEO_H__

#include <stdint.h>

namespace lol
{

class Video
{
public:
    static void Setup(int width, int height);
    static void Destroy();
    static void SetFov(float theta);
    static void SetDepth(bool set);
    static void Clear();
    static void Capture(uint32_t *buffer);
    static int GetWidth();
    static int GetHeight();
};

} /* namespace lol */

#endif // __DH_VIDEO_H__

