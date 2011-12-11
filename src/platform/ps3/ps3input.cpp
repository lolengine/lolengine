//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
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
  : data(new Ps3InputData())
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
    data->circle_validates =
            (ret == CELL_OK && tmp == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CIRCLE);

    for (int i = 0; i < NUM_PADS; i++)
        for (int j = 0; j < 4; j++)
            cellPadFilterIIRInit(&data->filter_sos[i][j],
                                 CELL_PADFILTER_IIR_CUTOFF_2ND_LPF_BT_010);

    data->mousepos = vec2(320.0f, 240.0f);
    data->mousebuttons = ivec3(0, 0, 0);

    m_gamegroup = GAMEGROUP_BEFORE;
#endif
}

void Ps3Input::TickGame(float deltams)
{
    Entity::TickGame(deltams);

#if defined __CELLOS_LV2__
    CellPadInfo2 pad_info2;
    int32_t ret = cellPadGetInfo2(&pad_info2);
    if (ret != CELL_PAD_OK)
        return;

    for (int i = 0; i < NUM_PADS; i++)
    {
        if (!(pad_info2.port_status[i] & CELL_PAD_STATUS_CONNECTED))
            continue;

        /* Get Pad status. If the data hasn't changed since the last call,
         * data->pad[i].len will be 0 but we carry on anyway. */
        ret = cellPadGetData(i, &data->pad_data[i]);
        if (ret != CELL_PAD_OK)
            continue;

        /* Right stick moves the mouse */
        if (!(pad_info2.system_info & CELL_PAD_INFO_INTERCEPTED))
        {
            int x = data->pad_data[i].button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X];
            int y = data->pad_data[i].button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X + 1];
            vec2 delta(4e-3f * (abs(x - 127) < 16 ? 0 : x - 127),
                       -4e-3f * (abs(y - 127) < 16 ? 0 : y - 127));
            data->mousepos += delta * deltams;
            Input::SetMousePos((ivec2)data->mousepos);
        }

        /* L1 or R1 for mouse button */
        int but = (data->pad_data[i].button[CELL_PAD_BTN_OFFSET_DIGITAL2]
                                                          & CELL_PAD_CTRL_L1)
               || (data->pad_data[i].button[CELL_PAD_BTN_OFFSET_DIGITAL2]
                                                          & CELL_PAD_CTRL_R1);
        if (but && !data->mousebuttons.x)
            Input::SetMouseButton(0);
        else if (!but && data->mousebuttons.x)
            Input::UnsetMouseButton(0);

        data->mousebuttons.x = but;

        /* Only handle the first pad we meet */
        break;
    }
#endif
}

Ps3Input::~Ps3Input()
{
    delete data;
}

} /* namespace lol */

