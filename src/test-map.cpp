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

#include "core.h"
#include "sdlinput.h"
#include "debugfps.h"
#include "debugsprite.h"
#include "debugrecord.h"

static float const FPS = 30.0f;

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

    SDL_WM_SetCaption("Map Test (SDL)", NULL);
    SDL_ShowCursor(0);
    //SDL_WM_GrabInput(SDL_GRAB_ON);

    /* Initialise OpenGL */
    Video::Setup(video->w, video->h);

    /* Create a game */
    Game *game = new Game("maps/testmap.tmx");

    /* Register an input driver and some debug stuff */
    new SdlInput(game);
    new DebugFps();
    new DebugSprite(game);
    //new DebugRecord("lolengine.ogg");

    while (!game->Finished())
    {
        /* Tick the game */
        Ticker::TickGame();

        /* Clear the screen, tick the renderer, show the frame and
         * clamp to desired framerate. */
        Video::Clear();
        Ticker::TickRender();
        SDL_GL_SwapBuffers();
        Ticker::ClampFps(1000.0f / FPS);
    }

    SDL_Quit();

    return EXIT_SUCCESS;
}

