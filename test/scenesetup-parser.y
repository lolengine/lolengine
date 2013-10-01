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

#include "core.h"
#include "../scenesetup.h"

%}

%require "2.3"
%debug
%defines
%skeleton "lalr1.cc"
%name-prefix="lol"
%define parser_class_name "SceneSetupParser"
%locations
%parse-param { class SceneSetupCompiler& uc }
%error-verbose

%union
{
    float   fval;
    int     ival;
    bool    bval;
    float   vval[4];
    int     ivval[4];
    char*   sval;
    char*   svval;
    /* Can't use uin32_t here for some reason */
    unsigned u32val;
}

%start sstp_description

%token T_COLOR

%token T_ADDLIGHT T_OBJPOSITION T_OBJLOOKAT T_OBJCOLOR
%token T_CUSTOMCMD

%token T_END 0
%token T_ERROR

%token <fval>   F_NUMBER
%token <ival>   I_NUMBER
%token <sval>   STRING
%token <svval>  STRING_VAR
%token <bval>   BOOLEAN
%token <u32val> COLOR

/* Base Number types */
%type <fval>    fv
%type <ival>    iv
/* Vector types */
%type <vval>    v3
%type <ivval>   iv3
%type <vval>    v4
/* Special types */
%type <bval>    bv
%type <sval>    sv
%type <svval>   svv


%{
#include "../scenesetup-compiler.h"

#undef yylex
#define yylex uc.m_lexer->lex

/* HACK: workaround for Bison who insists on using exceptions */
#define try if (true)
#define catch(...) if (false)
#define throw (void)0
%}

%%

sstp_description:
    sstp_expression_list T_END
  ;

sstp_expression_list:
    sstp_expression
  | sstp_expression sstp_expression_list
  ;

sstp_expression:
    sstp_command_list
  ;

sstp_command_list:
    sstp_command
  | sstp_command_list sstp_command
  ;

sstp_command:
    light_command
  | setup_command
  | custom_command
  ;

light_command:
    T_ADDLIGHT                      { uc.m_sstp.m_lights << new Light(); uc.m_last_cmd = "ADDLIGHT"; }
  | T_ADDLIGHT      fv              { uc.m_sstp.m_lights << new Light(); uc.m_last_cmd = "ADDLIGHT";
                                      uc.m_sstp.m_lights.Last()->SetPosition(vec4(vec3::zero, $2)); }
  ;

setup_command:
    T_OBJPOSITION   v3              { if (uc.m_last_cmd == "ADDLIGHT")
                                        uc.m_sstp.m_lights.Last()->SetPosition(vec4(vec3($2[0], $2[1], $2[2]), uc.m_sstp.m_lights.Last()->GetPosition().w)); }
  | T_OBJLOOKAT     v3              { if (uc.m_last_cmd == "ADDLIGHT")
                                        { /* */ } }
  | T_OBJCOLOR      v4              { if (uc.m_last_cmd == "ADDLIGHT")
                                        uc.m_sstp.m_lights.Last()->SetColor(vec4($2[0], $2[1], $2[2], $2[3])); }
  | T_OBJCOLOR      COLOR           { uint32_t x = $2;
                                      ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                                      vec4 vv = vec4(v) * (1.f / 255.f);
                                      if (uc.m_last_cmd == "ADDLIGHT")
                                        uc.m_sstp.m_lights.Last()->SetColor(vv); }
  ;

custom_command:
    T_CUSTOMCMD     svv sv          { uc.m_sstp.m_custom_cmd.Push($2, $3); }
  ;


/* Base Number types */
fv:
    F_NUMBER        { $$ = $1; }
  | '-' fv          { $$ = -$2; }
  | I_NUMBER        { $$ = (float)$1; }
  | '-' iv          { $$ = -(float)$2; }
  ;

iv:
    I_NUMBER        { $$ = $1; }
  | '-' iv          { $$ = -$2; }
  | F_NUMBER        { $$ = (int)$1; }
  | '-' fv          { $$ = -(int)$2; }
  ;

/* Vector types */
v3:
    '('fv')'        { $$[0] = $2; $$[1] = $2; $$[2] = $2; }
  | '('fv fv fv')'  { $$[0] = $2; $$[1] = $3; $$[2] = $4; }
  ;

iv3:
    '('iv')'        { $$[0] = $2; $$[1] = $2; $$[2] = $2; }
  | '('iv iv iv')'  { $$[0] = $2; $$[1] = $3; $$[2] = $4; }
  ;

v4:
    '('fv')'          { $$[0] = $2; $$[1] = $2; $$[2] = $2; $$[3] = $2; }
  | '('fv fv fv fv')' { $$[0] = $2; $$[1] = $3; $$[2] = $4; $$[3] = $5; }
  ;

/* Special types */
bv:
    BOOLEAN         { $$ = $1; }
  | I_NUMBER        { $$ = !!$1; }
  | F_NUMBER        { $$ = !!$1; }
  ;

svv:
    STRING_VAR      { $$ = $1; }
  ;

sv:
    STRING          { String t = $1;
                      t.Replace('"', ' ', true);
                      free($1);
                      $$ = strdup((const char *)t.C()); }
  | STRING sv       { String t = $1;
                      t += $2;
                      t.Replace('"', ' ', true);
                      free($1);
                      free($2);
                      $$ = strdup((const char *)t.C()); }
  ;

%%

void lol::SceneSetupParser::error(const SceneSetupParser::location_type& l,
                                const std::string& m)
{
    uc.Error(l, m);
}

