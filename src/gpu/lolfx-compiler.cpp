//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <string>

#include "core.h"
#include "gpu/lolfx-compiler.h"

namespace lol
{

LolFxCompiler::LolFxCompiler(LolFx &lolfx)
    : m_lolfx(lolfx)
{
}

bool LolFxCompiler::ParseString(char const *command)
{
    LolFxScanner scanner(command);
    m_lexer = &scanner;
    LolFxParser parser(*this);
    if (parser.parse() != 0)
        return false;
    return true;
}

void LolFxCompiler::Error(const class location& l, const std::string& m)
{
    Log::Error("LolFx syntax error line %d column %d: %s\n",
               l.begin.line, l.begin.column, m.c_str());
}

void LolFxCompiler::Error(const std::string& m)
{
    Log::Error("LolFx syntax error: %s\n", m.c_str());
}

} /* namespace lol */

