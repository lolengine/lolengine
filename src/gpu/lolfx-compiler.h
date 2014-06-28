//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#ifndef __LOLFX_COMPILER_H__
#define __LOLFX_COMPILER_H__

#include <string>

#ifndef __FLEX_LEXER_H
#   define yyFlexLexer LolFxFlexLexer
#   include "FlexLexer.h"
#   undef yyFlexLexer
#endif

#include "generated/lolfx-parser.h"

class LolFx;

namespace lol
{

class LolFxScanner : public LolFxFlexLexer
{
public:
    LolFxScanner(char const *command);
    virtual ~LolFxScanner();
    virtual int LexerInput(char* buf, int max_size);
    virtual LolFxParser::token_type lex(LolFxParser::semantic_type* yylval,
                                        LolFxParser::location_type* yylloc);

private:
    bool IsExpressionTrue(char const *buf);

    map<String, String> m_pp_defines;

    enum
    {
        BlockIsFalse,
        BlockIsForeverFalse,
        BlockIsTrue,
    };
    array<int> m_pp_stack;

    char const *m_input;
};

class LolFxCompiler
{
public:
    LolFxCompiler(class LolFx &lolfx);

    bool ParseString(char const *command);

    void Error(const class location& l, const std::string& m);
    void Error(const std::string& m);

    class LolFxScanner *m_lexer;
    class LolFx &m_lolfx;
};

} /* namespace lol */

#endif /* __LOLFX_COMPILER_H__ */

