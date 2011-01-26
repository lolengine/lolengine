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
// The Input static class
// ----------------------
//

#if !defined __DH_INPUT_H__
#define __DH_INPUT_H__

#include "matrix.h"

class Input
{
public:
    static float2 GetAxis(int axis);
    static void SetMousePos(int2 coord);
    static int2 GetMousePos();
    static void SetMouseButton(int index);
    static void UnsetMouseButton(int index);
    static int3 GetMouseButtons();
};

#endif // __DH_INPUT_H__

