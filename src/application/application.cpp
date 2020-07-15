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
#include <cassert>

#if HAVE_EMSCRIPTEN_H
#   include <emscripten.h>
#endif

#include "lolgl.h"

#if __ANDROID__
#   include "application/android-app.h"
#elif __NX__
#   include "private/nx/nx-app.h"
#elif LOL_USE_SDL
#   include "application/sdl-app.h"
#elif HAVE_GLES_2X
#   include "application/egl-app.h"
#endif

namespace lol
{

#if __EMSCRIPTEN__
static app *g_app;
static void AppCallback() { g_app->Tick(); }
#endif

//
// Public app class
//

app::app(char const *name, ivec2 res, float framerate)
{
    ticker::setup(framerate);

    // FIXME: this should probably be a call to e.g. nx::app::init() which
    // creates the proper shared pointers.

#if __ANDROID__
    // TODO: implement m_display
    m_data = std::make_shared<AndroidAppData>(name, res, framerate);
#elif __NX__
    m_display = std::make_shared<nx::app_display>(name, res);
    m_data = std::make_shared<nx::app_data>(name, m_display->resolution(), framerate);
#elif LOL_USE_SDL
    m_display = std::make_shared<sdl::app_display>(name, res);
    m_data = std::make_shared<sdl::app_data>(name, m_display->resolution(), framerate);
#elif HAVE_GLES_2X
    // FIXME: this macro is only deactivated if we include "lolgl.h"
    //NOT HANDLED YET
    m_data = std::make_shared<EglAppData>(name, res, framerate);
#endif
    Scene::add_display(m_display);
}

app::~app()
{
    ticker::teardown();
}

bool app::must_tick()
{
    return !ticker::Finished();
}

void app::tick()
{
    if (m_data)
        m_data->tick();
}

void app::run()
{
#if __EMSCRIPTEN__
    g_app = this;
    emscripten_set_main_loop(AppCallback, 0, 1);
#else
    while (must_tick())
        tick();
#endif
}

void app::show_pointer(bool show)
{
    if (m_data)
        m_data->show_pointer(show);
}

} // namespace lol
