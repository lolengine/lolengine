//
// Lol Engine
//
// Copyright: (c) 2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//                2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "scenesetup.h"
#include "scenesetup-compiler.h"

namespace lol
{

//-----------------------------------------------------------------------------
//CTor/DTor
SceneSetup::SceneSetup()
{
}

//----
SceneSetup::~SceneSetup()
{
    Shutdown(true);
}

//-----------------------------------------------------------------------------
bool SceneSetup::Compile(char const *command)
{
    SceneSetupCompiler mc(*this);
    return mc.ParseString(command);
}

//-----------------------------------------------------------------------------
bool SceneSetup::Startup()
{
    for (int i = 0; i < m_lights.Count(); i++)
        Ticker::Ref(m_lights[i]);
    return true;
}

//-----------------------------------------------------------------------------
bool SceneSetup::Shutdown(bool destroy)
{
    for (int i = 0; i < m_lights.Count(); i++)
        if (m_lights[i]->IsTicked())
            Ticker::Unref(m_lights[i]);

    if (destroy)
        m_lights.Empty();
    return true;
}

} /* namespace lol */
