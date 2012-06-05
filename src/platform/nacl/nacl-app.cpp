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

#include "core.h"

#include "nacl-app.h"
#include "nacl-instance.h"

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
    Ticker::Setup(fps);
#if defined __native_client__
#endif
}

void NaClApp::ShowPointer(bool show)
{
    ;
}

void NaClApp::Run()
{
#if defined __native_client__
    NaClInstance::MainSignal();
#endif

    /* Wait forever */
    Queue<int, 1> q;
    q.Pop();
}

NaClApp::~NaClApp()
{
#if defined __native_client__
#endif
    delete data;
}

} /* namespace lol */

