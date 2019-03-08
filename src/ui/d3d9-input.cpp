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

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined LOL_USE_XINPUT
#   include <d3d9.h>
#   include <xinput.h>
#endif

#include <memory>

#include <lol/engine-internal.h>

#include "ui/d3d9-input.h"
#include "ui/input.h"

namespace lol
{

/*
 * D3d9 Input implementation class
 */

class D3d9InputData
{
    friend class D3d9Input;

private:
#if defined LOL_USE_XINPUT
    array<int, std::shared_ptr<input::device::joystick>> m_joysticks;
#endif // LOL_USE_XINPUT
};

/*
 * Public D3d9Input class
 */

D3d9Input::D3d9Input()
  : m_data(new D3d9InputData())
{
#if defined LOL_USE_XINPUT
    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE state;
        if (XInputGetState(i, &state) != ERROR_SUCCESS)
            continue;

        auto stick = input::joystick(i);

        stick->internal_add_axis(input::axis::LeftX, "LeftX");
        stick->internal_add_axis(input::axis::LeftY, "LeftY");
        stick->internal_add_axis(input::axis::RightX, "RightX");
        stick->internal_add_axis(input::axis::RightY, "RightY");
        stick->internal_add_axis(input::axis::LeftShoulder, "LeftShoulder");
        stick->internal_add_axis(input::axis::RightShoulder, "RightShoulder");

        #define _BTN(id, name) stick->internal_add_button(input::button::BTN_##name, #name);
        #include "ui/buttons.inc" // FIXME: ignore mouse buttons here

        m_data->m_joysticks.push(i, stick);
    }
#endif

    m_gamegroup = tickable::group::game::input;
}

D3d9Input::~D3d9Input()
{
#if defined LOL_USE_XINPUT
    /* Unregister all the joysticks we added (FIXME: this code seems unnecessary) */
    while (m_data->m_joysticks.count())
        m_data->m_joysticks.remove(0);
#endif
    delete m_data;
}

void D3d9Input::tick_game(float seconds)
{
    entity::tick_game(seconds);

#if defined LOL_USE_XINPUT
    for (int i = 0; i < m_data->m_joysticks.count(); i++)
    {
        XINPUT_STATE state;
        if (XInputGetState(m_data->m_joysticks[i].m1, &state) != ERROR_SUCCESS)
            continue;

        auto stick = m_data->m_joysticks[i].m2;
        stick->internal_begin_frame();
        stick->internal_set_axis(input::axis::LeftX, state.Gamepad.sThumbLX / 32768.f);
        stick->internal_set_axis(input::axis::LeftY, -state.Gamepad.sThumbLY / 32768.f);
        stick->internal_set_axis(input::axis::RightX, state.Gamepad.sThumbRX / 32768.f);
        stick->internal_set_axis(input::axis::RightY, -state.Gamepad.sThumbRY / 32768.f);
        stick->internal_set_axis(input::axis::LeftShoulder, state.Gamepad.bLeftTrigger / 32768.f);
        stick->internal_set_axis(input::axis::RightShoulder, state.Gamepad.bRightTrigger / 32768.f);

        for (int b = 0; b < 16; b++)
        {
            // Reserved values
            if ((1 << b) > XINPUT_GAMEPAD_RIGHT_SHOULDER && (1 << b) < XINPUT_GAMEPAD_A)
                continue;

            int key_index = (1 << b) > XINPUT_GAMEPAD_RIGHT_SHOULDER ? b - 2 : b;

            m_data->m_joysticks[i].m2->internal_set_button((input::button)key_index, ((uint16_t)(state.Gamepad.wButtons) >> b) & 1);
        }
    }
#endif
}

void D3d9Input::tick_draw(float seconds, Scene &scene)
{
    entity::tick_draw(seconds, scene);
}

} /* namespace lol */

