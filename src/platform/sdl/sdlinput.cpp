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

#if defined USE_SDL
#   include <SDL.h>
#endif

#include "core.h"
#include "sdlinput.h"

namespace lol
{

/*
 * SDL Input implementation class
 */

class SdlInputData
{
    friend class SdlInput;

private:
    void Tick(float seconds);

    static ivec2 GetMousePos();
#if defined USE_SDL
    Array<SDL_Joystick *, Stick *> m_joysticks;
#endif
};

/*
 * Public SdlInput class
 */

SdlInput::SdlInput()
  : m_data(new SdlInputData())
{
#if defined USE_SDL
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_JOYSTICK);

    /* Register all the joysticks we can find, and let the input
     * system decide what it wants to track. */
    SDL_JoystickEventState(SDL_ENABLE);
    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        SDL_Joystick *sdlstick = SDL_JoystickOpen(i);
        Stick *stick = Input::CreateStick();
        stick->SetAxisCount(SDL_JoystickNumAxes(sdlstick));
        stick->SetButtonCount(SDL_JoystickNumButtons(sdlstick));
        m_data->m_joysticks.Push(sdlstick, stick);
    }
#endif

    m_gamegroup = GAMEGROUP_BEFORE;
}

SdlInput::~SdlInput()
{
#if defined USE_SDL
    /* Unregister all the joysticks we added */
    while (m_data->m_joysticks.Count())
    {
        SDL_JoystickClose(m_data->m_joysticks[0].m1);
        Input::DestroyStick(m_data->m_joysticks[0].m2);
        m_data->m_joysticks.Remove(0);
    }
#endif
    delete m_data;
}

void SdlInput::TickGame(float seconds)
{
    Entity::TickGame(seconds);

#if !defined _WIN32
    m_data->Tick(seconds);
#endif
}

void SdlInput::TickDraw(float seconds)
{
    Entity::TickDraw(seconds);

#if defined _WIN32
    m_data->Tick(seconds);
#endif
}

void SdlInputData::Tick(float seconds)
{
#if defined USE_SDL
    /* Handle mouse input */
    ivec2 mouse = SdlInputData::GetMousePos();;
    Input::SetMousePos(mouse);

    /* Handle keyboard and WM events */
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            Ticker::Shutdown();
            break;
#if 0
        case SDL_KEYDOWN:
            Input::KeyPressed(event.key.keysym.sym, seconds);
            break;
#endif
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            ivec2 newmouse = SdlInputData::GetMousePos();
            if (newmouse != mouse)
                Input::SetMousePos(mouse = newmouse);
            if (event.type == SDL_MOUSEBUTTONDOWN)
                Input::SetMouseButton(event.button.button - 1);
            else
                Input::UnsetMouseButton(event.button.button - 1);
            break;
        }

        case SDL_JOYAXISMOTION:
            m_joysticks[event.jaxis.which].m2->SetAxis(event.jaxis.axis, (float)event.jaxis.value / 32768.f);
            break;

        case SDL_JOYBUTTONUP:
        case SDL_JOYBUTTONDOWN:
            m_joysticks[event.jbutton.which].m2->SetButton(event.jbutton.button, event.jbutton.state);
            break;
        }
    }

    /* Send the whole keyboard state to the input system */
#if 0
    Uint8 *keystate = SDL_GetKeyState(NULL);
    for (int i = 0; i < 256; i++)
        if (keystate[i])
            Input::KeyPressed(i, seconds);
#endif
#endif
}

ivec2 SdlInputData::GetMousePos()
{
    ivec2 ret(-1, -1);

#if defined USE_SDL
    if (SDL_GetAppState() & SDL_APPMOUSEFOCUS)
    {
        SDL_GetMouseState(&ret.x, &ret.y);
        ret.y = Video::GetSize().y - 1 - ret.y;
    }
#endif
    return ret;
}

} /* namespace lol */

