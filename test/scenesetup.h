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

//
// The Scene setup class
// ----------------
//

#if !defined __SCENESETUP_H__
#define __SCENESETUP_H__

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
    int GetLightNb() { return m_lights.Count(); }

//private:
    Array<Light *>          m_lights;
    Array<String, String>   m_custom_cmd;
};

} /* namespace lol */

#endif /* __SCENESETUP_H__ */
