//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <SDL.h>

#include "sdlinput.h"

/*
 * SDL Input implementation class
 */

class SdlInputData
{
    friend class SdlInput;

private:
    Game *game;
};

/*
 * Public SdlInput class
 */

SdlInput::SdlInput(Game *game)
{
    SDL_Init(SDL_INIT_TIMER);

    data = new SdlInputData();
    data->game = game;
}

Asset::Group SdlInput::GetGroup()
{
    return GROUP_BEFORE;
}

void SdlInput::TickGame(float delta_time)
{
    Asset::TickGame(delta_time);

    if (data->game->Finished())
        destroy = 1;

    /* Handle mouse input */
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    data->game->SetMouse(mx * (640 - 32) / 640, my * (480 - 32) / 480);

    /* Handle keyboard and WM input */
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            data->game->Quit();
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                data->game->Quit();
#if 0
            else if (event.key.keysym.sym == SDLK_RETURN)
                video->FullScreen();
#endif
        }
    }

}

SdlInput::~SdlInput()
{
    delete data;
}

