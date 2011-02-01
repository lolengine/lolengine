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

class WorldEntity;

class Input
{
public:
    /* These methods are general queries */
    static float2 GetAxis(int axis);
    static int2 GetMousePos();
    static int3 GetMouseButtons();

    /* Entities can subscribe to events */
    static void ListenMouse(WorldEntity *e);
    static void UnlistenMouse(WorldEntity *e);

    /* These methods are called by the underlying input listeners */
    static void SetMousePos(int2 coord);
    static void SetMouseButton(int index);
    static void UnsetMouseButton(int index);
};

#endif // __DH_INPUT_H__

