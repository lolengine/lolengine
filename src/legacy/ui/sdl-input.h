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
// The SdlInput class
// ------------------
//

#include "engine/entity.h"

#if LOL_USE_SDL
#   if HAVE_SDL2_SDL_H
#      include <SDL2/SDL.h>
#   elif HAVE_SDL_H
#      include <SDL.h>
#   endif
#endif

#include <memory>
#include <vector>
#include <tuple>

namespace lol
{

class SdlInput : public entity
{
public:
    /** passing the screen resolution (note: not the windowed app resolution!) allows to make the mouse axis resolution-independent */
    SdlInput(int screen_w, int screen_h);

    virtual ~SdlInput();
    void SetScreenResolution();

protected:
    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, Scene &scene);

private:
    void tick(float seconds);

#if LOL_USE_SDL
    std::vector<std::tuple<SDL_Joystick *, std::shared_ptr<class input::device::joystick>>> m_joysticks;
#endif

    vec2 m_app;
    vec2 m_screen;

    ivec2 m_prev_mouse_pos = ivec2::zero;
    bool m_mousecapture = false;
    bool m_tick_in_draw_thread = false;
};

} /* namespace lol */

