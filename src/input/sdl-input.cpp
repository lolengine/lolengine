//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if LOL_USE_SDL
#   if HAVE_SDL2_SDL_H
#      include <SDL2/SDL.h>
#   elif HAVE_SDL_H
#      include <SDL.h>
#   endif
#endif
#if HAVE_EMSCRIPTEN_HTML5_H
#   include <emscripten/html5.h>
#endif

#include "input/input_internal.h"
#include "input/sdl-input.h"

/* We force joystick polling because no events are received when
 * there is no SDL display (eg. on the Raspberry Pi). */
#define SDL_FORCE_POLL_JOYSTICK 1

#if __EMSCRIPTEN__
#   define MOUSE_SPEED_MOD 10.f
#else
#   define MOUSE_SPEED_MOD 100.f
#endif

namespace lol
{

#define _SC(id, str, name) static const uint16_t SDLOL_##name = id;
#include "input/keys.h"

//-------------------------------------------------------------------------
/* DEBUG STUFF
static String ScanCodeToText(int sc)
{
    switch (sc)
    {
#define _SC(id, str, name) \
    case id: return String(str);
#include "input/keys.h"
    default:
        msg::error("ScanCodeToText unknown scancode %0d\n", sc);
    }
    return String();
}
*/
//-------------------------------------------------------------------------
/* DEBUG STUFF
static String ScanCodeToName(int sc)
{
    switch (sc)
    {
#define _SC(id, str, name) \
        case id: return String(#name);
#include "input/keys.h"
    default:
        msg::error("ScanCodeToText unknown scancode %0d\n", sc);
    }
    return String();
}
*/

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

#if LOL_USE_SDL
    array<SDL_Joystick *, InputDeviceInternal *> m_joysticks;
    InputDeviceInternal *m_mouse;
    InputDeviceInternal *m_keyboard;
#endif // LOL_USE_SDL

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

#if __EMSCRIPTEN__
    emscripten_sample_gamepad_data();
#endif
#if LOL_USE_SDL
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
#endif

    m_data->m_keyboard = InputDeviceInternal::CreateStandardKeyboard();
    m_data->m_mouse = InputDeviceInternal::CreateStandardMouse();

#if LOL_USE_SDL
    // XXX: another option is to properly handle gamepad support
#   if !__EMSCRIPTEN__
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
        char const *name = SDL_JoystickName(sdlstick);
        if (strstr(name, "HDAPS")
#       if LOL_USE_XINPUT
             || strstr(name, "XBOX 360 For Windows")
#       endif //LOL_USE_XINPUT
             || false)
        {
            SDL_JoystickClose(sdlstick);
            continue;
        }

        //format("Joystick%d", i + 1).c_str()
        InputDeviceInternal* stick = new InputDeviceInternal(g_name_joystick(i + 1));
        for (int j = 0; j < SDL_JoystickNumAxes(sdlstick); ++j)
            stick->AddAxis(format("Axis%d", j + 1).c_str());
        for (int j = 0; j < SDL_JoystickNumButtons(sdlstick); ++j)
            stick->AddKey(format("Button%d", j + 1).c_str());

        m_data->m_joysticks.push(sdlstick, stick);
    }
#   endif // __EMSCRIPTEN__
#endif

    m_gamegroup = GAMEGROUP_INPUT;
}

SdlInput::~SdlInput()
{
#if LOL_USE_SDL && !__EMSCRIPTEN__
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

void SdlInput::tick_game(float seconds)
{
    Entity::tick_game(seconds);

    if (!m_data->m_tick_in_draw_thread)
        m_data->Tick(seconds);
}

void SdlInput::tick_draw(float seconds, Scene &scene)
{
    Entity::tick_draw(seconds, scene);

    if (m_data->m_tick_in_draw_thread)
        m_data->Tick(seconds);
}

void SdlInputData::Tick(float seconds)
{
#if LOL_USE_SDL
    /* FIXME: maybe we should make use of this? */
    UNUSED(seconds);

    /* Pump all joystick events because no event is coming to us. */
#   if SDL_FORCE_POLL_JOYSTICK && !__EMSCRIPTEN__
    SDL_JoystickUpdate();
    for (int j = 0; j < m_joysticks.count(); j++)
    {
        for (int i = 0; i < SDL_JoystickNumButtons(m_joysticks[j].m1); i++)
            m_joysticks[j].m2->internal_set_key(i, SDL_JoystickGetButton(m_joysticks[j].m1, i) != 0);
        for (int i = 0; i < SDL_JoystickNumAxes(m_joysticks[j].m1); i++)
            m_joysticks[j].m2->internal_set_axis(i, (float)SDL_JoystickGetAxis(m_joysticks[j].m1, i) / 32768.f);
    }
#   endif

    m_mouse->internal_set_axis(4, 0);

    if (m_keyboard->IsTextInputActive())
        SDL_StartTextInput();
    else
        SDL_StopTextInput();

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
            switch (int sc = event.key.keysym.scancode)
            {
            //Lock management
            case SDLOL_CapsLock:
            case SDLOL_ScrollLock:
            case SDLOL_NumLockClear:
#   if defined SDLOL_CapsLock && defined SDLOL_ScrollLock && defined SDLOL_NumLockClear
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
                    m_keyboard->internal_set_key(sc2, !m_keyboard->GetKey(sc2));
                    /* DEBUG STUFF
                    msg::debug("Repeat: 0x%02x : %s/%s/%s/%i\n",
                        (int)m_keyboard, ScanCodeToText(sc2).C(), ScanCodeToName(sc2).C(),
                        m_keyboard->GetKey(sc2) ? "up" : "down", event.key.repeat);
                    */
                }
#   endif
            default:
                // Set key updates the corresponding key
                m_keyboard->internal_set_key(sc, event.type == SDL_KEYDOWN);
                break;
            }
            break;

        //case SDL_TEXTEDITING: //TODO: handle that?
        case SDL_TEXTINPUT:
                m_keyboard->internal_add_text(event.text.text);
                break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            //event.button.which
            m_mouse->internal_set_key(event.button.button - 1, event.type == SDL_MOUSEBUTTONDOWN);
            break;
        case SDL_MOUSEWHEEL:
            m_mouse->internal_set_axis(4, (float)event.button.y);
            break;
        case SDL_WINDOWEVENT:
        {
            switch (event.window.event)
            {
                case SDL_WINDOWEVENT_ENTER:
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    m_mouse->internal_set_key(3, true);
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    m_mouse->internal_set_key(3, false);
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    Video::Resize(ivec2(event.window.data1, event.window.data2));
                    break;
            }
            break;
        }

#   if !SDL_FORCE_POLL_JOYSTICK
        case SDL_JOYAXISMOTION:
            m_joysticks[event.jaxis.which].m2->internal_set_axis(event.jaxis.axis, (float)event.jaxis.value / 32768.f);
            break;

        case SDL_JOYBUTTONUP:
        case SDL_JOYBUTTONDOWN:
            m_joysticks[event.jbutton.which].m2->internal_set_key(event.jbutton.button, event.jbutton.state);
            break;
#   endif
        }
    }

    /* Handle mouse input */
    ivec2 mouse = SdlInputData::GetMousePos();
    if (InputDeviceInternal::GetMouseCapture() != m_mousecapture)
    {
        m_mousecapture = InputDeviceInternal::GetMouseCapture();
#   if LOL_USE_SDL
        SDL_SetRelativeMouseMode(m_mousecapture ? SDL_TRUE : SDL_FALSE);
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
        m_mouse->internal_set_axis(0, (mouse.x - vprevmouse.x) * MOUSE_SPEED_MOD / max_screen_size);
        // Y Axis is also negated to match the usual joystick Y axis (negatives values are for the upper direction)
        m_mouse->internal_set_axis(1,-(mouse.y - vprevmouse.y) * MOUSE_SPEED_MOD / max_screen_size);
        //Pixel movement
        m_mouse->internal_set_axis(2, (mouse.x - vprevmouse.x));
        m_mouse->internal_set_axis(3,-(mouse.y - vprevmouse.y));
    }

    if (m_mousecapture)
    {
        mouse = ivec2(m_app * .5f);
        SdlInputData::SetMousePos(mouse);
    }

    m_prevmouse = mouse;

#else
    UNUSED(seconds);
#endif //LOL_USE_SDL
}

// NOTE: these two functions are pointless now and could be inlined directly
ivec2 SdlInputData::GetMousePos()
{
    ivec2 ret(-1, -1);

#if LOL_USE_SDL
    {
        SDL_GetMouseState(&ret.x, &ret.y);
        ret.y = Video::GetSize().y - 1 - ret.y;
    }
#endif
    return ret;
}

void SdlInputData::SetMousePos(ivec2 position)
{
    UNUSED(position);
#if LOL_USE_SDL
    // FIXME: how do I warped mouse?
    //SDL_WarpMouse((uint16_t)position.x, (uint16_t)position.y);
#endif
}

} /* namespace lol */

