//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

/* This instructs our headers to let SDL override the "main"
 * symbol using its macros. */
#define LOL_DONT_DIVERT_MAIN 1

#if defined USE_SDL
#   if defined HAVE_SDL_SDL_H
#      include <SDL/SDL.h>
#   else
#      include <SDL.h>
#   endif
#   if defined USE_D3D9
#       include <d3d9.h>
#       include <SDL_syswm.h>
#   endif
#endif

#include "core.h"
#include "lolgl.h"
#include "platform/sdl/sdlapp.h"
#include "platform/sdl/sdlinput.h"
#if defined USE_XINPUT
#   include "platform/d3d9/d3d9input.h"
#endif

#if defined USE_SDL && defined USE_D3D9
HWND g_hwnd = NULL;
extern IDirect3DDevice9 *g_d3ddevice;
#endif

#if defined main
#   if defined _MSC_VER
int lol_sdl_main();
int lol_sdl_main(int argc, char **argv);
int lol_sdl_main(int argc, char **argv, char **envp);
#       define WRAPPER lol_sdl_main_msvc
#   else
int lol_sdl_main() __attribute__((weak));
int lol_sdl_main(int argc, char **argv) __attribute__((weak));
int lol_sdl_main(int argc, char **argv, char **envp) __attribute__((weak));
#       define WRAPPER lol_sdl_main
#   endif

/* One of these wrappers will be overridden by the user's version */

int WRAPPER() { return 0; }
int WRAPPER(int argc, char **argv) { return 0; }
int WRAPPER(int argc, char **argv, char **envp) { return 0; }

int main(int argc, char *argv[])
{
    int ret = 0;
    ret += lol_sdl_main();
    ret += lol_sdl_main(argc, argv);
    ret += lol_sdl_main(argc, argv, NULL);
    return ret;
}
#endif

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

#   if defined USE_D3D9
    SDL_Surface *video = SDL_SetVideoMode(res.x, res.y, 16, 0);
    SDL_SysWMinfo wminfo;
    SDL_VERSION(&wminfo.version);
    SDL_GetWMInfo(&wminfo);
    g_hwnd = wminfo.window;
#   else
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_Surface *video = SDL_SetVideoMode(res.x, res.y, 0, SDL_OPENGL);
#   endif
    if (!video)
    {
        Log::Error("cannot create rendering window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption(title, NULL);

    /* Initialise everything */
    Ticker::Setup(fps);
    Video::Setup(ivec2(video->w, video->h));
    Audio::Setup(2);

    /* Autoreleased objects */
#   if defined USE_XINPUT
    /* Prefer D3d9 for joysticks on Windows, because the X360 pads are not
     * advertised with the proper number of axes. */
    new D3d9Input();
#   endif
    new SdlInput();
#endif
}

void SdlApp::ShowPointer(bool show)
{
#if defined USE_SDL
    SDL_ShowCursor(show ? 1 : 0);
#endif
}

void SdlApp::Tick()
{
#if defined USE_SDL && defined USE_D3D9
    HRESULT hr;
    hr = g_d3ddevice->BeginScene();
    if (FAILED(hr))
        Abort();
#endif
    /* Tick the renderer, show the frame and clamp to desired framerate. */
    Ticker::TickDraw();
#if defined USE_SDL
#   if defined USE_D3D9
    hr = g_d3ddevice->EndScene();
    if (FAILED(hr))
        Abort();
    hr = g_d3ddevice->Present(NULL, NULL, NULL, NULL);
    if (FAILED(hr))
        Abort();
#   else
    SDL_GL_SwapBuffers();
#   endif
#endif
}

SdlApp::~SdlApp()
{
#if defined USE_SDL
    SDL_Quit();
#endif
    delete data;
}

} /* namespace lol */

