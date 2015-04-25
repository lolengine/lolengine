//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if defined _XBOX
#   include <xtl.h>
#endif

#include "xboxapp.h"
#include "xboxinput.h"

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
    Ticker::Setup(fps);
    Video::Setup(res);

    /* Autoreleased objects */
    new XboxInput();
#endif
}

void XboxApp::ShowPointer(bool show)
{
    ;
}

void XboxApp::Tick()
{
    /* Tick the renderer, show the frame and clamp to desired framerate. */
    Ticker::TickDraw();

#if defined _XBOX
    D3DDevice *d3d_dev = (D3DDevice *)Renderer::Get()->GetDevice();
    d3d_dev->Present(nullptr, nullptr, nullptr, nullptr);
#endif
}

XboxApp::~XboxApp()
{
#if defined _XBOX
#endif
    delete data;
}

} /* namespace lol */

