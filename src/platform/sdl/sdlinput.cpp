//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#if USE_SDL
#   if HAVE_SDL_SDL_H
#      include <SDL/SDL.h>
#   else
#      include <SDL.h>
#   endif
#endif

#include "core.h"
#include "sdlinput.h"

#include "input/input_internal.h"

/* We force joystick polling because no events are received when
 * there is no SDL display (eg. on the Raspberry Pi). */
#define SDL_FORCE_POLL_JOYSTICK 1
#if EMSCRIPTEN
#define MOUSE_SPEED_MOD 10.f
#else
#define MOUSE_SPEED_MOD 100.f
#endif

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
    static void SetMousePos(ivec2 position);

#if USE_SDL
    SdlInputData(int app_w, int app_h, int screen_w, int screen_h) :
        m_prevmouse(ivec2::zero),
        m_app(vec2((float)app_w, (float)app_h)),
        m_screen(vec2((float)screen_w, (float)screen_h)),
        m_mousecapture(false)
    { }

    Array<SDL_Joystick*, InputDeviceInternal*> m_joysticks;
    InputDeviceInternal* m_mouse;
    InputDeviceInternal* m_keyboard;

    ivec2 m_prevmouse;
    vec2 m_app;
    vec2 m_screen;
    bool m_mousecapture;
#endif // USE_SDL
};

/*
 * Public SdlInput class
 */

SdlInput::SdlInput(int app_w, int app_h, int screen_w, int screen_h)
#if USE_SDL
  : m_data(new SdlInputData(app_w, app_h, screen_w, screen_h))
#endif //USE_SDL
{
#if USE_SDL
    /* Enable Unicode translation of keyboard events */
    SDL_EnableUNICODE(1);

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_JOYSTICK);

    m_data->m_keyboard = InputDeviceInternal::CreateStandardKeyboard();
    m_data->m_mouse = InputDeviceInternal::CreateStandardMouse();

#   if !EMSCRIPTEN
#       if SDL_FORCE_POLL_JOYSTICK
    SDL_JoystickEventState(SDL_QUERY);
#       else
    SDL_JoystickEventState(SDL_ENABLE);
#       endif //SDL_FORCE_POLL_JOYSTICK

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
#       if USE_XINPUT
             || strstr(name, "XBOX 360 For Windows")
#       endif //USE_XINPUT
             || false)
        {
            SDL_JoystickClose(sdlstick);
            continue;
        }

        InputDeviceInternal* stick = new InputDeviceInternal(String::Printf("Joystick%d", i+1).C());
        for (int j = 0; j < SDL_JoystickNumAxes(sdlstick); ++j)
            stick->AddAxis(String::Printf("Axis%d", j + 1).C());
        for (int j = 0; j < SDL_JoystickNumButtons(sdlstick); ++j)
            stick->AddKey(String::Printf("Button%d", j + 1).C());

        m_data->m_joysticks.Push(sdlstick, stick);
    }
#   endif //EMSCRIPTEN
#else
    UNUSED(app_w, app_h, screen_w, screen_h);
#endif //USE_SDL

    m_gamegroup = GAMEGROUP_BEFORE;
}

SdlInput::~SdlInput()
{
#if USE_SDL && !EMSCRIPTEN
    /* Unregister all the joysticks we added */
    while (m_data->m_joysticks.Count())
    {
        SDL_JoystickClose(m_data->m_joysticks[0].m1);
        delete m_data->m_joysticks[0].m2;
        m_data->m_joysticks.Remove(0);
    }
#endif
    delete m_data;
}

void SdlInput::TickGame(float seconds)
{
    Entity::TickGame(seconds);

#if !_WIN32
    m_data->Tick(seconds);
#endif
}

void SdlInput::TickDraw(float seconds)
{
    Entity::TickDraw(seconds);

#if _WIN32
    m_data->Tick(seconds);
#endif //_WIN32
}

void SdlInputData::Tick(float seconds)
{
#if USE_SDL
    /* Pump all joystick events because no event is coming to us. */
#   if SDL_FORCE_POLL_JOYSTICK && !EMSCRIPTEN
    SDL_JoystickUpdate();
    for (int j = 0; j < m_joysticks.Count(); j++)
    {
        for (int i = 0; i < SDL_JoystickNumButtons(m_joysticks[j].m1); i++)
            m_joysticks[j].m2->SetKey(i, SDL_JoystickGetButton(m_joysticks[j].m1, i) != 0);
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
            if (event.key.keysym.unicode)
                fprintf(stderr, "%c (0x%04X)\n", event.key.keysym.unicode, event.key.keysym.unicode);
            break;
#endif
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            if (event.button.button != SDL_BUTTON_WHEELUP && event.button.button != SDL_BUTTON_WHEELDOWN)
                m_mouse->SetKey(event.button.button - 1, event.type == SDL_MOUSEBUTTONDOWN);
            // TODO: mouse wheel as axis
            break;
        }

#   if !SDL_FORCE_POLL_JOYSTICK
        case SDL_JOYAXISMOTION:
            m_joysticks[event.jaxis.which].m2->SetAxis(event.jaxis.axis, (float)event.jaxis.value / 32768.f);
            break;

        case SDL_JOYBUTTONUP:
        case SDL_JOYBUTTONDOWN:
            m_joysticks[event.jbutton.which].m2->SetKey(event.jbutton.button, event.jbutton.state);
            break;
#   endif
        }
    }

    /* Handle mouse input */
    ivec2 mouse = SdlInputData::GetMousePos();
    if (InputDeviceInternal::GetMouseCapture() != m_mousecapture)
    {
        m_mousecapture = InputDeviceInternal::GetMouseCapture();
        SDL_WM_GrabInput(m_mousecapture ? SDL_GRAB_ON : SDL_GRAB_OFF);
        mouse = (ivec2)m_app / 2;
        SdlInputData::SetMousePos(mouse);
        //SDL_ShowCursor(m_mousecapture ? SDL_DISABLE : SDL_ENABLE);
    }

    if (mouse.x >= 0 && mouse.x < m_app.x && mouse.y >= 0 && mouse.y < m_app.y)
    {
        //We need the max if we want coherent mouse speed between axis
        float max_screen_size = lol::max(m_screen.x, m_screen.y);
        vec2 vmouse = vec2(mouse);
        vec2 vprevmouse = vec2(m_prevmouse);
        m_mouse->SetCursor(0, vmouse / m_app, mouse);
        // Note: 100.0f is an arbitrary value that makes it feel about the same than an xbox controller joystick
        m_mouse->SetAxis(0, (mouse.x - vprevmouse.x) * MOUSE_SPEED_MOD / max_screen_size);
        // Y Axis is also negated to match the usual joystick Y axis (negatives values are for the upper direction)
        m_mouse->SetAxis(1,-(mouse.y - vprevmouse.y) * MOUSE_SPEED_MOD / max_screen_size);
    }

    //Mouse is focused, Validate the InScreen Key
    //Hardcoded 3, not very nice.
#   if !EMSCRIPTEN
    m_mouse->SetKey(3, !!(SDL_GetAppState() & SDL_APPMOUSEFOCUS));
#else //Emscripten doesn't seem to handle SDL_APPMOUSEFOCUS
    m_mouse->SetKey(3, true);
#endif

    if (m_mousecapture)
    {
        mouse = ivec2(m_app * .5f);
        SdlInputData::SetMousePos(mouse);
    }

    m_prevmouse = mouse;

#   if SDL_VERSION_ATLEAST(1,3,0)
    Uint8 *sdlstate = SDL_GetKeyboardState(nullptr);
#   else
    Uint8 *sdlstate = SDL_GetKeyState(nullptr);
#   endif

    int keyindex = 0;
#       define KEY_FUNC(name, index) \
            m_keyboard->SetKey(keyindex++, sdlstate[index] != 0);
        /* FIXME: we ignore SDLK_WORLD_0, which means our list of
         * keys and SDL's list of keys could be out of sync. */
#    include "input/keys.h"
#    undef KEY_FUNC

#else
    UNUSED(seconds);
#endif //USE_SDL
}

// NOTE: these two functions are pointless now and could be inlined directly
ivec2 SdlInputData::GetMousePos()
{
    ivec2 ret(-1, -1);

#if USE_SDL
#   if !EMSCRIPTEN
    if (SDL_GetAppState() & SDL_APPMOUSEFOCUS)
#   endif
    {
        SDL_GetMouseState(&ret.x, &ret.y);
        ret.y = Video::GetSize().y - 1 - ret.y;
    }
#endif //USE_SDL
    return ret;
}

void SdlInputData::SetMousePos(ivec2 position)
{
#if USE_SDL
    SDL_WarpMouse((uint16_t)position.x, (uint16_t)position.y);
#else
    UNUSED(position);
#endif //USE_SDL
}

} /* namespace lol */

