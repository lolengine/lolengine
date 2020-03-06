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

#if LOL_USE_SDL
#   if HAVE_SDL2_SDL_H
#      include <SDL2/SDL.h>
#   elif HAVE_SDL_H
#      include <SDL.h>
#   endif
#endif

namespace lol
{

namespace sdl
{

class app
{
public:
    app(char const *title, ivec2 res, float fps);
    virtual ~app();

    void ShowPointer(bool show);
    void Tick();
};

class app_display
{
    friend class lol::ApplicationDisplay;

public:
    app_display(char const *title, ivec2 resolution);
    virtual ~app_display();

protected:
    virtual void set_resolution(ivec2 resolution);
    virtual ivec2 resolution() const;

    void SetPosition(ivec2 position);

    void Enable();
    void Disable();

private:
#if LOL_USE_SDL
    SDL_Window *m_window;
    SDL_GLContext m_glcontext;
#endif
};

} /* namespace sdl */

} /* namespace lol */

