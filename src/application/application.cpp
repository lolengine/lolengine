//
//  Lol Engine
//
//  Copyright © 2010—2016 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if HAVE_EMSCRIPTEN_H
#   include <emscripten.h>
#endif

#include "lolgl.h"

#if __native_client__
#   include "platform/nacl/nacl-app.h"
#elif __ANDROID__
#   include "platform/android/androidapp.h"
#elif LOL_USE_SDL
#   include "platform/sdl/sdlapp.h"
#   include "platform/sdl/sdlinput.h"
#elif HAVE_GLES_2X
#   include "eglapp.h"
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

    void Enable() {}
    void Disable() {}
};

class null_app
{
public:
    null_app(char const *, ivec2, float)
    {
        ASSERT(false, "no display library (SDL, EGL…) available");
    }
    virtual ~null_app() {}

    void ShowPointer(bool) {}
    void Tick() {}
};

//-----------------------------------------------------------------------------
class ApplicationDisplayData
{
    friend class ApplicationDisplay;

    ApplicationDisplayData(char const *name, ivec2 res)
        : display(name, res)
    { }

protected:
#if __native_client__
    //NOT HANDLED YET
#elif __ANDROID__
    //NOT HANDLED YET
#elif LOL_USE_SDL
    SdlAppDisplay display;
#elif HAVE_GLES_2X
    /* FIXME: this macro is only deactivated if we include "lolgl.h" */
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
    super::set_resolution(res);

    data->display.set_resolution(res);
}

void ApplicationDisplay::SetPosition(ivec2 position)
{
    super::SetPosition(position);

    data->display.SetPosition(position);
}

void ApplicationDisplay::Enable()
{
    super::Enable();

    data->display.Enable();
}

void ApplicationDisplay::Disable()
{
    data->display.Disable();

    super::Disable();
}

//-----------------------------------------------------------------------------
class ApplicationData
{
    friend class Application;

    ApplicationData(char const *name, ivec2 res, float framerate)
        : app(name, res, framerate)
    { }

#if __native_client__
    NaClApp app;
#elif __ANDROID__
    AndroidApp app;
#elif LOL_USE_SDL
    SdlApp app;
#elif HAVE_GLES_2X
    /* FIXME: this macro is only deactivated if we include "lolgl.h" */
    EglApp app;
#else
    null_app app;
#endif
};

#if EMSCRIPTEN
static Application *g_app;

static void AppCallback()
{
    g_app->Tick();
}
#endif

/*
 * Public Application class
 */

Application::Application(char const *name, ivec2 res, float framerate)
{
    auto app_display = new ApplicationDisplay(name, res);
    SceneDisplay::Add(app_display);
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
#if EMSCRIPTEN
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
    delete data;
}

} /* namespace lol */

