//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
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
    int dummy;
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
    f.y -= keystate[SDLK_e];
    f.y += keystate[SDLK_d];
    f.x -= keystate[SDLK_s];
    f.x += keystate[SDLK_f];
    if (keystate[SDLK_e] + keystate[SDLK_d]
         == keystate[SDLK_s] + keystate[SDLK_f])
    {
        f.x *= invsqrt2;
        f.y *= invsqrt2;
    }

    return f;
}

