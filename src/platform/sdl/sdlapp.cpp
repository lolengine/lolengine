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

#if defined USE_SDL
#   include <SDL.h>
#endif

#include "core.h"
#include "lolgl.h"
#include "platform/sdl/sdlapp.h"
#include "platform/sdl/sdlinput.h"

namespace lol
{

/*
 * SDL App implementation class
 */

class SdlAppData
{
    friend class SdlApp;

private:
    int unused;
};

/*
 * Public SdlApp class
 */

SdlApp::SdlApp(char const *title, ivec2 res, float fps) :
    data(new SdlAppData())
{
#if defined USE_SDL
    /* Initialise SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        Log::Error("cannot initialise SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_Surface *video = SDL_SetVideoMode(res.x, res.y, 0, SDL_OPENGL);
    if (!video)
    {
        Log::Error("cannot create OpenGL screen: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption(title, NULL);

    /* Initialise everything */
    Ticker::Setup(fps);
    Video::Setup(ivec2(video->w, video->h));
    Audio::Setup(2);

    /* Autoreleased objects */
    new SdlInput();
#endif
}

void SdlApp::ShowPointer(bool show)
{
#if defined USE_SDL
    SDL_ShowCursor(show ? 1 : 0);
#endif
}

void SdlApp::Run()
{
    while (!Ticker::Finished())
    {
        /* Tick the game */
        Ticker::TickGame();

        /* Tick the renderer, show the frame and clamp to desired framerate. */
        Ticker::TickDraw();
#if defined USE_SDL
        SDL_GL_SwapBuffers();
#endif
        Ticker::ClampFps();
    }
}

SdlApp::~SdlApp()
{
#if defined USE_SDL
    SDL_Quit();
#endif
    delete data;
}

} /* namespace lol */

