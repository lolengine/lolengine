//
// Lol Engine
//
// Copyright: (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The CommandStack struct
// ------------------
//

#if !defined __COMMANDSTACK_COMMANDSTACK_H__
#define __COMMANDSTACK_COMMANDSTACK_H__

namespace lol
{

//Utility struct to convert command code to pseudo-bytecode
struct CommandStack
{
private:
    array<int, int, int>    m_commands;
    array<float>            m_floats;
    array<int>              m_ints;
    int                     m_f_cur;
    int                     m_i_cur;

public:
    //GET/SET exec
    int     GetCmdNb()      { return m_commands.Count(); }
    int     GetCmd(int i)
    {
        ASSERT(0 <= i && i < m_commands.Count());
        m_f_cur = m_commands[i].m2;
        m_i_cur = m_commands[i].m3;
        return m_commands[i].m1;
    }

    //cmd storage
    void    AddCmd(int cmd) { m_commands.Push(cmd, m_floats.Count(), m_ints.Count()); }

    //GETTER
    inline float   F()      { return m_floats[m_f_cur++]; }
    inline int     I()      { return m_ints[m_i_cur++]; }
    inline int     E()      { return I(); }
    inline bool    B()      { return !!I(); }
    inline vec2    V2()     { vec2  v(F());       v.y = F(); return v; }
    inline vec3    V3()     { vec3  v(V2(), 0.f); v.z = F(); return v; }
    inline vec4    V4()     { vec4  v(V3(), 0.f); v.w = F(); return v; }
    inline ivec2   IV2()    { ivec2 v(I());       v.y = I(); return v; }
    inline ivec3   IV3()    { ivec3 v(IV2(), 0);  v.z = I(); return v; }
    inline ivec4   IV4()    { ivec4 v(IV3(), 0);  v.w = I(); return v; }

    // Alternate getters
    inline void    GetValue(float &f)   { f = F(); }
    inline void    GetValue(int &i)     { i = I(); }
    inline void    GetValue(bool &b)    { b = B(); }
    inline void    GetValue(vec2 &v2)   { v2 = V2(); }
    inline void    GetValue(vec3 &v3)   { v3 = V3(); }
    inline void    GetValue(vec4 &v4)   { v4 = V4(); }
    inline void    GetValue(ivec2 &iv2) { iv2 = IV2(); }
    inline void    GetValue(ivec3 &iv3) { iv3 = IV3(); }
    inline void    GetValue(ivec4 &iv4) { iv4 = IV4(); }
    // For Safe Enum
    template<class DEF> inline
    void GetValue(SafeEnum<DEF> &i) { i = typename DEF::Type(I()); }

    //SETTER
    CommandStack &operator<<(int i)     { m_ints << i; return *this; }
    CommandStack &operator<<(float f)   { m_floats << f; return *this; }
    CommandStack &operator<<(bool b)    { return (*this << (int)b); }
    CommandStack &operator<<(vec2 v)    { return (*this << v.x   << v.y); }
    CommandStack &operator<<(vec3 v)    { return (*this << v.xy  << v.z); }
    CommandStack &operator<<(vec4 v)    { return (*this << v.xyz << v.w); }
    CommandStack &operator<<(ivec2 iv)  { return (*this << iv.x   << iv.y); }
    CommandStack &operator<<(ivec3 iv)  { return (*this << iv.xy  << iv.z); }
    CommandStack &operator<<(ivec4 iv)  { return (*this << iv.xyz << iv.w); }
    // For Safe Enum
    template<class DEF> inline
    CommandStack &operator<<(SafeEnum<DEF> &i) { return *this << i.ToScalar(); }
};

} /* namespace lol */

#endif /* __COMMANDSTACK_COMMANDSTACK_H__ */
