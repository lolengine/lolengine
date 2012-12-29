//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2012 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2012 Benjamin Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#ifndef __EASYMESH_COMPILER_H__
#define __EASYMESH_COMPILER_H__

#include <string>

#ifndef __FLEX_LEXER_H
#   define yyFlexLexer EasyMeshFlexLexer
#   include "FlexLexer.h"
#   undef yyFlexLexer
#endif

#include "generated/easymesh-parser.h"

class EasyMesh;

namespace lol
{

class EasyMeshScanner : public EasyMeshFlexLexer
{
public:
    EasyMeshScanner(char const *command);
    virtual ~EasyMeshScanner();
    virtual int LexerInput(char* buf, int max_size);
    virtual EasyMeshParser::token_type lex(EasyMeshParser::semantic_type* yylval,
                                           EasyMeshParser::location_type* yylloc);

private:
    char const *m_input;
};

class EasyMeshCompiler
{
public:
    EasyMeshCompiler(class EasyMesh &mesh);

    bool ParseString(char const *command);

    void Error(const class location& l, const std::string& m);
    void Error(const std::string& m);

    class EasyMeshScanner* m_lexer;
    class EasyMesh &m_mesh;
};

} /* namespace lol */

#endif /* __EASYMESH_COMPILER_H__ */

