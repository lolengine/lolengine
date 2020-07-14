﻿//
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

class null_display
{
    friend class ApplicationDisplayData;
    friend class ApplicationDisplay;

public:
    null_display(char const *, ivec2) {}
    virtual ~null_display() {}

protected:
    ivec2 resolution() const { return ivec2(0); }
    void set_resolution(ivec2) {}
    void SetPosition(ivec2) {}
};

class null_app
{
public:
    null_app(char const *, ivec2, float)
    {
        msg::error("no display library (SDL, EGL…) available");
        assert(false);
    }
    virtual ~null_app() {}

    void ShowPointer(bool) {}
    void Tick() {}
};

class ApplicationDisplayData
{
    friend class ApplicationDisplay;

    ApplicationDisplayData(char const *name, ivec2 res)
        : display(name, res)
    { }

protected:
#if __ANDROID__
    // TODO: implement this
    null_display display;
#elif __NX__
    nx::app_display display;
#elif LOL_USE_SDL
    sdl::app_display display;
#elif HAVE_GLES_2X
    // FIXME: this macro is only deactivated if we include "lolgl.h"
    //NOT HANDLED YET
#else
    null_display display;
#endif
};

ApplicationDisplay::ApplicationDisplay(char const *name, ivec2 res)
{
    data = new ApplicationDisplayData(name, res);
}

ApplicationDisplay::~ApplicationDisplay()
{
    delete data;
}

ivec2 ApplicationDisplay::resolution() const
{
    return data->display.resolution();
}

void ApplicationDisplay::set_resolution(ivec2 res)
{
    data->display.set_resolution(res);
}

void ApplicationDisplay::SetPosition(ivec2 position)
{
    data->display.SetPosition(position);
}

void ApplicationDisplay::start_frame()
{
    data->display.start_frame();
}

void ApplicationDisplay::end_frame()
{
    data->display.end_frame();
}

class ApplicationData
{
    friend class Application;

    ApplicationData(char const *name, ivec2 res, float framerate)
        : app(name, res, framerate)
    { }

#if __ANDROID__
    AndroidApp app;
#elif __NX__
    nx::app app;
#elif LOL_USE_SDL
    sdl::app app;
#elif HAVE_GLES_2X
    // FIXME: this macro is only deactivated if we include "lolgl.h"
    EglApp app;
#else
    null_app app;
#endif
};

#if __EMSCRIPTEN__
static Application *g_app;

static void AppCallback()
{
    g_app->Tick();
}
#endif

//
// Public Application class
//

Application::Application(char const *name, ivec2 res, float framerate)
{
    ticker::setup(framerate);

    auto app_display = new ApplicationDisplay(name, res);
    Scene::add_display(app_display);
    data = new ApplicationData(name, app_display->resolution(), framerate);
}

bool Application::MustTick()
{
    return !Ticker::Finished();
}

void Application::Tick()
{
    data->app.Tick();
}

void Application::Run()
{
#if __EMSCRIPTEN__
    g_app = this;
    emscripten_set_main_loop(AppCallback, 0, 1);
#else
    while (MustTick())
        Tick();
#endif
}

void Application::ShowPointer(bool show)
{
    data->app.ShowPointer(show);
}

Application::~Application()
{
    ticker::teardown();
    delete data;
}

} // namespace lol
