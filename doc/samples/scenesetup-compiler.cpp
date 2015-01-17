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

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <string>

#include <lol/engine.h>
#include "scenesetup.h"
#include "scenesetup-compiler.h"

namespace lol
{

SceneSetupCompiler::SceneSetupCompiler(SceneSetup &uro)
    : m_sstp(uro), m_last_cmd("")
{
}

bool SceneSetupCompiler::ParseString(char const *command)
{
    SceneSetupScanner scanner(command);
    m_lexer = &scanner;
    SceneSetupParser parser(*this);
    if (parser.parse() != 0)
    {
        Log::Debug("Uro source: %s\n", command);
        return false;
    }
    return true;
}

void SceneSetupCompiler::Error(const class location& l, const std::string& m)
{
    Log::Error("SceneSetup syntax error line %d column %d: %s\n",
               l.begin.line, l.begin.column, m.c_str());
}

void SceneSetupCompiler::Error(const std::string& m)
{
    Log::Error("SceneSetup syntax error: %s\n", m.c_str());
}

} /* namespace lol */

