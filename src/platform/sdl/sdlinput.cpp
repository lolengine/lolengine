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
#   if defined HAVE_SDL_SDL_H
#      include <SDL/SDL.h>
#   else
#      include <SDL.h>
#   endif
#endif

#include "core.h"
#include "sdlinput.h"

/* We force joystick polling because no events are received when
 * there is no SDL display (eg. on the Raspberry Pi). */
#define SDL_FORCE_POLL_JOYSTICK 1

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

#   if SDL_FORCE_POLL_JOYSTICK
    SDL_JoystickEventState(SDL_QUERY);
#   else
    SDL_JoystickEventState(SDL_ENABLE);
#   endif

    /* Register all the joysticks we can find, and let the input
     * system decide what it wants to track. */
    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        SDL_Joystick *sdlstick = SDL_JoystickOpen(i);

        /* Blacklist some devices:
         *  - HDAPS, it's not a real joystick.
         *  - X360 controllers, Xinput handles them better since
         *    it won't think there is only one trigger axis. */
        char const *name = SDL_JoystickName(i);
        if (strstr(name, "HDAPS")
#   if defined USE_XINPUT
             || strstr(name, "XBOX 360 For Windows")
#   endif
             || false)
        {
            SDL_JoystickClose(sdlstick);
            continue;
        }

        Stick *stick = Input::CreateStick();
        stick->SetAxisCount(SDL_JoystickNumAxes(sdlstick));
        stick->SetButtonCount(SDL_JoystickNumButtons(sdlstick));

        /* It's possible to remap axes */
        //stick->RemapAxis(4, 2);
        //stick->RemapAxis(2, 4);

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

#   if SDL_FORCE_POLL_JOYSTICK
    /* Pump all joystick events because no event is coming to us. */
    SDL_JoystickUpdate();
    for (int j = 0; j < m_joysticks.Count(); j++)
    {
        for (int i = 0; i < SDL_JoystickNumButtons(m_joysticks[j].m1); i++)
            m_joysticks[j].m2->SetButton(i, SDL_JoystickGetButton(m_joysticks[j].m1, i));
        for (int i = 0; i < SDL_JoystickNumAxes(m_joysticks[j].m1); i++)
            m_joysticks[j].m2->SetAxis(i, (float)SDL_JoystickGetAxis(m_joysticks[j].m1, i) / 32768.f);
    }
#   endif

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

#   if !SDL_FORCE_POLL_JOYSTICK
        case SDL_JOYAXISMOTION:
            m_joysticks[event.jaxis.which].m2->SetAxis(event.jaxis.axis, (float)event.jaxis.value / 32768.f);
            break;

        case SDL_JOYBUTTONUP:
        case SDL_JOYBUTTONDOWN:
            m_joysticks[event.jbutton.which].m2->SetButton(event.jbutton.button, event.jbutton.state);
            break;
#   endif
        }
    }

    /* Send the whole keyboard state to the input system */
#if 0
    Uint8 *keystate = SDL_GetKeyState(NULL);
    for (int i = 0; i < 256; i++)
        if (keystate[i])
            Input::KeyPressed(i, seconds);
#else
    (void)seconds;
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

