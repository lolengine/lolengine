//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if USE_SDL || USE_OLD_SDL
#   if HAVE_SDL2_SDL_H
#      include <SDL2/SDL.h>
#   elif HAVE_SDL_SDL_H
#      include <SDL/SDL.h>
#   else
#      include <SDL.h>
#   endif
#endif

#include "sdlinput.h"

#include "input/input_internal.h"

/* We force joystick polling because no events are received when
 * there is no SDL display (eg. on the Raspberry Pi). */
#define SDL_FORCE_POLL_JOYSTICK 1

#if EMSCRIPTEN
#   define MOUSE_SPEED_MOD 10.f
#else
#   define MOUSE_SPEED_MOD 100.f
#endif

namespace lol
{

#if USE_OLD_SDL
/* Quick and dirty for now... This is deprecated anyway. */
static int sdl12_to_scancode(int ch, int sc)
{
    if (ch >= 'a' && ch <= 'z')
        ch = ch - 'a' + 'A';

#   define _SC(id, str, name) if (ch == str[0]) return id;
#   include "input/keys.h"

    return 0;
}
#else
    //-------------------------------------------------------------------------
#   define _SC(id, str, name) static const uint16_t SDLOL_##name = id;
#   include "input/keys.h"
    //-------------------------------------------------------------------------
    static bool ScanCodeIsValid(int sc)
    {
        switch (sc)
        {
#   define _SC(id, str, name) \
        case id: return true;
#   include "input/keys.h"
        default: return false;
        }
        return false;
    }
    //-------------------------------------------------------------------------
    static String ScanCodeToText(int sc)
    {
        switch (sc)
        {
#   define _SC(id, str, name) \
        case id: return String(str);
#   include "input/keys.h"
        default:
            msg::error("ScanCodeToText unknown scancode %0d\n", sc);
        }
        return String();
    }
    //-------------------------------------------------------------------------
    static String ScanCodeToName(int sc)
    {
        switch (sc)
        {
#   define _SC(id, str, name) \
        case id: return String(#name);
#   include "input/keys.h"
        default:
            msg::error("ScanCodeToText unknown scancode %0d\n", sc);
        }
        return String();
    }
#endif

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

    SdlInputData(int app_w, int app_h, int screen_w, int screen_h)
      : m_mouse(nullptr),
        m_keyboard(nullptr),
        m_prevmouse(ivec2::zero),
        m_app(vec2((float)app_w, (float)app_h)),
        m_screen(vec2((float)screen_w, (float)screen_h)),
        m_mousecapture(false),
        m_tick_in_draw_thread(false)
    { }

#if USE_SDL || USE_OLD_SDL
    array<SDL_Joystick *, InputDeviceInternal *> m_joysticks;
    InputDeviceInternal *m_mouse;
    InputDeviceInternal *m_keyboard;
#endif // USE_SDL

    ivec2 m_prevmouse;
    vec2 m_app;
    vec2 m_screen;

    bool m_mousecapture;
    bool m_tick_in_draw_thread;
};

/*
 * Public SdlInput class
 */

SdlInput::SdlInput(int app_w, int app_h, int screen_w, int screen_h)
  : m_data(new SdlInputData(app_w, app_h, screen_w, screen_h))
{
#if _WIN32
    m_data->m_tick_in_draw_thread = true;
#endif

#if USE_OLD_SDL
    /* Enable Unicode translation of keyboard events */
    SDL_EnableUNICODE(1);
#endif

#if USE_SDL || USE_OLD_SDL
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
#endif

    m_data->m_keyboard = InputDeviceInternal::CreateStandardKeyboard();
    m_data->m_mouse = InputDeviceInternal::CreateStandardMouse();

#if USE_SDL || USE_OLD_SDL
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
#       if USE_SDL
        char const *name = SDL_JoystickName(sdlstick);
#       elif USE_OLD_SDL
        char const *name = SDL_JoystickName(i);
#       endif
        if (strstr(name, "HDAPS")
#       if USE_XINPUT
             || strstr(name, "XBOX 360 For Windows")
#       endif //USE_XINPUT
             || false)
        {
            SDL_JoystickClose(sdlstick);
            continue;
        }

        InputDeviceInternal* stick = new InputDeviceInternal(String::format("Joystick%d", i+1).C());
        for (int j = 0; j < SDL_JoystickNumAxes(sdlstick); ++j)
            stick->AddAxis(String::format("Axis%d", j + 1).C());
        for (int j = 0; j < SDL_JoystickNumButtons(sdlstick); ++j)
            stick->AddKey(String::format("Button%d", j + 1).C());

        m_data->m_joysticks.push(sdlstick, stick);
    }
#   endif //EMSCRIPTEN
#endif

    m_gamegroup = GAMEGROUP_INPUT;
}

SdlInput::~SdlInput()
{
#if (USE_SDL || USE_OLD_SDL) && !EMSCRIPTEN
    /* Unregister all the joysticks we added */
    while (m_data->m_joysticks.count())
    {
        SDL_JoystickClose(m_data->m_joysticks[0].m1);
        delete m_data->m_joysticks[0].m2;
        m_data->m_joysticks.remove(0);
    }
#endif
    delete m_data;
}

void SdlInput::TickGame(float seconds)
{
    Entity::TickGame(seconds);

    if (!m_data->m_tick_in_draw_thread)
        m_data->Tick(seconds);
}

void SdlInput::TickDraw(float seconds, Scene &scene)
{
    Entity::TickDraw(seconds, scene);

    if (m_data->m_tick_in_draw_thread)
        m_data->Tick(seconds);
}

void SdlInputData::Tick(float seconds)
{
#if USE_SDL || USE_OLD_SDL
    /* Pump all joystick events because no event is coming to us. */
#   if SDL_FORCE_POLL_JOYSTICK && !EMSCRIPTEN
    SDL_JoystickUpdate();
    for (int j = 0; j < m_joysticks.count(); j++)
    {
        for (int i = 0; i < SDL_JoystickNumButtons(m_joysticks[j].m1); i++)
            m_joysticks[j].m2->SetKey(i, SDL_JoystickGetButton(m_joysticks[j].m1, i) != 0);
        for (int i = 0; i < SDL_JoystickNumAxes(m_joysticks[j].m1); i++)
            m_joysticks[j].m2->SetAxis(i, (float)SDL_JoystickGetAxis(m_joysticks[j].m1, i) / 32768.f);
    }
#   endif

    m_mouse->SetAxis(4, 0);

    /* Handle keyboard and WM events */
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            Ticker::Shutdown();
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
#   if USE_OLD_SDL
            switch (int sc = sdl12_to_scancode(event.key.keysym.sym,
                                               event.key.keysym.scancode))
#   else
            switch (int sc = event.key.keysym.scancode)
#   endif
            {
                //Lock management
#   if defined SDLOL_CapsLock && defined SDLOL_ScrollLock && defined SDLOL_NumLockClear
            case SDLOL_CapsLock:
            case SDLOL_ScrollLock:
            case SDLOL_NumLockClear:
                //Update status on key down only
                if (event.type == SDL_KEYDOWN)
                {
                    int sc2 = sc;
                    switch (sc)
                    {
                    case SDLOL_CapsLock:
                        sc2 = SDLOL_CapsLockStatus;
                        break;
                    case SDLOL_ScrollLock:
                        sc2 = SDLOL_ScrollLockStatus;
                        break;
                    case SDLOL_NumLockClear:
                        sc2 = SDLOL_NumLockClearStatus;
                        break;
                    }
                    m_keyboard->SetKey(sc2, !m_keyboard->GetKey(sc2));
                    /* DEBUG STUFF
                    msg::info("Repeat: 0x%02x : %s/%s/%s/%i\n",
                        (int)m_keyboard, ScanCodeToText(sc2).C(), ScanCodeToName(sc2).C(),
                        m_keyboard->GetKey(sc2) ? "up" : "down", event.key.repeat);
                    */
                }
#   endif
            default:
#   if USE_OLD_SDL
                m_keyboard->SetKey(sc ? sc : event.key.keysym.scancode,
                                   event.type == SDL_KEYDOWN);
#   else
                if (ScanCodeIsValid(sc))
                {
                    m_keyboard->SetKey(sc, event.type == SDL_KEYDOWN);
                    if (event.type == SDL_KEYDOWN
                        && !m_keyboard->GetKey(SDLOL_RCtrl)
                        && !m_keyboard->GetKey(SDLOL_LCtrl)
                        && !m_keyboard->GetKey(SDLOL_RAlt)
                        && !m_keyboard->GetKey(SDLOL_LAlt))
                    {
                        String str = ScanCodeToText(sc);
                        str.case_change(m_keyboard->GetKey(SDLOL_CapsLockStatus)
                                         ^ (m_keyboard->GetKey(SDLOL_RShift)
                                         || m_keyboard->GetKey(SDLOL_LShift)));
                        m_keyboard->AddText(str);
                    }
                    /* DEBUG STUFF
                    msg::info("Repeat: 0x%02x : %s/%s/%s/%i\n",
                        (int)m_keyboard, ScanCodeToText(sc).C(), ScanCodeToName(sc).C(),
                        event.type == SDL_KEYDOWN ? "up" : "down", event.key.repeat);
                    */
                }
                /* DEBUG STUFF
                else
                    msg::error("unknown keypress (sym 0x%02x, scancode %0d)\n",
                                event.key.keysym.sym, event.key.keysym.scancode);
                */
#   endif
            }
            break;

#   if USE_OLD_SDL
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            if (event.button.button != SDL_BUTTON_WHEELUP && event.button.button != SDL_BUTTON_WHEELDOWN)
                m_mouse->SetKey(event.button.button - 1, event.type == SDL_MOUSEBUTTONDOWN);
            else
                m_mouse->SetAxis(4, (event.button.button != SDL_BUTTON_WHEELUP) ? (1) : (-1));
            break;
#   else
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            m_mouse->SetKey(event.button.button - 1, event.type == SDL_MOUSEBUTTONDOWN);
            break;
        case SDL_MOUSEWHEEL:
            m_mouse->SetAxis(4, (float)event.button.y);
            break;
        case SDL_WINDOWEVENT:
        {
            switch (event.window.event)
            {
                case SDL_WINDOWEVENT_ENTER:
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    m_mouse->SetKey(3, true);
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    m_mouse->SetKey(3, false);
                    break;
            }
            break;
        }
#   endif

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
#   if USE_SDL
        SDL_SetRelativeMouseMode(m_mousecapture ? SDL_TRUE : SDL_FALSE);
#   elif USE_OLD_SDL
        SDL_WM_GrabInput(m_mousecapture ? SDL_GRAB_ON : SDL_GRAB_OFF);
#   endif
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
        //Pixel movement
        m_mouse->SetAxis(2, (mouse.x - vprevmouse.x));
        m_mouse->SetAxis(3,-(mouse.y - vprevmouse.y));
    }

    //Mouse is focused, Validate the InScreen Key
    //Hardcoded 3, not very nice.
#   if !EMSCRIPTEN && USE_OLD_SDL
    m_mouse->SetKey(3, !!(SDL_GetAppState() & SDL_APPMOUSEFOCUS));
#   else
    //Handled in PollEvent
#   endif

    if (m_mousecapture)
    {
        mouse = ivec2(m_app * .5f);
        SdlInputData::SetMousePos(mouse);
    }

    m_prevmouse = mouse;

#else
    UNUSED(seconds);
#endif //USE_SDL
}

// NOTE: these two functions are pointless now and could be inlined directly
ivec2 SdlInputData::GetMousePos()
{
    ivec2 ret(-1, -1);

#if USE_SDL || USE_OLD_SDL
#   if !EMSCRIPTEN && USE_OLD_SDL
    if (SDL_GetAppState() & SDL_APPMOUSEFOCUS)
#   endif
    {
        SDL_GetMouseState(&ret.x, &ret.y);
        ret.y = Video::GetSize().y - 1 - ret.y;
    }
#endif
    return ret;
}

void SdlInputData::SetMousePos(ivec2 position)
{
#if USE_SDL
    // FIXME: how do I warped mouse?
#elif USE_OLD_SDL
    SDL_WarpMouse((uint16_t)position.x, (uint16_t)position.y);
#else
    UNUSED(position);
#endif
}

} /* namespace lol */

