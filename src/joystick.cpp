//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <SDL.h>

#include "joystick.h"

/*
 * Joystick implementation class
 */

class JoystickData
{
    friend class Joystick;

private:
    int dummy;
};

/*
 * Public Joystick class
 */

Joystick::Joystick()
{
    data = new JoystickData();

    SDL_WM_GrabInput(SDL_GRAB_ON);
}

Joystick::~Joystick()
{
}

