//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>

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
    ivec2 mouse;
    ivec3 buttons;

    static int const MAX_ENTITIES = 100;
    WorldEntity *entities[MAX_ENTITIES];
    int nentities;
    WorldEntity *lastfocus;

    Array<Stick *> m_sticks;
}
inputdata;

static InputData * const data = &inputdata;

/*
 * Public Input class
 */

#if 0
vec2 Input::GetAxis(int axis)
{
    vec2 ret;

#if defined USE_SDL
    /* Simulate a joystick using the keyboard. This SDL call is free. */
#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION >= 3
    Uint8 *keystate = SDL_GetKeyboardState(NULL);
#else
    Uint8 *keystate = SDL_GetKeyState(NULL);
#endif
    int left = keystate[SDLK_d] - (keystate[SDLK_a] | keystate[SDLK_q]);
    int up = (keystate[SDLK_w] | keystate[SDLK_z]) - keystate[SDLK_s] ;
    ret.x += left;
    ret.y += up;
    if (left && up)
        ret = ret * sqrtf(0.5f);
#else
    ret = vec2(0, 0);
#endif

    return ret;
}
#endif

ivec2 Input::GetMousePos()
{
    return data->mouse;
}

ivec3 Input::GetMouseButtons()
{
    return data->buttons;
}

//BH : Added this, is a v0.1 Alpha version.
int Input::GetButtonState(int button)
{
#if defined USE_SDL
#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION >= 3
    Uint8 *keystate = SDL_GetKeyboardState(NULL);
#else
    Uint8 *keystate = SDL_GetKeyState(NULL);
#endif
    return keystate[button];
#else
    return 0;
#endif
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

void Input::SetMousePos(ivec2 coord)
{
    data->mouse = coord;

    WorldEntity *top = NULL;

    for (int n = 0; n < data->nentities; n++)
    {
        if (coord.x < data->entities[n]->m_bbox[0].x
             || coord.x >= data->entities[n]->m_bbox[1].x
             || coord.y < data->entities[n]->m_bbox[0].y
             || coord.y >= data->entities[n]->m_bbox[1].y)
            continue;

        if (!top || top->m_bbox[1].z < data->entities[n]->m_bbox[1].z)
            top = data->entities[n];
    }

    for (int n = 0; n < data->nentities; n++)
    {
        if (data->entities[n] == top)
        {
            data->entities[n]->m_mousepos = coord - (ivec2)top->m_bbox[0].xy;
            if (top != data->lastfocus)
                data->entities[n]->m_pressed = data->buttons;
            else
                data->entities[n]->m_clicked = ivec3(0);
        }
        else
        {
            data->entities[n]->m_mousepos = ivec2(-1);
            /* FIXME */
            data->entities[n]->m_released = ivec3(0);
            data->entities[n]->m_pressed = ivec3(0);
            data->entities[n]->m_clicked = ivec3(0);
        }
    }

    data->lastfocus = top;
}

void Input::SetMouseButton(int index)
{
    data->buttons[index] = 1;

    if (data->lastfocus)
    {
        if (!data->lastfocus->m_pressed[index])
            data->lastfocus->m_clicked[index] = 1;
        data->lastfocus->m_pressed[index] = 1;
        data->lastfocus->m_released[index] = 0;
    }
}

void Input::UnsetMouseButton(int index)
{
    data->buttons[index] = 0;

    if (data->lastfocus)
    {
        if (data->lastfocus->m_pressed[index])
            data->lastfocus->m_released[index] = 1;
        data->lastfocus->m_pressed[index] = 0;
        data->lastfocus->m_clicked[index] = 0;
    }
}

Stick *Input::CreateStick()
{
    Stick *stick = new Stick();
    Ticker::Ref(stick);
    data->m_sticks.Push(stick);
    return stick;
}

void Input::DestroyStick(Stick *stick)
{
    for (int i = 0; i < data->m_sticks.Count(); i++)
        if (data->m_sticks[i] == stick)
            data->m_sticks.Remove(i);
    Ticker::Unref(stick);
}

Stick *Input::TrackStick()
{
    /* FIXME: add the possibility to choose amongst sticks */
    if (!data->m_sticks.Count())
        return NULL;
    Ticker::Ref(data->m_sticks[0]);
    return data->m_sticks[0];
}

void Input::UntrackStick(Stick *stick)
{
    Ticker::Unref(stick);
}

} /* namespace lol */

