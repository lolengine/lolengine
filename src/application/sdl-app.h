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

#pragma once

//
// The SdlApp class
// ----------------
//

#include <lol/vector>

#include "application/application.h"

#if LOL_USE_SDL
#   if HAVE_SDL2_SDL_H
#      include <SDL2/SDL.h>
#   elif HAVE_SDL_H
#      include <SDL.h>
#   endif
#endif

namespace lol::sdl
{

// Application is its own display
class app : public lol::app,
            public lol::app::display,
            public std::enable_shared_from_this<app>
{
public:
    app(char const *title, ivec2 resolution);
    virtual ~app();

    // App interface
    virtual void show_pointer(bool show);
    virtual void tick();
    virtual std::shared_ptr<display> get_display();

protected:
    // Display interface
    virtual void start_frame();
    virtual void end_frame();

    virtual void set_resolution(ivec2 resolution);
    virtual ivec2 resolution() const;

    virtual void set_position(ivec2 position);

private:
#if LOL_USE_SDL
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    SDL_GLContext m_glcontext;
#endif
};

} // namespace lol::sdl
