//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if LOL_USE_SDL
#   if HAVE_SDL2_SDL_H
#      include <SDL2/SDL.h>
#   elif HAVE_SDL_H
#      include <SDL.h>
#   endif
#endif

#include "lolgl.h"
#include "application/sdl-app.h"
#include "ui/sdl-input.h"
#if LOL_USE_XINPUT
#   include "ui/d3d9-input.h"
#endif

namespace lol
{

/*
* SDL App display implementation class
*/
class SdlAppDisplayData
{
    friend class SdlAppDisplay;

private:
#if LOL_USE_SDL
    SDL_Window *m_window;
    SDL_GLContext m_glcontext;
#endif
};

/*
* Public SdlApp class
*/
SdlAppDisplay::SdlAppDisplay(char const *title, ivec2 res)
    : data(new SdlAppDisplayData())
{
#if LOL_USE_SDL
    ivec2 window_size = res;

    /* Initialise SDL */
    if (!SDL_WasInit(0))
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0)
        {
            msg::error("cannot initialise SDL: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }

    /* This seems to fix the swap context bug.
     * However, perfs warning have been may occur. */
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

    int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    if (window_size == ivec2(0))
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    data->m_window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_size.x, window_size.y, flags);
    if (!data->m_window)
    {
        msg::error("cannot create rendering window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    SDL_GetWindowSize(data->m_window, &res.x, &res.y);

    data->m_glcontext = SDL_GL_CreateContext(data->m_window);

    /* Initialise everything */
    Video::Setup(res); //TODO ?? Should it be here ?
#endif
}

SdlAppDisplay::~SdlAppDisplay()
{
#if LOL_USE_SDL
    if (data->m_window)
    {
        SDL_GL_DeleteContext(data->m_glcontext);
        SDL_DestroyWindow(data->m_window);
    }
#endif

    delete data;
}

ivec2 SdlAppDisplay::resolution() const
{
    ivec2 ret(0);
#if LOL_USE_SDL
    SDL_GetWindowSize(data->m_window, &ret.x, &ret.y);
#endif
    return ret;
}

void SdlAppDisplay::set_resolution(ivec2 resolution)
{
#if LOL_USE_SDL
    SDL_SetWindowSize(data->m_window, resolution.x, resolution.y);
#endif
}

void SdlAppDisplay::SetPosition(ivec2 position)
{
#if LOL_USE_SDL
    SDL_SetWindowPosition(data->m_window, position.x, position.y);
#endif
}

void SdlAppDisplay::Enable()
{
#if LOL_USE_SDL
    //TODO: Should we do that: ?
    SDL_GL_MakeCurrent(data->m_window, data->m_glcontext);
#endif
}

void SdlAppDisplay::Disable()
{
#if LOL_USE_SDL
        SDL_GL_SwapWindow(data->m_window);
#endif
}

#if LOL_USE_SDL
int SceneDisplay::GetPhysicalCount()
{
    return SDL_GetNumVideoDisplays();
}

const char* SceneDisplay::GetPhysicalName(int index)
{
    return SDL_GetDisplayName(index);
}
#endif

/*
 * SDL App implementation class
 */
class SdlAppData
{
    friend class SdlApp;

private:
#if LOL_USE_SDL
    SDL_Window *m_window;
    SDL_GLContext m_glcontext;
#endif
};

/*
 * Public SdlApp class
 */
SdlApp::SdlApp(char const *title, ivec2 res, float fps) :
    data(new SdlAppData())
{
    UNUSED(title);
#if LOL_USE_SDL
    ivec2 window_size = res;
    ivec2 screen_size = res;

    /* Initialise everything */
    ticker::setup(fps);
    audio::init();

    /* Autoreleased objects */
#if defined LOL_USE_XINPUT
    /* Prefer D3d9 for joysticks on Windows, because the X360 pads are not
     * advertised with the proper number of axes. */
    new D3d9Input();
#endif

    new SdlInput(res.x, res.y, screen_size.x, screen_size.y);
#endif
}

void SdlApp::ShowPointer(bool show)
{
#if LOL_USE_SDL
    SDL_ShowCursor(show ? 1 : 0);
#endif
}

void SdlApp::Tick()
{
    /* Tick the renderer, show the frame and clamp to desired framerate. */
    Ticker::tick_draw();
}

SdlApp::~SdlApp()
{
#if LOL_USE_SDL
    SDL_Quit();
#endif
    delete data;
    ticker::teardown();
}

} /* namespace lol */

