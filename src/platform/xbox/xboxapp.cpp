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

#if defined _XBOX
#   include <xtl.h>
#endif

#include "core.h"
#include "lolgl.h"
#include "xboxapp.h"

namespace lol
{

/*
 * XboxApp implementation class
 */

class XboxAppData
{
    friend class XboxApp;

private:
#if defined _XBOX
#endif
};

/*
 * Public XboxApp class
 */

XboxApp::XboxApp(char const *title, ivec2 res, float fps) :
    data(new XboxAppData())
{
#if defined _XBOX
#endif
}

void XboxApp::ShowPointer(bool show)
{
    ;
}

void XboxApp::Run()
{
    while (!Ticker::Finished())
    {
        /* Tick the renderer, show the frame and clamp to desired framerate. */
        Ticker::TickDraw();

#if defined _XBOX
#endif
    }
}

XboxApp::~XboxApp()
{
#if defined _XBOX
#endif
    delete data;
}

} /* namespace lol */

