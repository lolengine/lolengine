// Test stuff

#include <SDL.h>

#include <stdio.h>
#include <math.h>

#include "video.h"
#include "tiler.h"
#include "map.h"

int main(int argc, char **argv)
{
    Video *video = new Video("Deus Hax", 640, 480);
    Map *map = new Map("maps/testmap-grass.tmx");

    for (int done = 0; !done; )
    {
        video->Clear();

        //map->Draw(tiler);

        /* Test stuff */
        int playerx, playery;
        SDL_GetMouseState(&playerx, &playery);
        playerx = playerx * (640 - 32) / 640;
        playery = playery * (480 - 32) / 480;

        //tiler->AddTile(50, playerx, playery, 1);

        //tiler->Render();
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

    delete map;
    delete video;

    return EXIT_SUCCESS;
}

