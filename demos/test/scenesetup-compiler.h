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

#include <string>

#ifndef __FLEX_LEXER_H
#   define yyFlexLexer SceneSetupFlexLexer
#   include "FlexLexer.h"
#   undef yyFlexLexer
#endif

#include "generated/scenesetup-parser.h"

class SceneSetup;

namespace lol
{

//----
class SceneSetupScanner : public SceneSetupFlexLexer
{
public:
    SceneSetupScanner(char const *command);
    virtual ~SceneSetupScanner();
    virtual int LexerInput(char* buf, int max_size);
    virtual SceneSetupParser::token_type lex(SceneSetupParser::semantic_type* yylval,
                                             SceneSetupParser::location_type* yylloc);

private:
    char const *m_input;
};

//----
class SceneSetupCompiler
{
public:
    SceneSetupCompiler(class SceneSetup &uro);

    bool ParseString(char const *command);

    void Error(const class location& l, const std::string& m);
    void Error(const std::string& m);

    class SceneSetupScanner*    m_lexer;
    class SceneSetup&           m_sstp;
    String                      m_last_cmd;
};

} /* namespace lol */

