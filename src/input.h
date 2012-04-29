//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Input static class
// ----------------------
//

#if !defined __LOL_INPUT_H__
#define __LOL_INPUT_H__

#include "lol/math/vector.h"

namespace lol
{

class WorldEntity;

class Input
{
public:
    /* These methods are general queries */
    static vec2 GetAxis(int axis);
    static ivec2 GetMousePos();
    static ivec3 GetMouseButtons();
    //BH : Added this, is a v0.1 Alpha version.
    static int GetButtonState(int button);

    /* Entities can subscribe to events */
    static void TrackMouse(WorldEntity *e);
    static void UntrackMouse(WorldEntity *e);

    /* These methods are called by the underlying input listeners */
    static void SetMousePos(ivec2 coord);
    static void SetMouseButton(int index);
    static void UnsetMouseButton(int index);
};

} /* namespace lol */

#endif // __LOL_INPUT_H__

