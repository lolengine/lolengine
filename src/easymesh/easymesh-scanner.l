%{
//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2012 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2012 Benjamin Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
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
#include "easymesh/easymesh-compiler.h"

typedef lol::EasyMeshParser::token token;
typedef lol::EasyMeshParser::token_type token_type;

#ifndef YY_DECL
#   define YY_DECL lol::EasyMeshParser::token_type \
        lol::EasyMeshScanner::lex(lol::EasyMeshParser::semantic_type* yylval, \
                                  lol::EasyMeshParser::location_type* yylloc)
#endif

#define yyterminate() return token::T_END
#define YY_NO_UNISTD_H
#define YY_USER_ACTION yylloc->columns(yyleng);
%}

%option c++ prefix="EasyMesh"
%option batch yywrap nounput stack

%%

%{
    /* reset location at the beginning of yylex() */
    yylloc->step();
%}

sc    { return token::T_COLOR; }
scb   { return token::T_BGCOLOR; }

ch    { return token::T_CHAMFER; }
tx    { return token::T_TRANSLATEX; }
ty    { return token::T_TRANSLATEY; }
tz    { return token::T_TRANSLATEZ; }
t     { return token::T_TRANSLATE; }
rx    { return token::T_ROTATEX; }
ry    { return token::T_ROTATEY; }
rz    { return token::T_ROTATEZ; }
tax   { return token::T_TAPERX; }
tay   { return token::T_TAPERY; }
taz   { return token::T_TAPERZ; }
sx    { return token::T_SCALEX; }
sy    { return token::T_SCALEY; }
sz    { return token::T_SCALEZ; }
s     { return token::T_SCALE; }
mx    { return token::T_MIRRORX; }
my    { return token::T_MIRRORY; }
mz    { return token::T_MIRRORZ; }

ac    { return token::T_CYLINDER; }
ab    { return token::T_BOX; }
ascb  { return token::T_SMOOTHCHAMFBOX; }
afcb  { return token::T_FLATCHAMFBOX; }
acap  { return token::T_CAPSULE; }
asph  { return token::T_SPHERE; }
at    { return token::T_TORUS; }
as    { return token::T_STAR; }
aes   { return token::T_EXPANDEDSTAR; }
ad    { return token::T_DISC; }
at    { return token::T_TRIANGLE; }
aq    { return token::T_QUAD; }
acg   { return token::T_COG; }

#[0-9a-fA-F]{3} {
        uint32_t tmp = std::strtol(yytext + 1, NULL, 16);
        yylval->u32val = 0x11000000u * (tmp >> 8)
                       | 0x00110000u * ((tmp >> 4) & 0xf)
                       | 0x00001100u * (tmp & 0xf)
                       | 0x000000ffu;
        return token::COLOR; }
#[0-9a-fA-F]{4} {
        uint32_t tmp = std::strtol(yytext + 1, NULL, 16);
        yylval->u32val = 0x11000000u * (tmp >> 12)
                       | 0x00110000u * ((tmp >> 8) & 0xf)
                       | 0x00001100u * ((tmp >> 4) & 0xf)
                       | 0x00000011u * (tmp & 0xf);
        return token::COLOR; }
#[0-9a-fA-F]{6} {
        yylval->u32val = 0xffu
                       | 0x100u * (uint32_t)std::strtol(yytext + 1, NULL, 16);
        return token::COLOR; }
#[0-9a-fA-F]{8} {
        yylval->u32val = (uint32_t)std::strtol(yytext + 1, NULL, 16);
        return token::COLOR; }
[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)? {
        yylval->fval = std::atof(yytext); return token::NUMBER; }
-     { return token_type('-'); }
"["   { return token_type('['); }
"]"   { return token_type(']'); }
[ ,]  { /* ignore this */ }
[\n]  { /* ignore this */ }
.     { return token::T_ERROR; }

%%

lol::EasyMeshScanner::EasyMeshScanner(char const *command)
    : EasyMeshFlexLexer(0, 0),
      m_input(command)
{
}

lol::EasyMeshScanner::~EasyMeshScanner()
{
}

int lol::EasyMeshScanner::LexerInput(char* buf, int max_size)
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
int EasyMeshFlexLexer::yylex()
{
    std::cerr << "in EasyMeshFlexLexer::yylex() !" << std::endl;
    return 0;
}

int EasyMeshFlexLexer::yywrap()
{
    return 1;
}

