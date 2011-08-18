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

#if defined __CELLOS_LV2__
#   include <padutil.h>
#endif

#include "core.h"
#include "ps3input.h"

namespace lol
{

/*
 * PS3 Input implementation class
 */

class Ps3InputData
{
    friend class Ps3Input;

    vec2 mousepos;
    vec3i mousebuttons;
};

/*
 * Public Ps3Input class
 */

Ps3Input::Ps3Input()
  : data(new Ps3InputData())
{
#if defined __CELLOS_LV2__
    cellPadUtilPadInit();
    cellPadUtilSetSensorMode(true);
    cellPadUtilSetPressMode(true);
    cellPadUtilSetSensorFilter(CELL_PADFILTER_IIR_CUTOFF_2ND_LPF_BT_010);

    data->mousepos = vec2(320.0f, 240.0f);
    data->mousebuttons = vec3i(0, 0, 0);

    gamegroup = GAMEGROUP_BEFORE;
#endif
}

void Ps3Input::TickGame(float deltams)
{
    Entity::TickGame(deltams);

#if defined __CELLOS_LV2__
    cellPadUtilUpdate();

    int pad = cellPadUtilGetFirstConnectedPad();
    if (pad >= 0)
    {
        CellPadUtilAxis axis = cellPadUtilGetAxisValue(pad, CELL_UTIL_ANALOG_RIGHT);
        vec2 delta(4e-3f * (abs(axis.x - 127) < 16 ? 0 : axis.x - 127),
                   -4e-3f * (abs(axis.y - 127) < 16 ? 0 : axis.y - 127));
        data->mousepos += delta * deltams;
        Input::SetMousePos((vec2i)data->mousepos);

        // L1 for mouse button
        uint32_t paddata = cellPadUtilGetDigitalData(pad);
        int but = cellPadUtilDigitalButtonPressed(paddata, CELL_UTIL_BUTTON_L1)
               || cellPadUtilDigitalButtonPressed(paddata, CELL_UTIL_BUTTON_R1);
        
        if (but && !data->mousebuttons.x)
            Input::SetMouseButton(0);
        else if (!but && data->mousebuttons.x)
            Input::UnsetMouseButton(0);

        data->mousebuttons.x = but;
    }
#endif
}

Ps3Input::~Ps3Input()
{
    delete data;
}

} /* namespace lol */

