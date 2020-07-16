//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>
#include <lol/msg>

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
* Public sdl::app_display class
*/
sdl::app::app(char const *title, ivec2 res)
{
#if LOL_USE_SDL
    ivec2 window_size = res;

    // Initialise SDL
    if (!SDL_WasInit(0))
    {
        msg::debug("initialising SDL\n");
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0)
        {
            msg::error("cannot initialise SDL: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }

    msg::debug("found %d displays\n", SDL_GetNumVideoDisplays());
    for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
        msg::debug("%d: %s\n", i, SDL_GetDisplayName(i));

    int flags = SDL_WINDOW_RESIZABLE;

    if (engine::has_opengl())
    {
        flags |= SDL_WINDOW_OPENGL;

        // This seems to fix a bug we used to have at context swap. Maybe remove one day.
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

#if !defined __EMSCRIPTEN__ && !defined HAVE_GLES_2X
        // Ask for GL 3.2 at least
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#if __APPLE__
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

#if LOL_BUILD_DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
#endif
    }

    if (window_size == ivec2(0))
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    msg::debug("initialising main window\n");
    m_window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_size.x, window_size.y, flags);
    if (!m_window)
    {
        msg::error("cannot create rendering window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    SDL_GetWindowSize(m_window, &res.x, &res.y);

    // Enable drag-and-drop
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

    if (engine::has_opengl())
    {
        m_glcontext = SDL_GL_CreateContext(m_window);
        SDL_GL_MakeCurrent(m_window, m_glcontext);
        msg::info("created GL context: %s\n", glGetString(GL_VERSION));

        // Initialise everything
        video::init(res); //TODO ?? Should it be here ?
    }

#if defined LOL_USE_XINPUT
    /* Prefer D3d9 for joysticks on Windows, because the X360 pads are not
     * advertised with the proper number of axes. */
    new D3d9Input();
#endif

    new SdlInput(res.x, res.y);

    audio::init();
#endif
}

sdl::app::~app()
{
#if LOL_USE_SDL
    if (m_window)
    {
        if (engine::has_opengl())
            SDL_GL_DeleteContext(m_glcontext);

        SDL_DestroyWindow(m_window);
    }

    SDL_Quit();
#endif
}

std::shared_ptr<app::display> sdl::app::get_display()
{
    return shared_from_this();
}

ivec2 sdl::app::resolution() const
{
    ivec2 ret(0);
#if LOL_USE_SDL
    SDL_GetWindowSize(m_window, &ret.x, &ret.y);
#endif
    return ret;
}

void sdl::app::set_resolution(ivec2 resolution)
{
#if LOL_USE_SDL
    SDL_SetWindowSize(m_window, resolution.x, resolution.y);
#endif
}

void sdl::app::set_position(ivec2 position)
{
#if LOL_USE_SDL
    SDL_SetWindowPosition(m_window, position.x, position.y);
#endif
}

void sdl::app::start_frame()
{
#if LOL_USE_SDL
    //TODO: Should we do that: ?
    if (engine::has_opengl())
        SDL_GL_MakeCurrent(m_window, m_glcontext);
#endif
}

void sdl::app::end_frame()
{
#if LOL_USE_SDL
    if (engine::has_opengl())
        SDL_GL_SwapWindow(m_window);
#endif
}

void sdl::app::show_pointer(bool show)
{
#if LOL_USE_SDL
    SDL_ShowCursor(show ? 1 : 0);
#endif
}

void sdl::app::tick()
{
    // Tick the renderer, show the frame and clamp to desired framerate.
    ticker::tick_draw();
}

} // namespace lol
