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
static std::shared_ptr<app> g_app;
static void AppCallback() { g_app->Tick(); }
#endif

//
// Public app class
//

std::shared_ptr<app> app::init(char const *name, ivec2 res, float framerate)
{
    ticker::setup(framerate);

#if __ANDROID__
    auto ret = std::make_shared<android::app>(name, res);
#elif __NX__
    auto ret = std::make_shared<nx::app>(name, res);
#elif LOL_USE_SDL
    auto ret = std::make_shared<sdl::app>(name, res);
#elif HAVE_GLES_2X
    // FIXME: this macro is only deactivated if we include "lolgl.h"
    //NOT HANDLED YET
    auto ret = std::make_shared<egl::app>(name, res);
#endif

    Scene::add_display(ret->get_display());
    return ret;
}

app::~app()
{
    ticker::teardown();
}

bool app::must_tick()
{
    return !ticker::Finished();
}

void app::run()
{
#if __EMSCRIPTEN__
    // FIXME: convert this to a local lambda
    g_app = this;
    emscripten_set_main_loop(AppCallback, 0, 1);
#else
    while (must_tick())
        tick();
#endif
}

} // namespace lol
