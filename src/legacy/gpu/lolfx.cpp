//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

//
// The LolFx class
// ---------------
//

#include <lol/engine-internal.h>

namespace lol
{

LolFx::LolFx()
  : m_blend(false),
    m_alphatest(false),
    m_cullface(false),
    m_depthtest(false),
    m_depthmask(false),
    m_shader(0)
{
    ;
}

bool LolFx::Compile(char const *)
{
    return false;
}

} /* namespace lol */

