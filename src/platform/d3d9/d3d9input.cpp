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
#if defined USE_XINPUT
#   include <d3d9.h>
#   include <xinput.h>
#endif

#include <lol/engine-internal.h>

#include "d3d9input.h"

#include "input/input_internal.h"

namespace lol
{

/*
 * D3d9 Input implementation class
 */

class D3d9InputData
{
    friend class D3d9Input;

private:
#if defined USE_XINPUT
    array<int, InputDeviceInternal*> m_joysticks;
#endif // USE_XINPUT
};

/*
 * Public D3d9Input class
 */

D3d9Input::D3d9Input()
  : m_data(new D3d9InputData())
{
#if defined USE_XINPUT
    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE state;
        if (XInputGetState(i, &state) != ERROR_SUCCESS)
            continue;
        // TODO: we can put more friendly name here, such as LeftAxisX, ButtonX...
        InputDeviceInternal* stick = new InputDeviceInternal(String::format("Joystick%d", i+1).C());

        stick->AddAxis(g_name_xbox_axis_left_x.C());
        stick->AddAxis(g_name_xbox_axis_left_y.C());
        stick->AddAxis(g_name_xbox_axis_right_x.C());
        stick->AddAxis(g_name_xbox_axis_right_y.C());
        stick->AddAxis(g_name_xbox_axis_left_trigger.C());
        stick->AddAxis(g_name_xbox_axis_right_trigger.C());

        stick->AddKey(g_name_xbox_key_dpad_up.C());
        stick->AddKey(g_name_xbox_key_dpad_down.C());
        stick->AddKey(g_name_xbox_key_dpad_left.C());
        stick->AddKey(g_name_xbox_key_dpad_right.C());
        stick->AddKey(g_name_xbox_key_start.C());
        stick->AddKey(g_name_xbox_key_back.C());
        stick->AddKey(g_name_xbox_key_left_thumb.C());
        stick->AddKey(g_name_xbox_key_right_thumb.C());
        stick->AddKey(g_name_xbox_key_left_shoulder.C());
        stick->AddKey(g_name_xbox_key_right_shoulder.C());
        stick->AddKey(g_name_xbox_key_a.C());
        stick->AddKey(g_name_xbox_key_b.C());
        stick->AddKey(g_name_xbox_key_x.C());
        stick->AddKey(g_name_xbox_key_y.C());

        m_data->m_joysticks.push(i, stick);
    }
#endif

    m_gamegroup = GAMEGROUP_INPUT;
}

D3d9Input::~D3d9Input()
{
#if defined USE_XINPUT
    /* Unregister all the joysticks we added */
    while (m_data->m_joysticks.count())
    {
        delete m_data->m_joysticks[0].m2;
        m_data->m_joysticks.remove(0);
    }
#endif
    delete m_data;
}

void D3d9Input::TickGame(float seconds)
{
    Entity::TickGame(seconds);

#if defined USE_XINPUT
    for (int i = 0; i < m_data->m_joysticks.count(); i++)
    {
        XINPUT_STATE state;
        if (XInputGetState(m_data->m_joysticks[i].m1, &state) != ERROR_SUCCESS)
            continue;

        m_data->m_joysticks[i].m2->SetAxis(0, (float)state.Gamepad.sThumbLX / 32768.f);
        m_data->m_joysticks[i].m2->SetAxis(1, -(float)state.Gamepad.sThumbLY / 32768.f);
        m_data->m_joysticks[i].m2->SetAxis(2, (float)state.Gamepad.sThumbRX / 32768.f);
        m_data->m_joysticks[i].m2->SetAxis(3, -(float)state.Gamepad.sThumbRY / 32768.f);
        m_data->m_joysticks[i].m2->SetAxis(4, (float)state.Gamepad.bLeftTrigger / 32768.f);
        m_data->m_joysticks[i].m2->SetAxis(5, (float)state.Gamepad.bRightTrigger / 32768.f);

        for (int b = 0; b < 16; b++)
        {
            // Reserved values
            if ((1 << b) > XINPUT_GAMEPAD_RIGHT_SHOULDER && (1 << b) < XINPUT_GAMEPAD_A)
                continue;

            int key_index = (1 << b) > XINPUT_GAMEPAD_RIGHT_SHOULDER ? b - 2 : b;

            m_data->m_joysticks[i].m2->SetKey(key_index, ((uint16_t)(state.Gamepad.wButtons) >> b) & 1);
        }
    }
#endif
}

void D3d9Input::TickDraw(float seconds, Scene &scene)
{
    Entity::TickDraw(seconds, scene);
}

} /* namespace lol */

