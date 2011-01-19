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

#include <SDL.h>

#include "core.h"
#include "sdlinput.h"

/*
 * SDL Input implementation class
 */

class SdlInputData
{
    friend class SdlInput;

private:
    int mx, my;
};

/*
 * Public SdlInput class
 */

SdlInput::SdlInput()
{
    SDL_Init(SDL_INIT_TIMER);

    data = new SdlInputData();
    SDL_GetMouseState(&data->mx, &data->my);

    gamegroup = GAMEGROUP_BEFORE;
}

void SdlInput::TickGame(float deltams)
{
    Entity::TickGame(deltams);

    /* Handle mouse input */
    Int2 mouse;
    if (SDL_GetAppState() & SDL_APPMOUSEFOCUS)
        SDL_GetMouseState(&mouse.x, &mouse.y);
    else
        mouse.x = mouse.y = -1;
    Input::SetMousePos(mouse);

    /* Handle keyboard and WM events */
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            Ticker::Shutdown();
#if 0
        else if (event.type == SDL_KEYDOWN)
            Input::KeyPressed(event.key.keysym.sym, deltams);
#endif
    }

    /* Send the whole keyboard state to the input system */
#if 0
    Uint8 *keystate = SDL_GetKeyState(NULL);
    for (int i = 0; i < 256; i++)
        if (keystate[i])
            Input::KeyPressed(i, deltams);
#endif
}

SdlInput::~SdlInput()
{
    delete data;
}

