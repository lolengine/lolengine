//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

#include "lolgl.h"

#if defined __CELLOS_LV2__
#   include "platform/ps3/ps3app.h"
#elif defined _XBOX
#   include "platform/xbox/xboxapp.h"
#elif defined __native_client__
#   include "platform/nacl/nacl-app.h"
#elif defined __ANDROID__
#   include "platform/android/androidapp.h"
#elif defined HAVE_GLES_2X
#   include "eglapp.h"
#else
#   include "platform/sdl/sdlapp.h"
#   include "platform/sdl/sdlinput.h"
#endif

using namespace std;

namespace lol
{

class ApplicationData
{
    friend class Application;

    ApplicationData(char const *name, ivec2 resolution, float framerate)
        : app(name, resolution, framerate)
    { }

#if defined __CELLOS_LV2__
    Ps3App app;
#elif defined _XBOX
    XboxApp app;
#elif defined __native_client__
    NaClApp app;
#elif defined __ANDROID__
    AndroidApp app;
#elif defined HAVE_GLES_2X
    /* FIXME: this macro is only deactivated if we include "lolgl.h" */
    EglApp app;
#elif defined USE_SDL
    SdlApp app;
#else
#   error No application class available on this platform
#endif
};

/*
 * Public Application class
 */

Application::Application(char const *name, ivec2 resolution, float framerate)
{
    data = new ApplicationData(name, resolution, framerate);

    g_world.ExecLua("lua/init.lua");
}

bool Application::MustTick()
{
    return !Ticker::Finished();
}

void Application::Tick()
{
    data->app.Tick();
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

