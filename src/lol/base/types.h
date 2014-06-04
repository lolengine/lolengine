//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if !defined __LOL_BASE_TYPES_H__
#define __LOL_BASE_TYPES_H__

namespace lol
{

/* There are many reasons for wanting single-word type names, the most
 * important one being compilation speedups in our vector.h: we can hide
 * some global methods in namespaces that contain the name of the type,
 * but namespaces cannot have spaces in their names. */
typedef long double ldouble;

/* The “real” type used for real numbers. It’s a specialisation of the
 * “Real” template class. */
template<int N> class Real;
typedef Real<16> real;

/* The “half” type used for 16-bit floating point numbers. */
class half;

#define DEFINE_ENUM(NEW_ENUM)   struct NEW_ENUM {

#define START_VALUE             enum Value {
#define ADD_VALUE(NEW_VALUE)    E##NEW_VALUE,
#define END_VALUE               MAX } m_value;

#define START_TEXT                                                          \
inline String C()                                                           \
{                                                                           \
    static String text[] = {
#define ADD_TEXT(NEW_TEXT)      #NEW_TEXT,
#define END_TEXT                                                            \
};                                                                          \
if (m_value >= MAX)                                                         \
    return "INVALID";                                                       \
return text[m_value];                                                       \
};                                                                          \

#define END_ENUM(NEW_ENUM)                                                  \
    inline NEW_ENUM()                   : m_value(MAX) {}                   \
    inline NEW_ENUM(Value v)            : m_value(v) {}                     \
    inline NEW_ENUM(int v)              : m_value((Value)v) {}              \
    inline NEW_ENUM(float v)            : m_value((Value)(int)v) {}         \
    bool operator==(const NEW_ENUM& v)  { return m_value == v.m_value; }    \
    bool operator==(const int& v)       { return m_value == NEW_ENUM(v); }  \
    bool operator==(const float& v)     { return m_value == NEW_ENUM(v); }  \
    inline operator Value()             { return m_value; }                 \
};                                                                          \
bool operator== (int e1, NEW_ENUM& e2)  { return (e2 == e1); }              \
bool operator== (float e1, NEW_ENUM& e2){ return (e2 == e1); }

} /* namespace lol */

#endif // __LOL_BASE_TYPES_H__

