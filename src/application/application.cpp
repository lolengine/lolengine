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
#elif LOL_USE_SDL || LOL_USE_OLD_SDL
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

public:
    null_display(char const *, ivec2) {}
    virtual ~null_display() {}

protected:
    void SetResolution(ivec2) {}
    void SetPosition(ivec2) {}

    void Enable() {}
    void Disable() {}
};

class null_app
{
public:
    null_app(char const *, ivec2, float) {}
    virtual ~null_app() {}

    void ShowPointer(bool) {}
    void Tick() {}
};

//-----------------------------------------------------------------------------
class ApplicationDisplayData
{
    friend class ApplicationDisplay;

    ApplicationDisplayData(char const *name, ivec2 resolution)
        : display(name, resolution)
    { }

protected:
    void SetResolution(ivec2 resolution)
    {
        display.SetResolution(resolution);
    }
    void SetPosition(ivec2 position)
    {
        display.SetPosition(position);
    }
    void Enable()
    {
        display.Enable();
    }
    void Disable()
    {
        display.Disable();
    }

#if __native_client__
    //NOT HANDLED YET
#elif __ANDROID__
    //NOT HANDLED YET
#elif LOL_USE_SDL || LOL_USE_OLD_SDL
    SdlAppDisplay display;
#elif HAVE_GLES_2X
    /* FIXME: this macro is only deactivated if we include "lolgl.h" */
    //NOT HANDLED YET
#else
    null_display display;
#endif
};

ApplicationDisplay::ApplicationDisplay(char const *name, ivec2 resolution)
{
    data = new ApplicationDisplayData(name, resolution);
}

ApplicationDisplay::~ApplicationDisplay()
{
    delete data;
}

void ApplicationDisplay::SetResolution(ivec2 resolution)
{
    super::SetResolution(resolution);

    data->SetResolution(resolution);
}
void ApplicationDisplay::SetPosition(ivec2 position)
{
    super::SetPosition(position);

    data->SetPosition(position);
}

void ApplicationDisplay::Enable()
{
    super::Enable();

    data->Enable();
}

void ApplicationDisplay::Disable()
{
    data->Disable();

    super::Disable();
}

//-----------------------------------------------------------------------------
class ApplicationData
{
    friend class Application;

    ApplicationData(char const *name, ivec2 resolution, float framerate)
        : app(name, resolution, framerate)
    { }

#if __native_client__
    NaClApp app;
#elif __ANDROID__
    AndroidApp app;
#elif LOL_USE_SDL || LOL_USE_OLD_SDL
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

Application::Application(char const *name, ivec2 resolution, float framerate)
{
    SceneDisplay::Add(new ApplicationDisplay(name, resolution));
    data = new ApplicationData(name, resolution, framerate);
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

