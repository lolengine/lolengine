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

#include <cstdlib>

#if defined __CELLOS_LV2__
#   include <cell/pad.h>
#   include <cell/padfilter.h>
#   include <sysutil/sysutil_sysparam.h>
#endif

#include "core.h"
#include "ps3input.h"

using namespace std;

namespace lol
{

static int const NUM_PADS = 7; /* CellPadUtil also has 7 */

/*
 * PS3 Input implementation class
 */

class Ps3InputData
{
    friend class Ps3Input;

#if defined __CELLOS_LV2__
    Array<int, Stick *> m_joysticks;

    vec2 mousepos;
    ivec3 mousebuttons;

    CellPadData pad_data[NUM_PADS];
    CellPadFilterIIRSos filter_sos[NUM_PADS][4];
    bool circle_validates;
#endif
};

/*
 * Public Ps3Input class
 */

Ps3Input::Ps3Input()
  : m_data(new Ps3InputData())
{
#if defined __CELLOS_LV2__
    int32_t ret = cellPadInit(NUM_PADS);
    if (ret != CELL_OK && ret != CELL_PAD_ERROR_ALREADY_INITIALIZED)
    {
        Log::Error("could not initialise PS3 pad library\n");
        exit(1);
    }

    int tmp;
    ret = cellSysutilGetSystemParamInt(
                        CELL_SYSUTIL_SYSTEMPARAM_ID_ENTER_BUTTON_ASSIGN, &tmp);
    m_data->circle_validates =
            (ret == CELL_OK && tmp == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CIRCLE);

    for (int i = 0; i < NUM_PADS; i++)
        for (int j = 0; j < 4; j++)
            cellPadFilterIIRInit(&m_data->filter_sos[i][j],
                                 CELL_PADFILTER_IIR_CUTOFF_2ND_LPF_BT_010);

    /* Try to detect connected pads. We should do this each frame. */
    CellPadInfo2 pad_info2;
    ret = cellPadGetInfo2(&pad_info2);
    if (ret == CELL_PAD_OK)
    {
        for (int i = 0; i < NUM_PADS; i++)
        {
            if (!(pad_info2.port_status[i] & CELL_PAD_STATUS_CONNECTED))
                continue;

            Stick *stick = Input::CreateStick();
            stick->SetAxisCount(4);
            stick->SetButtonCount(16);
            m_data->m_joysticks.Push(i, stick);
        }
    }

    m_data->mousepos = vec2(320.0f, 240.0f);
    m_data->mousebuttons = ivec3(0, 0, 0);

    m_gamegroup = GAMEGROUP_BEFORE;
#endif
}

void Ps3Input::TickGame(float seconds)
{
    Entity::TickGame(seconds);

#if defined __CELLOS_LV2__
    CellPadInfo2 pad_info2;
    int32_t ret = cellPadGetInfo2(&pad_info2);
    if (ret != CELL_PAD_OK)
        return;

    for (int i = 0; i < m_data->m_joysticks.Count(); i++)
    {
        int j = m_data->m_joysticks[i].m1;

        if (!(pad_info2.port_status[j] & CELL_PAD_STATUS_CONNECTED))
            continue;

        /* Propagate all buttons */
        uint16_t bitmap1 = m_data->pad_data[j].button[CELL_PAD_BTN_OFFSET_DIGITAL1];
        for (int k = 0; k < 8; k++)
            m_data->m_joysticks[i].m2->SetButton(k, bitmap1 & (1 << k));
        uint16_t bitmap2 = m_data->pad_data[j].button[CELL_PAD_BTN_OFFSET_DIGITAL2];
        for (int k = 0; k < 8; k++)
            m_data->m_joysticks[i].m2->SetButton(8 + k, bitmap2 & (1 << k));

        /* Get Pad status. If the data hasn't changed since the last call,
         * data->pad[j].len will be 0 but we carry on anyway. */
        ret = cellPadGetData(j, &m_data->pad_data[j]);
        if (ret != CELL_PAD_OK)
            continue;

        if (!(pad_info2.system_info & CELL_PAD_INFO_INTERCEPTED))
        {
            int x = m_data->pad_data[j].button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X];
            int y = m_data->pad_data[j].button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X + 1];
            int x2 = m_data->pad_data[j].button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X];
            int y2 = m_data->pad_data[j].button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X + 1];

            /* Propagate all axes */
            m_data->m_joysticks[i].m2->SetAxis(0, (x - 127) / 127.0f);
            m_data->m_joysticks[i].m2->SetAxis(1, (y - 127) / 127.0f);
            m_data->m_joysticks[i].m2->SetAxis(2, (x2 - 127) / 127.0f);
            m_data->m_joysticks[i].m2->SetAxis(3, (y2 - 127) / 127.0f);

            /* Right stick moves the mouse FIXME: deprecated */
            vec2 delta(4.f * (abs(x - 127) < 16 ? 0 : x - 127),
                       -4.f * (abs(y - 127) < 16 ? 0 : y - 127));
            m_data->mousepos += delta * seconds;
            Input::SetMousePos((ivec2)m_data->mousepos);
        }

        /* L1 or R1 for mouse button FIXME: deprecated */
        int but = (m_data->pad_data[j].button[CELL_PAD_BTN_OFFSET_DIGITAL2]
                                                          & CELL_PAD_CTRL_L1)
               || (m_data->pad_data[j].button[CELL_PAD_BTN_OFFSET_DIGITAL2]
                                                          & CELL_PAD_CTRL_R1);
        if (but && !m_data->mousebuttons.x)
            Input::SetMouseButton(0);
        else if (!but && m_data->mousebuttons.x)
            Input::UnsetMouseButton(0);

        m_data->mousebuttons.x = but;
    }
#endif
}

Ps3Input::~Ps3Input()
{
#if defined __CELLOS_LV2__
    for (int i = 0; i < m_data->m_joysticks.Count(); ++i)
    {
        Input::DestroyStick(m_data->m_joysticks[0].m2);
    }
#endif
    delete m_data;
}

} /* namespace lol */

