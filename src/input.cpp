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

#include <cstdio>
#include <cstdlib>
#include <cmath>

#if defined USE_SDL
#   include <SDL.h>
#endif

#include "core.h"

namespace lol
{

/*
 * Input implementation class
 */

static class InputData
{
    friend class Input;

public:
    InputData()
      : mouse(-1),
        buttons(0),
        nentities(0),
        lastfocus(0)
    { }

private:
    vec2i mouse;
    vec3i buttons;

    static int const MAX_ENTITIES = 100;
    WorldEntity *entities[MAX_ENTITIES];
    int nentities;
    WorldEntity *lastfocus;
}
inputdata;

static InputData * const data = &inputdata;

/*
 * Public Input class
 */

vec2 Input::GetAxis(int axis)
{
    vec2 ret;

#if defined USE_SDL
    /* Simulate a joystick using the keyboard. This SDL call is free. */
    Uint8 *keystate = SDL_GetKeyState(NULL);
    int left = keystate[SDLK_d] - (keystate[SDLK_a] | keystate[SDLK_q]);
    int up = (keystate[SDLK_w] | keystate[SDLK_z]) - keystate[SDLK_s] ;
    ret.x += left;
    ret.y += up;
    if (left && up)
        ret = ret * sqrtf(0.5f);
#else
    ret = 0;
#endif

    return ret;
}

vec2i Input::GetMousePos()
{
    return data->mouse;
}

vec3i Input::GetMouseButtons()
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

void Input::SetMousePos(vec2i coord)
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
        {
            data->entities[n]->mousepos = (vec2i)((vec3i)coord - best->bbox[0]);
            if (best != data->lastfocus)
                data->entities[n]->pressed = data->buttons;
            else
                data->entities[n]->clicked = 0;
        }
        else
        {
            data->entities[n]->mousepos = vec2i(-1);
            /* FIXME */
            data->entities[n]->released = 0;
            data->entities[n]->pressed = 0;
            data->entities[n]->clicked = 0;
        }
    }

    data->lastfocus = best;
}

void Input::SetMouseButton(int index)
{
    data->buttons[index] = 1;

    if (data->lastfocus)
    {
        if (!data->lastfocus->pressed[index])
            data->lastfocus->clicked[index] = 1;
        data->lastfocus->pressed[index] = 1;
        data->lastfocus->released[index] = 0;
    }
}

void Input::UnsetMouseButton(int index)
{
    data->buttons[index] = 0;

    if (data->lastfocus)
    {
        if (data->lastfocus->pressed[index])
            data->lastfocus->released[index] = 1;
        data->lastfocus->pressed[index] = 0;
        data->lastfocus->clicked[index] = 0;
    }
}

} /* namespace lol */

