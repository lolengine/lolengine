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

static int const FPS = 30.0f;

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

    /* Initialise OpenGL */
    Video::Setup(video->w, video->h);

    /* Create a game */
    Game *game = new Game("maps/testmap.tmx");

    /* Register an input driver and some debug stuff */
    new SdlInput(game);
    new DebugFps();

    while (!game->Finished())
    {
        /* Tick the game */
        Ticker::TickGame();

        /* Clear the screen, tick the renderer, and show the frame */
        Video::Clear();
        Ticker::TickRender();
        SDL_GL_SwapBuffers();

        /* Clamp to desired framerate */
        Ticker::ClampFps(FPS);
    }

    SDL_Quit();

    return EXIT_SUCCESS;
}

