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

#ifdef LOL_INPUT_V2
#include "input/inputdevice_internal.h"
#endif // LOL_INPUT_V2

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
#if defined LOL_INPUT_V2
    Array<int, InputDeviceInternal*> m_joysticks;
#else
    Array<int, Stick *> m_joysticks;
#endif // LOL_INPUT_V2
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
#if defined LOL_INPUT_V2
		// TODO: we can put more friendly name here, such as LeftAxisX, ButtonX...
		InputDeviceInternal* stick = new InputDeviceInternal(String::Printf("Joystick%d", i+1).C());
		for (int j = 0; j < 4; ++j)
			stick->AddAxis(String::Printf("Axis%d", j+1).C());
		for (int j = 0; j < 16; ++j)
			stick->AddKey(String::Printf("Button%d", j+1).C());

		m_data->m_joysticks.Push(i, stick);
#else
        Stick *stick = Input::CreateStick();
        stick->SetAxisCount(4);
        stick->SetButtonCount(16);
        m_data->m_joysticks.Push(i, stick);
#endif // LOL_INPUT_V2
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
#if defined LOL_INPUT_V2
		delete m_data->m_joysticks[0].m2;
#else
		Input::DestroyStick(m_data->m_joysticks[0].m2);
#endif // LOL_INPUT_V2
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
#if defined LOL_INPUT_V2
			m_data->m_joysticks[i].m2->SetKey(b, ((uint16_t)(state.Gamepad.wButtons) >> b) & 1);
#else
			m_data->m_joysticks[i].m2->SetButton(b, ((uint16_t)(state.Gamepad.wButtons) >> b) & 1);
#endif // LOL_INPUT_V2
	}
#endif
}

} /* namespace lol */

