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

#include "core.h"
#include "easymesh/easymesh.h"

#include <string>

%}

%require "2.3"
%debug
%defines
%skeleton "lalr1.cc"
%name-prefix="lol"
%define parser_class_name "EasyMeshParser"
%locations
%parse-param { class EasyMeshCompiler& mc }
%error-verbose

%union
{
    float fval;
    /* Can't use uin32_t here for some reason */
    unsigned u32val;
    struct { float f0, f1, f2, f3, f4, f5, f6, f7; } args;
}

%start mesh_description

%token T_COLOR T_BGCOLOR

%token T_TRANSLATEX T_ROTATEX T_TAPERX T_SCALEX T_MIRRORX
%token T_TRANSLATEY T_ROTATEY T_TAPERY T_SCALEY T_MIRRORY
%token T_TRANSLATEZ T_ROTATEZ T_TAPERZ T_SCALEZ T_MIRRORZ
%token T_TRANSLATE T_SCALE
%token T_CHAMFER

%token T_CYLINDER T_BOX T_SMOOTHCHAMFBOX T_FLATCHAMFBOX T_SPHERE T_CAPSULE
%token T_STAR T_EXPANDEDSTAR T_DISC T_TRIANGLE T_QUAD T_COG

%token T_END 0
%token T_ERROR

%token <fval> NUMBER
%token <u32val> COLOR

%type <fval> number
%type <args> args1 args2 args3 args4 args5 args6 args7 args8

%{
#include "easymesh/easymesh-compiler.h"

#undef yylex
#define yylex mc.m_lexer->lex
%}

%%

mesh_description:
    mesh_expression_list T_END
    ;

mesh_expression_list:
    mesh_expression
  | mesh_expression mesh_expression_list
  ;

mesh_expression:
    mesh_command_list
  | mesh_open mesh_expression_list mesh_close
    ;

mesh_open:
    '['       { mc.m_mesh.OpenBrace(); }
    ;

mesh_close:
    ']'       { mc.m_mesh.CloseBrace(); }
    ;

mesh_command_list:
    mesh_command
  | mesh_command_list mesh_command
    ;

mesh_command:
    color_command
  | transform_command
  | primitive_command
    ;

color_command:
    T_COLOR args4   { mc.m_mesh.SetCurColor(vec4($2.f0, $2.f1, $2.f2, $2.f3)); }
  | T_COLOR COLOR   { uint32_t x = $2;
                      vec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                      mc.m_mesh.SetCurColor(vec4(v) * (1. / 255)); }
  | T_BGCOLOR args4 { mc.m_mesh.SetCurColor2(vec4($2.f0, $2.f1, $2.f2, $2.f3)); }
  | T_BGCOLOR COLOR { uint32_t x = $2;
                      vec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                      mc.m_mesh.SetCurColor2(vec4(v) * (1. / 255)); }
    ;

transform_command:
    T_CHAMFER args1     { mc.m_mesh.Chamfer($2.f0); }
  | T_TRANSLATEX args1  { mc.m_mesh.Translate(vec3($2.f0, 0, 0)); }
  | T_TRANSLATEY args1  { mc.m_mesh.Translate(vec3(0, $2.f0, 0)); }
  | T_TRANSLATEZ args1  { mc.m_mesh.Translate(vec3(0, 0, $2.f0)); }
  | T_TRANSLATE args3   { mc.m_mesh.Translate(vec3($2.f0, $2.f1, $2.f2)); }
  | T_ROTATEX args1     { mc.m_mesh.RotateX($2.f0); }
  | T_ROTATEY args1     { mc.m_mesh.RotateY($2.f0); }
  | T_ROTATEZ args1     { mc.m_mesh.RotateZ($2.f0); }
  | T_TAPERX args3      { mc.m_mesh.TaperX($2.f0, $2.f1, $2.f2); }
  | T_TAPERY args3      { mc.m_mesh.TaperY($2.f0, $2.f1, $2.f2); }
  | T_TAPERZ args3      { mc.m_mesh.TaperZ($2.f0, $2.f1, $2.f2); }
  | T_SCALEX args1      { mc.m_mesh.Scale(vec3($2.f0, 0, 0)); }
  | T_SCALEY args1      { mc.m_mesh.Scale(vec3(0, $2.f0, 0)); }
  | T_SCALEZ args1      { mc.m_mesh.Scale(vec3(0, 0, $2.f0)); }
  | T_SCALE args3       { mc.m_mesh.Scale(vec3($2.f0, $2.f1, $2.f2)); }
  | T_MIRRORX           { mc.m_mesh.MirrorX(); }
  | T_MIRRORY           { mc.m_mesh.MirrorY(); }
  | T_MIRRORZ           { mc.m_mesh.MirrorZ(); }
    ;

primitive_command:
    T_CYLINDER args6       { mc.m_mesh.AppendCylinder((int)$2.f0, $2.f1,
                                                 $2.f2, $2.f3,
                                                 (int)$2.f4, (int)$2.f5); }
  | T_BOX args3            { mc.m_mesh.AppendBox(vec3($2.f0, $2.f1, $2.f2)); }
  | T_SMOOTHCHAMFBOX args4 { mc.m_mesh.AppendSmoothChamfBox(vec3($2.f0, $2.f1,
                                                            $2.f2), $2.f3); }
  | T_FLATCHAMFBOX args4   { mc.m_mesh.AppendFlatChamfBox(vec3($2.f0, $2.f1,
                                                          $2.f2), $2.f3); }
  | T_SPHERE args4         { mc.m_mesh.AppendSphere($2.f0,
                                               vec3($2.f1, $2.f2, $2.f3)); }
  | T_CAPSULE args3        { mc.m_mesh.AppendCapsule($2.f0, $2.f1, $2.f2); }
  | T_STAR args5           { mc.m_mesh.AppendStar((int)$2.f0, $2.f1, $2.f2,
                                             (int)$2.f3, (int)$2.f4); }
  | T_EXPANDEDSTAR args4   { mc.m_mesh.AppendExpandedStar((int)$2.f0, $2.f1,
                                                     $2.f2, $2.f3); }
  | T_DISC args3           { mc.m_mesh.AppendDisc((int)$2.f0, $2.f1, (int)$2.f2); }
  | T_TRIANGLE args2       { mc.m_mesh.AppendSimpleTriangle($2.f0, (int)$2.f1); }
  | T_QUAD args2           { mc.m_mesh.AppendSimpleQuad($2.f0, (int)$2.f1); }
  | T_COG args8            { mc.m_mesh.AppendCog((int)$2.f0, $2.f1, $2.f2, $2.f3,
                                            $2.f4, $2.f5, $2.f6, (int)$2.f7); }
    ;

args1: number { $$.f0 = $1; } ;
args2: args1 number { $$ = $1; $$.f1 = $2; } ;
args3: args2 number { $$ = $1; $$.f2 = $2; } ;
args4: args3 number { $$ = $1; $$.f3 = $2; } ;
args5: args4 number { $$ = $1; $$.f4 = $2; } ;
args6: args5 number { $$ = $1; $$.f5 = $2; } ;
args7: args6 number { $$ = $1; $$.f6 = $2; } ;
args8: args7 number { $$ = $1; $$.f7 = $2; } ;

number:
    NUMBER       { $$ = $1; }
  | '-' number   { $$ = -$2; }
    ;

%%

void lol::EasyMeshParser::error(const EasyMeshParser::location_type& l,
                                const std::string& m)
{
    mc.Error(l, m);
}

