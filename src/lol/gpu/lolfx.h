//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The LolFx class
// ---------------
//

#include <lol/gpu/shader.h>

namespace lol
{

class LolFx
{
    friend class LolFxParser;

public:
    LolFx();

    bool Compile(char const *command);

private:
    /* Simple GL / DX render states */
    bool m_blend;
    bool m_alphatest;
    bool m_cullface;
    bool m_depthtest;
    bool m_depthmask;

    /* Our shaders -- FIXME: should be split if possible */
    Shader *m_shader;
};

} /* namespace lol */

