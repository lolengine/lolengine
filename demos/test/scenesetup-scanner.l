%{
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

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
using std::exit;
using std::malloc;
using std::realloc;
using std::free;

#include "core.h"
#include "../scenesetup.h"
#include "../scenesetup-compiler.h"

typedef lol::SceneSetupParser::token      token;
typedef lol::SceneSetupParser::token_type token_type;

#ifndef YY_DECL
#   define YY_DECL lol::SceneSetupParser::token_type \
                   lol::SceneSetupScanner::lex(lol::SceneSetupParser::semantic_type* yylval, \
                   lol::SceneSetupParser::location_type* yylloc)
#endif

#define yyterminate() return token::T_END
#define YY_NO_UNISTD_H
#define YY_USER_ACTION yylloc->columns(yyleng);
%}

%option c++ prefix="SceneSetup"
%option batch yywrap nounput stack

%%

%{  /* reset location at the beginning of yylex() */
    yylloc->step();
%}

addlight            { return token::T_ADDLIGHT;     }
position            { return token::T_OBJPOSITION;  }
lookat              { return token::T_OBJLOOKAT;    }
color               { return token::T_OBJCOLOR;     }
clearcolor          { return token::T_CLEARCOLOR;   }

showgizmo           { return token::T_SHOWGIZMO;    }
showlight           { return token::T_SHOWLIGHT;    }
custom              { return token::T_CUSTOMCMD;    }

custom              { return token::T_CUSTOMCMD;    }

%{ /* =======  BASE COLOR TYPES  ========================================= */ %}
%{ /* COLOR */ %}
#[0-9a-fA-F]{3} {
        uint32_t tmp = std::strtol(yytext + 1, nullptr, 16);
        yylval->u32val = 0x11000000u * (tmp >> 8)
                       | 0x00110000u * ((tmp >> 4) & 0xf)
                       | 0x00001100u * (tmp & 0xf)
                       | 0x000000ffu;
        return token::COLOR; }
#[0-9a-fA-F]{4} {
        uint32_t tmp = std::strtol(yytext + 1, nullptr, 16);
        yylval->u32val = 0x11000000u * (tmp >> 12)
                       | 0x00110000u * ((tmp >> 8) & 0xf)
                       | 0x00001100u * ((tmp >> 4) & 0xf)
                       | 0x00000011u * (tmp & 0xf);
        return token::COLOR; }
#[0-9a-fA-F]{6} {
        yylval->u32val = 0xffu
                       | 0x100u * (uint32_t)std::strtol(yytext + 1, nullptr, 16);
        return token::COLOR; }
#[0-9a-fA-F]{8} {
        yylval->u32val = (uint32_t)std::strtol(yytext + 1, nullptr, 16);
        return token::COLOR; }

%{ /* =======  BASE DATA TYPES  ========================================= */ %}
%{ /* BOOL */ %}
true { yylval->bval = true; return token::BOOLEAN; }
false { yylval->bval = false; return token::BOOLEAN; }
%{ /* FLOAT */ %}
[-+]?[0-9]*\.[0-9]+([eE][-+]?[0-9]+)? {
        yylval->fval = (float)std::atof(yytext); return token::F_NUMBER; }
%{ /* INT */ %}
[-+]?[0-9]+ {
        yylval->ival = std::atoi(yytext); return token::I_NUMBER; }
%{ /* STRING */ %}
[\"][^\"]*[\"] {
        yylval->sval = strdup(yytext); return token::STRING; }
%{ /* STRING VAR */ %}
[a-zA-Z][a-zA-Z_\-]+ {
        yylval->svval = strdup(yytext); return token::STRING_VAR; }

%{ /* ======= COMMENTS ======= */ %}
"//".* { /* ignore this */ }

%{ /* Semantics tokens */ %}
"-"   { return token_type('-'); }
"("   { return token_type('('); }
")"   { return token_type(')'); }
[ ,]  { /* ignore this */       }
[\n]  { /* ignore this */       }
.     { return token::T_ERROR;  }

%%

lol::SceneSetupScanner::SceneSetupScanner(char const *command)
    : SceneSetupFlexLexer(0, 0),
      m_input(command)
{
}

lol::SceneSetupScanner::~SceneSetupScanner()
{
}

int lol::SceneSetupScanner::LexerInput(char* buf, int max_size)
{
    (void)max_size; /* unused for now */

    buf[0] = m_input[0];
    if (buf[0])
        ++m_input;
    return buf[0] ? 1 : 0;
}

#ifdef yylex
#undef yylex
#endif
int SceneSetupFlexLexer::yylex()
{
    std::cerr << "in SceneSetupFlexLexer::yylex() !" << std::endl;
    return 0;
}

int SceneSetupFlexLexer::yywrap()
{
    return 1;
}

