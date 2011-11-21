//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined __native_client__
#   include <ppapi/cpp/instance.h>
#   include <ppapi/cpp/module.h>
#   include <ppapi/cpp/var.h>
#endif

#include "core.h"
#include "lolgl.h"
#include "naclapp.h"

namespace lol
{

/*
 * PS3 App implementation class
 */

class NaClAppData
{
    friend class NaClApp;

private:
#if defined __native_client__
#endif
};

/*
 * Public NaClApp class
 */

NaClApp::NaClApp(char const *title, ivec2 res, float fps) :
    data(new NaClAppData())
{
#if defined __native_client__
#endif
}

void NaClApp::ShowPointer(bool show)
{
    ;
}

void NaClApp::Run()
{
    while (!Ticker::Finished())
    {
        /* Tick the game */
        Ticker::TickGame();

        /* Tick the renderer, show the frame and clamp to desired framerate. */
        Ticker::TickDraw();

#if defined __native_client__
#endif

        Ticker::ClampFps();
    }
}

NaClApp::~NaClApp()
{
#if defined __native_client__
#endif
    delete data;
}

} /* namespace lol */

