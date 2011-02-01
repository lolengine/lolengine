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
        buttons(0, 0, 0),
        nentities(0)
    { }

private:
    int2 mouse;
    int3 buttons;

    static int const MAX_ENTITIES = 100;
    WorldEntity *entities[MAX_ENTITIES];
    int nentities;
}
inputdata;

static InputData * const data = &inputdata;

/*
 * Public Input class
 */

float2 Input::GetAxis(int axis)
{
    float invsqrt2 = sqrtf(0.5f);
    float2 f;

    /* Simulate a joystick using the keyboard. This SDL call is free. */
    Uint8 *keystate = SDL_GetKeyState(NULL);
    int left = keystate[SDLK_d] - (keystate[SDLK_a] | keystate[SDLK_q]);
    int up = (keystate[SDLK_w] | keystate[SDLK_z]) - keystate[SDLK_s] ;
    f.x += left;
    f.y += up;
    if (left && up)
        f = f * invsqrt2;

    return f;
}

int2 Input::GetMousePos()
{
    return data->mouse;
}

int3 Input::GetMouseButtons()
{
    return data->buttons;
}

void Input::TrackMouse(WorldEntity *e)
{
    if (data->nentities >= InputData::MAX_ENTITIES)
        return;
    data->entities[data->nentities] = e;
    data->nentities++;
}

void Input::UntrackMouse(WorldEntity *e)
{
    for (int n = 0; n < data->nentities; n++)
    {
        if (data->entities[n] != e)
            continue;

        data->entities[n] = data->entities[data->nentities - 1];
        data->nentities--;
        n--;
    }
}

void Input::SetMousePos(int2 coord)
{
    data->mouse = coord;

    WorldEntity *best = NULL;

    for (int n = 0; n < data->nentities; n++)
    {
        if (coord.x < data->entities[n]->bbox[0].x
             || coord.x >= data->entities[n]->bbox[1].x
             || coord.y < data->entities[n]->bbox[0].y
             || coord.y >= data->entities[n]->bbox[1].y)
            continue;

        if (!best || best->bbox[1].z < data->entities[n]->bbox[1].z)
            best = data->entities[n];
    }

    for (int n = 0; n < data->nentities; n++)
    {
        if (data->entities[n] == best)
            data->entities[n]->mousepos = (int2)((int3)coord - best->bbox[0]);
        else
            data->entities[n]->mousepos = int2(-1);
    }
}

void Input::SetMouseButton(int index)
{
    data->buttons[index] = 1;

    /* FIXME: parse all subscribed entities and update them */
}

void Input::UnsetMouseButton(int index)
{
    data->buttons[index] = 0;

    /* FIXME: parse all subscribed entities and update them */
}

