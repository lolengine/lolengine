//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined USE_XINPUT
#   include <d3d9.h>
#   include <xinput.h>
#endif

#include "core.h"
#include "d3d9input.h"

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
    Array<int, Stick *> m_joysticks;
#endif
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

        Stick *stick = Input::CreateStick();
        stick->SetAxisCount(4);
        stick->SetButtonCount(16);
        m_data->m_joysticks.Push(i, stick);
    }
#endif

    m_gamegroup = GAMEGROUP_BEFORE;
}

D3d9Input::~D3d9Input()
{
#if defined USE_XINPUT
    /* Unregister all the joysticks we added */
    while (m_data->m_joysticks.Count())
    {
        Input::DestroyStick(m_data->m_joysticks[0].m2);
        m_data->m_joysticks.Remove(0);
    }
#endif
    delete m_data;
}

void D3d9Input::TickGame(float seconds)
{
    Entity::TickGame(seconds);
}

void D3d9Input::TickDraw(float seconds)
{
    Entity::TickDraw(seconds);

#if defined USE_XINPUT
    for (int i = 0; i < m_data->m_joysticks.Count(); i++)
    {
        XINPUT_STATE state;
        if (XInputGetState(m_data->m_joysticks[i].m1, &state) != ERROR_SUCCESS)
            continue;

        m_data->m_joysticks[i].m2->SetAxis(0, (float)state.Gamepad.sThumbLX / 32768.f);
        m_data->m_joysticks[i].m2->SetAxis(1, -(float)state.Gamepad.sThumbLY / 32768.f);
        m_data->m_joysticks[i].m2->SetAxis(2, (float)state.Gamepad.sThumbRX / 32768.f);
        m_data->m_joysticks[i].m2->SetAxis(3, -(float)state.Gamepad.sThumbRY / 32768.f);

        for (int b = 0; b < 16; b++)
            m_data->m_joysticks[i].m2->SetButton(b, ((uint16_t)(state.Gamepad.wButtons) >> b) & 1);
    }
#endif
}

} /* namespace lol */

