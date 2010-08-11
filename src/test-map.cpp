//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cmath>

#include <SDL.h>

#include "sdlvideo.h"
#include "game.h"
#include "ticker.h"

int main(int argc, char **argv)
{
    Video *video = new SdlVideo("Deus Hax", 640, 480);
    Game *game = new Game("maps/testmap.tmx");

    while (!game->Finished())
    {
        /* Test stuff */
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        game->SetMouse(mx * (640 - 32) / 640, my * (480 - 32) / 480);

        Ticker::TickGame(33.33333f);

        video->PreRender();
        Ticker::TickRender(33.33333f);
        game->Render();
        video->PostRender(33.33333f);

        /* This could go in a separate function */
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                game->Quit();
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN)
                    video->FullScreen();
                else if (event.key.keysym.sym == SDLK_ESCAPE)
                    game->Quit();
            }
        }
    }

    delete game;
    delete video;

    return EXIT_SUCCESS;
}

