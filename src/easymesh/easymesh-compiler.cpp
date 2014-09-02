//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2013 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <string>

#include "easymesh/easymesh-compiler.h"

namespace lol
{

EasyMeshCompiler::EasyMeshCompiler(EasyMesh &mesh)
    : m_mesh(mesh)
{
}

bool EasyMeshCompiler::ParseString(char const *command)
{
    EasyMeshScanner scanner(command);
    m_lexer = &scanner;
    EasyMeshParser parser(*this);
    if (parser.parse() != 0)
    {
        Log::Debug("Mesh source: %s\n", command);
        return false;
    }
    return true;
}

void EasyMeshCompiler::Error(const class location& l, const std::string& m)
{
    Log::Error("EasyMesh syntax error line %d column %d: %s\n",
               l.begin.line, l.begin.column, m.c_str());
}

void EasyMeshCompiler::Error(const std::string& m)
{
    Log::Error("EasyMesh syntax error: %s\n", m.c_str());
}

} /* namespace lol */

