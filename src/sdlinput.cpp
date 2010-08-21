//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
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
    Game *game;
    int mx, my;
};

/*
 * Public SdlInput class
 */

SdlInput::SdlInput(Game *game)
{
    SDL_Init(SDL_INIT_TIMER);

    data = new SdlInputData();
    data->game = game;
    SDL_GetMouseState(&data->mx, &data->my);
}

Entity::Group SdlInput::GetGroup()
{
    return GROUP_BEFORE;
}

void SdlInput::TickGame(float deltams)
{
    Entity::TickGame(deltams);

    if (data->game->Finished())
        destroy = 1;

    /* Handle mouse input */
    SDL_GetMouseState(&data->mx, &data->my);

    /* Handle keyboard and WM events */
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            data->game->Quit();
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

