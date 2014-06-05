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

//-- STRUCT TEMPLATE:
//DEF_ENUM(NEW_ENUM)
//    DEF_VALUE
//        ADD_VALUE(NEW_VALUE)
//        ADD_VALUE_SET(NEW_VALUE, VALUE_SET)
//    END_E_VALUE
//    DEF_TEXT
//        ADD_TEXT(NEW_VALUE)
//    END_TEXT
//END_ENUM(NEW_ENUM)

#define DEF_ENUM(NEW_ENUM)   struct NEW_ENUM {
#define END_ENUM(NEW_ENUM)                                                      \
    inline NEW_ENUM()                       : m_value(MAX) {}                   \
    inline NEW_ENUM(Value v)                : m_value(v) {}                     \
    inline NEW_ENUM(const int8_t    & v)    : m_value((Value)v) {}              \
    inline NEW_ENUM(const int16_t   & v)    : m_value((Value)v) {}              \
    inline NEW_ENUM(const int32_t   & v)    : m_value((Value)v) {}              \
    inline NEW_ENUM(const int64_t   & v)    : m_value((Value)v) {}              \
    inline NEW_ENUM(const uint8_t   & v)    : m_value((Value)v) {}              \
    inline NEW_ENUM(const uint16_t  & v)    : m_value((Value)v) {}              \
    inline NEW_ENUM(const uint32_t  & v)    : m_value((Value)v) {}              \
    inline NEW_ENUM(const uint64_t  & v)    : m_value((Value)v) {}              \
    inline NEW_ENUM(const float     & v)    : m_value((Value)(int)v) {}         \
    inline NEW_ENUM(const double    & v)    : m_value((Value)(int)v) {}         \
    inline operator Value()                 { return m_value; }                 \
    inline int32_t  I()                     { return (int32_t ) m_value; }      \
    inline int8_t   I8()                    { return (int8_t  ) m_value; }      \
    inline int16_t  I16()                   { return (int16_t ) m_value; }      \
    inline int32_t  I32()                   { return (int32_t ) m_value; }      \
    inline int64_t  I64()                   { return (int64_t ) m_value; }      \
    inline uint32_t UI()                    { return (uint32_t) m_value; }      \
    inline uint8_t  UI8()                   { return (uint8_t ) m_value; }      \
    inline uint16_t UI16()                  { return (uint16_t) m_value; }      \
    inline uint32_t UI32()                  { return (uint32_t) m_value; }      \
    inline uint64_t UI64()                  { return (uint64_t) m_value; }      \
    inline float    F()                     { return (float   ) m_value; }      \
    inline double   D()                     { return (double  ) m_value; }      \
    bool operator==(const NEW_ENUM  & v)    { return m_value == v.m_value; }    \
    bool operator==(const Value     & v)    { return m_value == v; }            \
    bool operator==(const int8_t    & v)    { return m_value == NEW_ENUM(v); }  \
    bool operator==(const int16_t   & v)    { return m_value == NEW_ENUM(v); }  \
    bool operator==(const int32_t   & v)    { return m_value == NEW_ENUM(v); }  \
    bool operator==(const int64_t   & v)    { return m_value == NEW_ENUM(v); }  \
    bool operator==(const uint8_t   & v)    { return m_value == NEW_ENUM(v); }  \
    bool operator==(const uint16_t  & v)    { return m_value == NEW_ENUM(v); }  \
    bool operator==(const uint32_t  & v)    { return m_value == NEW_ENUM(v); }  \
    bool operator==(const uint64_t  & v)    { return m_value == NEW_ENUM(v); }  \
    bool operator==(const float     & v)    { return m_value == NEW_ENUM(v); }  \
    bool operator==(const double    & v)    { return m_value == NEW_ENUM(v); }  \
    bool operator!=(const NEW_ENUM  & v)    { return m_value != v.m_value; }    \
    bool operator!=(const Value     & v)    { return m_value != v; }            \
    bool operator!=(const int8_t    & v)    { return m_value != NEW_ENUM(v); }  \
    bool operator!=(const int16_t   & v)    { return m_value != NEW_ENUM(v); }  \
    bool operator!=(const int32_t   & v)    { return m_value != NEW_ENUM(v); }  \
    bool operator!=(const int64_t   & v)    { return m_value != NEW_ENUM(v); }  \
    bool operator!=(const uint8_t   & v)    { return m_value != NEW_ENUM(v); }  \
    bool operator!=(const uint16_t  & v)    { return m_value != NEW_ENUM(v); }  \
    bool operator!=(const uint32_t  & v)    { return m_value != NEW_ENUM(v); }  \
    bool operator!=(const uint64_t  & v)    { return m_value != NEW_ENUM(v); }  \
    bool operator!=(const float     & v)    { return m_value != NEW_ENUM(v); }  \
    bool operator!=(const double    & v)    { return m_value != NEW_ENUM(v); }  \
    bool operator<(const NEW_ENUM   & v)    { return m_value < v.m_value; }     \
    bool operator<(const Value      & v)    { return m_value < v; }             \
    bool operator<(const int8_t     & v)    { return m_value < NEW_ENUM(v); }   \
    bool operator<(const int16_t    & v)    { return m_value < NEW_ENUM(v); }   \
    bool operator<(const int32_t    & v)    { return m_value < NEW_ENUM(v); }   \
    bool operator<(const int64_t    & v)    { return m_value < NEW_ENUM(v); }   \
    bool operator<(const uint8_t    & v)    { return m_value < NEW_ENUM(v); }   \
    bool operator<(const uint16_t   & v)    { return m_value < NEW_ENUM(v); }   \
    bool operator<(const uint32_t   & v)    { return m_value < NEW_ENUM(v); }   \
    bool operator<(const uint64_t   & v)    { return m_value < NEW_ENUM(v); }   \
    bool operator<(const float      & v)    { return m_value < NEW_ENUM(v); }   \
    bool operator<(const double     & v)    { return m_value < NEW_ENUM(v); }   \
    bool operator>(const NEW_ENUM   & v)    { return m_value > v.m_value; }     \
    bool operator>(const Value      & v)    { return m_value > v; }             \
    bool operator>(const int8_t     & v)    { return m_value > NEW_ENUM(v); }   \
    bool operator>(const int16_t    & v)    { return m_value > NEW_ENUM(v); }   \
    bool operator>(const int32_t    & v)    { return m_value > NEW_ENUM(v); }   \
    bool operator>(const int64_t    & v)    { return m_value > NEW_ENUM(v); }   \
    bool operator>(const uint8_t    & v)    { return m_value > NEW_ENUM(v); }   \
    bool operator>(const uint16_t   & v)    { return m_value > NEW_ENUM(v); }   \
    bool operator>(const uint32_t   & v)    { return m_value > NEW_ENUM(v); }   \
    bool operator>(const uint64_t   & v)    { return m_value > NEW_ENUM(v); }   \
    bool operator>(const float      & v)    { return m_value > NEW_ENUM(v); }   \
    bool operator>(const double     & v)    { return m_value > NEW_ENUM(v); }   \
    bool operator<=(const NEW_ENUM  & v)    { return m_value <= v.m_value; }    \
    bool operator<=(const Value     & v)    { return m_value <= v; }            \
    bool operator<=(const int8_t    & v)    { return m_value <= NEW_ENUM(v); }  \
    bool operator<=(const int16_t   & v)    { return m_value <= NEW_ENUM(v); }  \
    bool operator<=(const int32_t   & v)    { return m_value <= NEW_ENUM(v); }  \
    bool operator<=(const int64_t   & v)    { return m_value <= NEW_ENUM(v); }  \
    bool operator<=(const uint8_t   & v)    { return m_value <= NEW_ENUM(v); }  \
    bool operator<=(const uint16_t  & v)    { return m_value <= NEW_ENUM(v); }  \
    bool operator<=(const uint32_t  & v)    { return m_value <= NEW_ENUM(v); }  \
    bool operator<=(const uint64_t  & v)    { return m_value <= NEW_ENUM(v); }  \
    bool operator<=(const float     & v)    { return m_value <= NEW_ENUM(v); }  \
    bool operator<=(const double    & v)    { return m_value <= NEW_ENUM(v); }  \
    bool operator>=(const NEW_ENUM  & v)    { return m_value >= v.m_value; }    \
    bool operator>=(const Value     & v)    { return m_value >= v; }            \
    bool operator>=(const int8_t    & v)    { return m_value >= NEW_ENUM(v); }  \
    bool operator>=(const int16_t   & v)    { return m_value >= NEW_ENUM(v); }  \
    bool operator>=(const int32_t   & v)    { return m_value >= NEW_ENUM(v); }  \
    bool operator>=(const int64_t   & v)    { return m_value >= NEW_ENUM(v); }  \
    bool operator>=(const uint8_t   & v)    { return m_value >= NEW_ENUM(v); }  \
    bool operator>=(const uint16_t  & v)    { return m_value >= NEW_ENUM(v); }  \
    bool operator>=(const uint32_t  & v)    { return m_value >= NEW_ENUM(v); }  \
    bool operator>=(const uint64_t  & v)    { return m_value >= NEW_ENUM(v); }  \
    bool operator>=(const float     & v)    { return m_value >= NEW_ENUM(v); }  \
    bool operator>=(const double    & v)    { return m_value >= NEW_ENUM(v); }  \
};
/*
bool operator== (NEW_ENUM::Value& e1, NEW_ENUM& e2) { return (e2 == e1); }
bool operator== (int& e1, NEW_ENUM& e2)             { return (e2 == e1); }
bool operator== (uint32_t& e1, NEW_ENUM& e2)        { return (e2 == e1); }
bool operator== (float& e1, NEW_ENUM& e2)           { return (e2 == e1); }
bool operator!= (NEW_ENUM::Value& e1, NEW_ENUM& e2) { return (e2 != e1); }
bool operator!= (int& e1, NEW_ENUM& e2)             { return (e2 != e1); }
bool operator!= (uint32_t& e1, NEW_ENUM& e2)        { return (e2 != e1); }
bool operator!= (float& e1, NEW_ENUM& e2)           { return (e2 != e1); }
bool operator< (NEW_ENUM::Value& e1, NEW_ENUM& e2)  { return (e2 < e1);  }
bool operator< (int& e1, NEW_ENUM& e2)              { return (e2 < e1);  }
bool operator< (uint32_t& e1, NEW_ENUM& e2)         { return (e2 < e1);  }
bool operator< (float& e1, NEW_ENUM& e2)            { return (e2 < e1);  }
bool operator> (NEW_ENUM::Value& e1, NEW_ENUM& e2)  { return (e2 > e1);  }
bool operator> (int& e1, NEW_ENUM& e2)              { return (e2 > e1);  }
bool operator> (uint32_t& e1, NEW_ENUM& e2)         { return (e2 > e1);  }
bool operator> (float& e1, NEW_ENUM& e2)            { return (e2 > e1);  }
bool operator<= (NEW_ENUM::Value& e1, NEW_ENUM& e2) { return (e2 <= e1); }
bool operator<= (int& e1, NEW_ENUM& e2)             { return (e2 <= e1); }
bool operator<= (uint32_t& e1, NEW_ENUM& e2)        { return (e2 <= e1); }
bool operator<= (float& e1, NEW_ENUM& e2)           { return (e2 <= e1); }
bool operator>= (NEW_ENUM::Value& e1, NEW_ENUM& e2) { return (e2 >= e1); }
bool operator>= (int& e1, NEW_ENUM& e2)             { return (e2 >= e1); }
bool operator>= (uint32_t& e1, NEW_ENUM& e2)        { return (e2 >= e1); }
bool operator>= (float& e1, NEW_ENUM& e2)           { return (e2 >= e1); }
*/

#define DEF_VALUE                                   enum Value { INVALID = -1,
#define ADD_VALUE(NEW_VALUE)                                     NEW_VALUE,
#define ADD_VALUE_SET(NEW_VALUE, VALUE_SET)                      NEW_VALUE = VALUE_SET,
#define END_E_VALUE                                              MAX } m_value;

#define DEF_TEXT                                                                \
inline String C()                                                               \
{                                                                               \
    static const String text[] = {
#define ADD_TEXT(NEW_TEXT)          String(#NEW_TEXT),
#define ADD_TEXT_PADDING            String(""),
#define END_TEXT                 };                                             \
    if (m_value < 0 || m_value >= MAX)                                          \
        return "INVALID";                                                       \
    return text[m_value];                                                       \
};

} /* namespace lol */

#endif // __LOL_BASE_TYPES_H__

