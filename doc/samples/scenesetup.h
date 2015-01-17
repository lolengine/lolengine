//
// Lol Engine
//
// Copyright: (c) 2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Scene setup class
// ----------------
//

namespace lol
{

class SceneSetup
{
    friend class SceneSetupParser;

public:
    //CTor/DTor
    SceneSetup();
    ~SceneSetup();

    static char const *GetName() { return "<scenesetup>"; }

    //--
    bool Compile(char const *command);

    //--
    bool Startup();
    bool Shutdown(bool destroy=false);

    //--
    vec4                    m_clear_color;
    array<Light *>          m_lights;
    array<String, String>   m_custom_cmd;
    bool                    m_show_gizmo;
    bool                    m_show_lights;
};

} /* namespace lol */

