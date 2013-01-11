//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The LolFx class
// ---------------
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined _XBOX
#   define _USE_MATH_DEFINES /* for M_PI */
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#elif defined _WIN32
#   define _USE_MATH_DEFINES /* for M_PI */
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

#include "core.h"
#include "gpu/lolfx-compiler.h"

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

bool LolFx::Compile(char const *command)
{
    LolFxCompiler lc(*this);
    return lc.ParseString(command);
}

} /* namespace lol */

