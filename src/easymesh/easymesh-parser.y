%{
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
    float   fval;
    int     ival;
    bool    bval;
    float   vval[4];
    int     ivval[4];
    char*   lval;
    /* Can't use uin32_t here for some reason */
    unsigned u32val;
}

%start mesh_description

%token T_LOOP T_COLOR T_BCOLOR T_VCOLOR

%token T_TRANSLATEX T_ROTATEX T_TAPERX T_TWISTX T_SHEARX T_STRETCHX T_BENDXY T_BENDXZ T_SCALEX T_MIRRORX
%token T_TRANSLATEY T_ROTATEY T_TAPERY T_TWISTY T_SHEARY T_STRETCHY T_BENDYX T_BENDYZ T_SCALEY T_MIRRORY
%token T_TRANSLATEZ T_ROTATEZ T_TAPERZ T_TWISTZ T_SHEARZ T_STRETCHZ T_BENDZX T_BENDZY T_SCALEZ T_MIRRORZ
%token T_TRANSLATE T_ROTATE T_SCALE T_TOGGLESCALEWINDING T_RADIALJITTER T_SPLITTRIANGLE T_SMOOTHMESH
%token T_DUPLICATE
%token T_CSGUNION T_CSGSUBSTRACT T_CSGSUBSTRACTLOSS T_CSGAND T_CSGXOR
%token T_CHAMFER

%token T_CYLINDER T_BOX T_SMOOTHCHAMFBOX T_FLATCHAMFBOX T_SPHERE T_CAPSULE
%token T_STAR T_EXPANDEDSTAR T_DISC T_TRIANGLE T_QUAD T_COG T_TORUS

%token T_END 0
%token T_ERROR

%token <fval>   F_NUMBER
%token <ival>   I_NUMBER
%token <bval>   BOOLEAN
%token <u32val> COLOR

/* Base Number types */
%type <fval>    fv
%type <ival>    iv
/* Vector types */
%type <vval>    v3
%type <vval>    v4
/* Special types */
%type <bval>    bv

%{
#include "easymesh/easymesh-compiler.h"

#undef yylex
#define yylex mc.m_lexer->lex

/* HACK: workaround for Bison who insists on using exceptions */
#define try if (true)
#define catch(...) if (false)
#define throw (void)0
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

mesh_command_list:
    mesh_command
  | mesh_command mesh_command_list
  ;

mesh_command:
    color_command
  | transform_command
  | primitive_command
  | post_brace_command
  | pre_brace_command '[' mesh_expression_list mesh_close
  ;

post_brace_command:
    T_CSGUNION          mesh_open mesh_expression_list ']'     { mc.m_mesh.CsgUnion();  mc.m_mesh.CloseBrace(); }
  | T_CSGSUBSTRACT      mesh_open mesh_expression_list ']'     { mc.m_mesh.CsgSub();    mc.m_mesh.CloseBrace(); }
  | T_CSGSUBSTRACTLOSS  mesh_open mesh_expression_list ']'     { mc.m_mesh.CsgSubL();   mc.m_mesh.CloseBrace(); }
  | T_CSGAND            mesh_open mesh_expression_list ']'     { mc.m_mesh.CsgAnd();    mc.m_mesh.CloseBrace(); }
  | T_CSGXOR            mesh_open mesh_expression_list ']'     { mc.m_mesh.CsgXor();    mc.m_mesh.CloseBrace(); }
  | doloop '[' mesh_expression_list ']'           { mc.m_mesh.LoopEnd(); }
  ;

doloop:
    T_LOOP iv           { mc.m_mesh.LoopStart($2); }
pre_brace_command:
    T_DUPLICATE             { mc.m_mesh.DupAndScale(vec3::one, true); }
  ;

mesh_open:
    '['                     { mc.m_mesh.OpenBrace(); }
  ;

mesh_close:
    ']'                     { mc.m_mesh.CloseBrace(); }
  ;

color_command:
    T_COLOR fv fv fv fv     { mc.m_mesh.SetCurColor(vec4($2, $3, $4, $5)); }
  | T_COLOR v4              { mc.m_mesh.SetCurColor(vec4($2[0], $2[1], $2[2], $2[3])); }
  | T_COLOR COLOR           { uint32_t x = $2;
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetCurColor(vec4(v) * (1.f / 255.f)); }
  | T_BCOLOR fv fv fv fv    { mc.m_mesh.SetCurColor2(vec4($2, $3, $4, $5)); }
  | T_BCOLOR v4             { mc.m_mesh.SetCurColor2(vec4($2[0], $2[1], $2[2], $2[3])); }
  | T_BCOLOR COLOR          { uint32_t x = $2;
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetCurColor2(vec4(v) * (1.f / 255.f)); }
  | T_VCOLOR fv fv fv fv    { mc.m_mesh.SetVertColor(vec4($2, $3, $4, $5)); }
  | T_VCOLOR v4             { mc.m_mesh.SetVertColor(vec4($2[0], $2[1], $2[2], $2[3])); }
  | T_VCOLOR COLOR          { uint32_t x = $2;
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetVertColor(vec4(v) * (1.f / 255.f)); }
  ;

transform_command:
    T_TRANSLATEX fv        { mc.m_mesh.Translate(vec3($2, 0.f, 0.f)); }
  | T_TRANSLATEY fv        { mc.m_mesh.Translate(vec3(0.f, $2, 0.f)); }
  | T_TRANSLATEZ fv        { mc.m_mesh.Translate(vec3(0.f, 0.f, $2)); }
  | T_TRANSLATE  fv fv fv  { mc.m_mesh.Translate(vec3($2, $3, $4)); }
  |  T_TRANSLATE v3        { mc.m_mesh.Translate(vec3($2[0], $2[1], $2[2])); }
  | T_ROTATEX fv           { mc.m_mesh.RotateX($2); }
  | T_ROTATEY fv           { mc.m_mesh.RotateY($2); }
  | T_ROTATEZ fv           { mc.m_mesh.RotateZ($2); }
  | T_ROTATE  fv fv fv fv  { mc.m_mesh.Rotate($2, vec3($3, $4, $5)); }
  |  T_ROTATE fv v3        { mc.m_mesh.Rotate($2, vec3($3[0], $3[1], $3[2])); }
  | T_RADIALJITTER fv      { mc.m_mesh.RadialJitter($2); }
  | T_TAPERX  fv fv fv bv  { mc.m_mesh.TaperX($2, $3, $4, $5); }
  |  T_TAPERX fv fv fv     { mc.m_mesh.TaperX($2, $3, $4); }
  |  T_TAPERX fv fv        { mc.m_mesh.TaperX($2, $3); }
  | T_TAPERY  fv fv fv bv  { mc.m_mesh.TaperY($2, $3, $4, $5); }
  |  T_TAPERY fv fv fv     { mc.m_mesh.TaperY($2, $3, $4); }
  |  T_TAPERY fv fv        { mc.m_mesh.TaperY($2, $3); }
  | T_TAPERZ  fv fv fv bv  { mc.m_mesh.TaperZ($2, $3, $4, $5); }
  |  T_TAPERZ fv fv fv     { mc.m_mesh.TaperZ($2, $3, $4); }
  |  T_TAPERZ fv fv        { mc.m_mesh.TaperZ($2, $3); }
  | T_TWISTX  fv fv        { mc.m_mesh.TwistX($2, $3); }
  |  T_TWISTX fv           { mc.m_mesh.TwistX($2); }
  | T_TWISTY  fv fv        { mc.m_mesh.TwistY($2, $3); }
  |  T_TWISTY fv           { mc.m_mesh.TwistY($2); }
  | T_TWISTZ  fv fv        { mc.m_mesh.TwistZ($2, $3); }
  |  T_TWISTZ fv           { mc.m_mesh.TwistZ($2); }
  | T_SHEARX  fv fv fv bv  { mc.m_mesh.ShearX($2, $3, $4, $5); }
  |  T_SHEARX fv fv fv     { mc.m_mesh.ShearX($2, $3, $4); }
  |  T_SHEARX fv fv        { mc.m_mesh.ShearX($2, $3); }
  | T_SHEARY  fv fv fv bv  { mc.m_mesh.ShearY($2, $3, $4, $5); }
  |  T_SHEARY fv fv fv     { mc.m_mesh.ShearY($2, $3, $4); }
  |  T_SHEARY fv fv        { mc.m_mesh.ShearY($2, $3); }
  | T_SHEARZ  fv fv fv bv  { mc.m_mesh.ShearZ($2, $3, $4, $5); }
  |  T_SHEARZ fv fv fv     { mc.m_mesh.ShearZ($2, $3, $4); }
  |  T_SHEARZ fv fv        { mc.m_mesh.ShearZ($2, $3); }
  | T_STRETCHX  fv fv fv   { mc.m_mesh.StretchX($2, $3, $4); }
  |  T_STRETCHX fv fv      { mc.m_mesh.StretchX($2, $3); }
  | T_STRETCHY  fv fv fv   { mc.m_mesh.StretchY($2, $3, $4); }
  |  T_STRETCHY fv fv      { mc.m_mesh.StretchY($2, $3); }
  | T_STRETCHZ  fv fv fv   { mc.m_mesh.StretchZ($2, $3, $4); }
  |  T_STRETCHZ fv fv      { mc.m_mesh.StretchZ($2, $3); }
  | T_BENDXY  fv fv        { mc.m_mesh.BendXY($2, $3); }
  |  T_BENDXY fv           { mc.m_mesh.BendXY($2); }
  | T_BENDXZ  fv fv        { mc.m_mesh.BendXZ($2, $3); }
  |  T_BENDXZ fv           { mc.m_mesh.BendXZ($2); }
  | T_BENDYX  fv fv        { mc.m_mesh.BendYX($2, $3); }
  |  T_BENDYX fv           { mc.m_mesh.BendYX($2); }
  | T_BENDYZ  fv fv        { mc.m_mesh.BendYZ($2, $3); }
  |  T_BENDYZ fv           { mc.m_mesh.BendYZ($2); }
  | T_BENDZX  fv fv        { mc.m_mesh.BendZX($2, $3); }
  |  T_BENDZX fv           { mc.m_mesh.BendZX($2); }
  | T_BENDZY  fv fv        { mc.m_mesh.BendZY($2, $3); }
  |  T_BENDZY fv           { mc.m_mesh.BendZY($2); }
  | T_SCALEX fv            { mc.m_mesh.Scale(vec3($2, 1.f, 1.f)); }
  | T_SCALEY fv            { mc.m_mesh.Scale(vec3(1.f, $2, 1.f)); }
  | T_SCALEZ fv            { mc.m_mesh.Scale(vec3(1.f, 1.f, $2)); }
  | T_SCALE fv fv fv       { mc.m_mesh.Scale(vec3($2, $3, $4)); }
  |  T_SCALE v3            { mc.m_mesh.Scale(vec3($2[0], $2[1], $2[2])); }
  |  T_SCALE fv            { mc.m_mesh.Scale(vec3($2, $2, $2)); }
  | T_MIRRORX              { mc.m_mesh.MirrorX(); }
  | T_MIRRORY              { mc.m_mesh.MirrorY(); }
  | T_MIRRORZ              { mc.m_mesh.MirrorZ(); }
  | T_CHAMFER fv           { mc.m_mesh.Chamfer($2); }
  | T_SPLITTRIANGLE iv     { mc.m_mesh.SplitTriangles($2); }
  | T_SMOOTHMESH iv iv iv  { mc.m_mesh.SmoothMesh($2, $3, $4); }
  | T_TOGGLESCALEWINDING   { mc.m_mesh.ToggleScaleWinding(); }
  ;

primitive_command:
    T_CYLINDER  iv fv fv fv bv bv bv { mc.m_mesh.AppendCylinder($2, $3, $4, $5, $6, $7, $8); }
  |  T_CYLINDER iv fv fv fv bv bv   { mc.m_mesh.AppendCylinder($2, $3, $4, $5, $6, $7); }
  |  T_CYLINDER iv fv fv fv bv      { mc.m_mesh.AppendCylinder($2, $3, $4, $5, $6); }
  |  T_CYLINDER iv fv fv fv         { mc.m_mesh.AppendCylinder($2, $3, $4, $5); }
  | T_SPHERE iv fv                  { mc.m_mesh.AppendSphere($2, $3); }
  | T_CAPSULE iv fv fv              { mc.m_mesh.AppendCapsule($2, $3, $4); }
  | T_TORUS iv fv fv                { mc.m_mesh.AppendTorus($2, $3, $4); }
  | T_BOX  fv fv fv fv              { mc.m_mesh.AppendBox(vec3($2, $3, $4), $5); }
  |  T_BOX fv fv fv                 { mc.m_mesh.AppendBox(vec3($2, $3, $4)); }
  |  T_BOX fv                       { mc.m_mesh.AppendBox(vec3($2, $2, $2)); }
  |  T_BOX v3 fv                    { mc.m_mesh.AppendBox(vec3($2[0], $2[1], $2[2]), $3); }
  |  T_BOX v3                       { mc.m_mesh.AppendBox(vec3($2[0], $2[1], $2[2])); }
  | T_SMOOTHCHAMFBOX  fv fv fv fv   { mc.m_mesh.AppendSmoothChamfBox(vec3($2, $3, $4), $5); }
  |  T_SMOOTHCHAMFBOX fv fv         { mc.m_mesh.AppendSmoothChamfBox(vec3($2, $2, $2), $3); }
  |  T_SMOOTHCHAMFBOX v3 fv         { mc.m_mesh.AppendSmoothChamfBox(vec3($2[0], $2[1], $2[2]), $3); }
  | T_FLATCHAMFBOX  fv fv fv fv     { mc.m_mesh.AppendFlatChamfBox(vec3($2, $3, $4), $5); }
  |  T_FLATCHAMFBOX fv fv           { mc.m_mesh.AppendFlatChamfBox(vec3($2, $2, $2), $3); }
  |  T_FLATCHAMFBOX v3 fv           { mc.m_mesh.AppendFlatChamfBox(vec3($2[0], $2[1], $2[2]), $3); }
  | T_STAR  iv fv fv bv bv          { mc.m_mesh.AppendStar($2, $3, $4, $5, $6); }
  |  T_STAR iv fv fv bv             { mc.m_mesh.AppendStar($2, $3, $4, $5); }
  |  T_STAR iv fv fv                { mc.m_mesh.AppendStar($2, $3, $4); }
  | T_EXPANDEDSTAR  iv fv fv fv     { mc.m_mesh.AppendExpandedStar($2, $3, $4, $5); }
  |  T_EXPANDEDSTAR iv fv fv        { mc.m_mesh.AppendExpandedStar($2, $3, $4); }
  | T_DISC  iv fv bv                { mc.m_mesh.AppendDisc($2, $3, $4); }
  |  T_DISC iv fv                   { mc.m_mesh.AppendDisc($2, $3); }
  | T_TRIANGLE  fv bv               { mc.m_mesh.AppendSimpleTriangle($2, $3); }
  |  T_TRIANGLE fv                  { mc.m_mesh.AppendSimpleTriangle($2); }
  | T_QUAD  fv bv                   { mc.m_mesh.AppendSimpleQuad($2, $3); }
  |  T_QUAD fv                      { mc.m_mesh.AppendSimpleQuad($2); }
  | T_COG  iv fv fv fv fv fv fv fv fv bv { mc.m_mesh.AppendCog($2, $3, $4, $5, $6, $7, $8, $9, $10, $11); }
  |  T_COG iv fv fv fv fv fv fv fv fv    { mc.m_mesh.AppendCog($2, $3, $4, $5, $6, $7, $8, $9, $10); }
  |  T_COG iv fv fv fv fv fv fv fv       { mc.m_mesh.AppendCog($2, $3, $4, $5, $6, $7, $8, $9); }
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

%%

void lol::EasyMeshParser::error(const EasyMeshParser::location_type& l,
                                const std::string& m)
{
    mc.Error(l, m);
}

