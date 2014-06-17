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

#if defined _XBOX
#   include <xtl.h>
#   include <xbdm.h>
#endif

#include "core.h"
#include "xboxinput.h"

namespace lol
{

/*
 * Xbox Input implementation class
 */

class XboxInputData
{
    friend class XboxInput;

private:
#if defined _XBOX
    Array<int, Stick *> m_joysticks;
#endif
};

/*
 * Public XboxInput class
 */

XboxInput::XboxInput()
  : m_data(new XboxInputData())
{
#if defined _XBOX
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

XboxInput::~XboxInput()
{
#if defined _XBOX
    /* Unregister all the joysticks we added */
    while (m_data->m_joysticks.Count())
    {
        Input::DestroyStick(m_data->m_joysticks[0].m2);
        m_data->m_joysticks.Remove(0);
    }
#endif
    delete m_data;
}

void XboxInput::TickGame(float seconds)
{
    Entity::TickGame(seconds);
}

void XboxInput::TickDraw(float seconds, Scene &scene)
{
    Entity::TickDraw(seconds, scene);

#if defined _XBOX
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

