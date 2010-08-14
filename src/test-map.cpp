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

#include "sdlinput.h"
#include "debugfps.h"
#include "game.h"
#include "ticker.h"
#include "video.h"

int main(int argc, char **argv)
{
    /* Initialise SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Cannot initialise SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Surface *video = SDL_SetVideoMode(640, 480, 0, SDL_OPENGL);
    if (!video)
    {
        fprintf(stderr, "Cannot create OpenGL screen: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_WM_SetCaption("Deus Hax (SDL)", NULL);
    SDL_ShowCursor(0);
    SDL_WM_GrabInput(SDL_GRAB_ON);

    /* Initialise timer */
    Uint32 ticks = SDL_GetTicks();

    /* Initialise OpenGL */
    Video::Setup(video->w, video->h);

    /* Create a game */
    Game *game = new Game("maps/testmap.tmx");

    /* Register an input driver and some debug stuff */
    new SdlInput(game);
    new DebugFps();

    while (!game->Finished())
    {
        /* Compute delta time */
        Uint32 newticks = SDL_GetTicks();
        float delta_time = (float)(newticks - ticks);
        ticks = newticks;

        /* Tick the game */
        Ticker::TickGame(delta_time);

        /* Clear the screen, tick the renderer, and show the frame */
        Video::Clear();
        Ticker::TickRender(delta_time);
        SDL_GL_SwapBuffers();

        /* Clamp to desired framerate */
        while (SDL_GetTicks() < ticks + (33.33333f - 0.5f))
            SDL_Delay(1);
    }

    SDL_Quit();

    return EXIT_SUCCESS;
}

