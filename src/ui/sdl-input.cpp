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

#include "ui/sdl-input.h"
#include "ui/input.h"

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

/*
 * Public SdlInput class
 */

SdlInput::SdlInput(int app_w, int app_h, int screen_w, int screen_h)
  : m_app(vec2((float)app_w, (float)app_h)),
    m_screen(vec2((float)screen_w, (float)screen_h))
{
#if _WIN32
    m_tick_in_draw_thread = true;
#endif

#if __EMSCRIPTEN__
    emscripten_sample_gamepad_data();
#endif
#if LOL_USE_SDL
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
#endif

    // XXX: another option for emscripten is to properly support gamepads
#if LOL_USE_SDL && !__EMSCRIPTEN__
    SDL_JoystickEventState(SDL_FORCE_POLL_JOYSTICK ? SDL_QUERY : SDL_ENABLE);

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

        InputDevice* stick = new InputDevice(g_name_joystick(i + 1));
        for (int j = 0; j < SDL_JoystickNumAxes(sdlstick); ++j)
            stick->AddAxis(format("Axis%d", j + 1).c_str());
        for (int j = 0; j < SDL_JoystickNumButtons(sdlstick); ++j)
            stick->AddKey(format("Button%d", j + 1).c_str());

        m_joysticks.push(sdlstick, stick);
    }
#endif

    m_gamegroup = tickable::group::game::input;
}

SdlInput::~SdlInput()
{
#if LOL_USE_SDL && !__EMSCRIPTEN__
    /* Unregister all the joysticks we added */
    while (m_joysticks.count())
    {
        SDL_JoystickClose(m_joysticks[0].m1);
        delete m_joysticks[0].m2;
        m_joysticks.remove(0);
    }
#endif
}

void SdlInput::tick_game(float seconds)
{
    Entity::tick_game(seconds);

    if (!m_tick_in_draw_thread)
        tick(seconds);
}

void SdlInput::tick_draw(float seconds, Scene &scene)
{
    Entity::tick_draw(seconds, scene);

    if (m_tick_in_draw_thread)
        tick(seconds);
}

void SdlInput::tick(float seconds)
{
#if LOL_USE_SDL
    /* FIXME: maybe we should make use of this? */
    UNUSED(seconds);

    auto keyboard = input::keyboard();
    auto mouse = input::mouse();

    /* Pump all joystick events because no event is coming to us. */
#   if SDL_FORCE_POLL_JOYSTICK && !__EMSCRIPTEN__
    SDL_JoystickUpdate();
    for (int j = 0; j < m_joysticks.count(); j++)
    {
        for (int i = 0; i < SDL_JoystickNumButtons(m_joysticks[j].m1); i++)
            m_joysticks[j].m2->internal_set_button((input::button)i, SDL_JoystickGetButton(m_joysticks[j].m1, i) != 0);
        for (int i = 0; i < SDL_JoystickNumAxes(m_joysticks[j].m1); i++)
            m_joysticks[j].m2->internal_set_axis(i, (float)SDL_JoystickGetAxis(m_joysticks[j].m1, i) / 32768.f);
    }
#   endif

    keyboard->internal_begin_frame();
    mouse->internal_set_axis(4, 0);

    if (keyboard->capture_text())
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
            switch (auto sc = (input::key)event.key.keysym.scancode)
            {
            // Lock management
            case input::key::SC_CapsLock:
            case input::key::SC_ScrollLock:
            case input::key::SC_NumLockClear:
                // Update status on key down only
                if (event.type == SDL_KEYDOWN)
                {
                    auto sc2 = sc;
                    if (sc == input::key::SC_CapsLock)
                        sc2 = input::key::SC_CapsLockStatus;
                    else if (sc == input::key::SC_ScrollLock)
                        sc2 = input::key::SC_ScrollLockStatus;
                    else if (sc == input::key::SC_NumLockClear)
                        sc2 = input::key::SC_NumLockClearStatus;
                    keyboard->internal_set_key(sc2, !keyboard->key(sc2));
                }
                LOL_ATTR_FALLTHROUGH
            default:
                // Set key updates the corresponding key
                keyboard->internal_set_key(sc, event.type == SDL_KEYDOWN);
                break;
            }
            break;

        //case SDL_TEXTEDITING: //TODO: handle that?
        case SDL_TEXTINPUT:
                keyboard->internal_add_text(event.text.text);
                break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            //event.button.which
            mouse->internal_set_button((input::button)(event.button.button - 1), event.type == SDL_MOUSEBUTTONDOWN);
            break;
        case SDL_MOUSEWHEEL:
            mouse->internal_set_axis(4, (float)event.button.y);
            break;
        case SDL_WINDOWEVENT:
        {
            switch (event.window.event)
            {
                case SDL_WINDOWEVENT_ENTER:
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    mouse->internal_set_button(input::button::BTN_Focus, true);
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    mouse->internal_set_button(input::button::BTN_Focus, false);
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
    ivec2 mouse_pos(-1, -1);
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    mouse_pos.y = Video::GetSize().y - 1 - mouse_pos.y;

    if (mouse->capture() != m_mousecapture)
    {
        m_mousecapture = mouse->capture();
        SDL_SetRelativeMouseMode(m_mousecapture ? SDL_TRUE : SDL_FALSE);
        mouse_pos = ivec2(m_app * .5f);

        // FIXME: how do I warped mouse?
        //SDL_WarpMouse((uint16_t)mouse_pos.x, (uint16_t)mouse_pos.y);
        //SDL_ShowCursor(m_mousecapture ? SDL_DISABLE : SDL_ENABLE);
    }

    if (mouse_pos.x >= 0 && mouse_pos.x < m_app.x && mouse_pos.y >= 0 && mouse_pos.y < m_app.y)
    {
        //We need the max if we want coherent mouse speed between axis
        float max_screen_size = lol::max(m_screen.x, m_screen.y);
        vec2 vmouse = vec2(mouse_pos);
        vec2 vprevmouse = vec2(m_prev_mouse_pos);
        mouse->internal_set_cursor(0, vmouse / m_app, mouse_pos);
        // Note: 100.0f is an arbitrary value that makes it feel about the same than an xbox controller joystick
        mouse->internal_set_axis(0, (mouse_pos.x - vprevmouse.x) * MOUSE_SPEED_MOD / max_screen_size);
        // Y Axis is also negated to match the usual joystick Y axis (negatives values are for the upper direction)
        mouse->internal_set_axis(1,-(mouse_pos.y - vprevmouse.y) * MOUSE_SPEED_MOD / max_screen_size);
        //Pixel movement
        mouse->internal_set_axis(2, (mouse_pos.x - vprevmouse.x));
        mouse->internal_set_axis(3,-(mouse_pos.y - vprevmouse.y));
    }

    if (m_mousecapture)
    {
        mouse_pos = ivec2(m_app * .5f);
        //SDL_WarpMouse((uint16_t)mouse_pos.x, (uint16_t)mouse_pos.y);
    }

    m_prev_mouse_pos = mouse_pos;

#else
    UNUSED(seconds);
#endif //LOL_USE_SDL
}

} /* namespace lol */

