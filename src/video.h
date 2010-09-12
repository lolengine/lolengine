//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Video interface
// -------------------
// Helper GL functions to set up the scene.
//

#if !defined __DH_VIDEO_H__
#define __DH_VIDEO_H__

#include <stdint.h>

class Video
{
public:
    static void Setup(int width, int height);
    static void SetFov(float theta);
    static void Clear();
    static void Capture(uint32_t *buffer);
    static int GetWidth();
    static int GetHeight();
};

#endif // __DH_VIDEO_H__

