//
//  MY CLASS TYPE
//
//  Copyright © 2009-2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

namespace lol
{

//-----------------------------------------------------------------------------
class EasyMeshLuaLoader : public Lolua::Loader
{
public:
    EasyMeshLuaLoader();
    virtual ~EasyMeshLuaLoader();
};
    
//-----------------------------------------------------------------------------
class EasyMeshLuaObject : public Lolua::ObjectDef
{
public:
    //-------------------------------------------------------------------------
    EasyMeshLuaObject();
    virtual ~EasyMeshLuaObject();

    //-------------------------------------------------------------------------
    static EasyMeshLuaObject* New(Lolua::State* l, int arg_nb);
    static const Lolua::ObjectLib* GetLib();

    //-------------------------------------------------------------------------
    /*

    (csgu|csgunion)          { return token::T_CSGUNION; }
    (csgs|CsgSub)            { return token::T_CSGSUBSTRACT; }
    (csgsl|CsgSubL)          { return token::T_CSGSUBSTRACTLOSS; }
    (csga|csgand)            { return token::T_CSGAND; }
    (csgx|csgxor)            { return token::T_CSGXOR; }

    (lp|loop)                { return token::T_LOOP; }
    (sc|setcolor)            { return token::T_COLOR; }
    (sca|setcolora)          { return token::T_ACOLOR; }
    (scb|setcolorb)          { return token::T_BCOLOR; }
    (scv|setcolorv)          { return token::T_VCOLOR; }

    (tsw|tglscalewind)       { return token::T_TOGGLESCALEWINDING; }
    (tqw|tglquadweight)      { return token::T_TOGGLEQUADWEIGHTING; }
    (tpbn|tglpstbuildnormal) { return token::T_TOGGLEPOSTBUILDNORMAL; }
    (tvnc|tglvertnocleanup)  { return token::T_TOGGLEVERTNOCLEANUP; }

    (vm|vertmerge)           { return token::T_VERTMERGE; }
    (vs|vertseparate)        { return token::T_VERTSEPARATE; }

    (tx|translatex)          { return token::T_TRANSLATEX; }
    (ty|translatey)          { return token::T_TRANSLATEY; }
    (tz|translatez)          { return token::T_TRANSLATEZ; }
    (t|translate)            { return token::T_TRANSLATE; }
    (rx|rotatex)             { return token::T_ROTATEX; }
    (ry|rotatey)             { return token::T_ROTATEY; }
    (rz|rotatez)             { return token::T_ROTATEZ; }
    (r|rotate)               { return token::T_ROTATE; }
    (rj|radialjitter)        { return token::T_RADIALJITTER; }
    (tax|taperx)             { return token::T_TAPERX; }
    (tay|tapery)             { return token::T_TAPERY; }
    (taz|taperz)             { return token::T_TAPERZ; }
    (twx|twistx)             { return token::T_TWISTX; }
    (twy|twisty)             { return token::T_TWISTY; }
    (twz|twistz)             { return token::T_TWISTZ; }
    (shx|shearx)             { return token::T_SHEARX; }
    (shy|sheary)             { return token::T_SHEARY; }
    (shz|shearz)             { return token::T_SHEARZ; }
    (stx|stretchx)           { return token::T_STRETCHX; }
    (sty|stretchy)           { return token::T_STRETCHY; }
    (stz|stretchz)           { return token::T_STRETCHZ; }
    (bdxy|bendxy)            { return token::T_BENDXY; }
    (bdxz|bendxz)            { return token::T_BENDXZ; }
    (bdyx|bendyx)            { return token::T_BENDYX; }
    (bdyz|bendyz)            { return token::T_BENDYZ; }
    (bdzx|bendzx)            { return token::T_BENDZX; }
    (bdzy|bendzy)            { return token::T_BENDZY; }
    (sx|scalex)              { return token::T_SCALEX; }
    (sy|scaley)              { return token::T_SCALEY; }
    (sz|scalez)              { return token::T_SCALEZ; }
    (s|scale)                { return token::T_SCALE; }
    (mx|mirrorx)             { return token::T_MIRRORX; }
    (my|mirrory)             { return token::T_MIRRORY; }
    (mz|mirrorz)             { return token::T_MIRRORZ; }
    (ch|chamfer)             { return token::T_CHAMFER; }
    (splt|splittriangle)     { return token::T_SPLITTRIANGLE; }
    (smth|smooth)            { return token::T_SMOOTHMESH; }
    (dup|duplicate)          { return token::T_DUPLICATE; }

    (ac|addcylinder)         { return token::T_CYLINDER; }
    (asph|addsphere)         { return token::T_SPHERE; }
    (acap|addcapsule)        { return token::T_CAPSULE; }
    (ato|addtorus)           { return token::T_TORUS; }
    (ab|addbox)              { return token::T_BOX; }
    (ascb|addsmoothchamfbox) { return token::T_SMOOTHCHAMFBOX; }
    (afcb|addflatchamfbox)   { return token::T_FLATCHAMFBOX; }
    (as|addstar)             { return token::T_STAR; }
    (aes|addexpandedstar)    { return token::T_EXPANDEDSTAR; }
    (ad|adddisc)             { return token::T_DISC; }
    (at|addtriangle)         { return token::T_TRIANGLE; }
    (aq|addquad)             { return token::T_QUAD; }
    (acg|addcog)             { return token::T_COG; }
    */
    /*

    %{ *//* =======  BASE COLOR TYPES  ========================================= *//* %
}
%{ *//* COLOR *//* %}
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
*/

};

} /* namespace lol */
