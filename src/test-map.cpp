// Test stuff

#include <SDL.h>

#include <stdio.h>
#include <math.h>

#include "video.h"
#include "game.h"

int main(int argc, char **argv)
{
    Video *video = new Video("Deus Hax", 640, 480);
    Game *game = new Game("maps/testmap-library.tmx");

    for (int done = 0; !done; )
    {
        video->Clear();

        /* Test stuff */
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        game->SetMouse(mx * (640 - 32) / 640, my * (480 - 32) / 480);

        game->Render();
        video->Refresh(33.33333f);

        /* This could go in a separate function */
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                done = 1;
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN)
                    video->FullScreen();
                else if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = 1;
            }
        }
    }

    delete game;
    delete video;

    return EXIT_SUCCESS;
}

