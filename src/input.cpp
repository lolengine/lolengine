//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <SDL.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "core.h"

/*
 * Input implementation class
 */

static class InputData
{
    friend class Input;

public:
    InputData()
      : mouse(-1, -1),
        buttons(0, 0, 0)
    { }

    Int2 mouse;
    Int3 buttons;
}
inputdata;

static InputData * const data = &inputdata;

/*
 * Public Input class
 */

Float2 Input::GetAxis(int axis)
{
    float invsqrt2 = sqrtf(0.5f);
    Float2 f;

    /* Simulate a joystick using the keyboard. This SDL call is free. */
    Uint8 *keystate = SDL_GetKeyState(NULL);
    int left = keystate[SDLK_d] - (keystate[SDLK_a] | keystate[SDLK_q]);
    int up = (keystate[SDLK_w] | keystate[SDLK_z]) - keystate[SDLK_s] ;
    f.x += left;
    f.y += up;
    if (left && up)
    {
        f.x *= invsqrt2;
        f.y *= invsqrt2;
    }

    return f;
}

void Input::SetMousePos(Int2 coord)
{
    data->mouse = coord;
}

Int2 Input::GetMousePos()
{
    return data->mouse;
}

void Input::SetMouseButton(int index)
{
    data->buttons[index] = 1;
}

void Input::UnsetMouseButton(int index)
{
    data->buttons[index] = 0;
}

Int3 Input::GetMouseButtons()
{
    return data->buttons;
}

