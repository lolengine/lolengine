//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if HAVE_EMSCRIPTEN_H
#   include <emscripten.h>
#endif

#include "lolgl.h"

#if _XBOX
#   include "platform/xbox/xboxapp.h"
#elif __native_client__
#   include "platform/nacl/nacl-app.h"
#elif __ANDROID__
#   include "platform/android/androidapp.h"
#elif USE_SDL || USE_OLD_SDL
#   include "platform/sdl/sdlapp.h"
#   include "platform/sdl/sdlinput.h"
#elif HAVE_GLES_2X
#   include "eglapp.h"
#endif

namespace lol
{

class ApplicationData
{
    friend class Application;

    ApplicationData(char const *name, ivec2 resolution, float framerate)
        : app(name, resolution, framerate)
    { }

#if _XBOX
    XboxApp app;
#elif __native_client__
    NaClApp app;
#elif __ANDROID__
    AndroidApp app;
#elif USE_SDL || USE_OLD_SDL
    SdlApp app;
#elif HAVE_GLES_2X
    /* FIXME: this macro is only deactivated if we include "lolgl.h" */
    EglApp app;
#else
#   error No application class available on this platform
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
    data = new ApplicationData(name, resolution, framerate);

    g_world.ExecLua("lua/init.lua");
    int32_t gravity = g_world.GetVar<int32_t>("gravity");
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

