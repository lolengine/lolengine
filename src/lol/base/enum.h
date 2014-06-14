//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if !defined __LOL_BASE_ENUM_H__
#define __LOL_BASE_ENUM_H__

namespace lol
{

extern Map<int64_t, String> BuildEnumMap(char const *str, char const **custom);

template<typename BASE, typename T = typename BASE::Type>
class SafeEnum : public BASE
{
    typedef T Type;
    Type m_value;

public:
    inline SafeEnum() : m_value() {}
    inline SafeEnum(Type v) : m_value(v) {}

    /* Allow conversion from int and to the underlying type */
    inline explicit SafeEnum(int i) : m_value(T(i)) {}
    inline Type ToScalar() const { return m_value; }

    /* Convert to string description */
    inline class String ToString() const
    {
        /* FIXME: we all know this isn’t thread safe. But is it really
         * a big deal? */
        static Map<int64_t, String> map;
        static bool ready = false;

        if (!ready)
            map = BuildEnumMap(BASE::GetDescription(), BASE::GetCustomString());
        ready = true;

        if (map.HasKey((int64_t)m_value))
            return map[(int64_t)m_value];
        return "<invalid enum>";
    }

    /* Safe comparisons between enums of the same type */
    friend bool operator == (SafeEnum const &a, SafeEnum const &b)
    {
        return a.m_value == b.m_value;
    }
    friend bool operator != (SafeEnum const &a, SafeEnum const &b)
    {
        return a.m_value != b.m_value;
    }
    friend bool operator <  (SafeEnum const &a, SafeEnum const &b)
    {
        return a.m_value <  b.m_value;
    }
    friend bool operator >  (SafeEnum const &a, SafeEnum const &b)
    {
        return a.m_value >  b.m_value;
    }
    friend bool operator <= (SafeEnum const &a, SafeEnum const &b)
    {
        return a.m_value <= b.m_value;
    }
    friend bool operator >= (SafeEnum const &a, SafeEnum const &b)
    {
        return a.m_value >= b.m_value;
    }
};

#define LOL_OPEN_SAFE_ENUM(name, ...) \
    struct name ## Base \
    { \
        enum Type {__VA_ARGS__}; \
    protected: \
        static inline char const *GetDescription() { return #__VA_ARGS__; }
#define LOL_SAFE_ENUM_CUSTOM_TOSTRING(...) \
        static inline char const **GetCustomString() \
        { \
            static char const *custom_list[] = { __VA_ARGS__ }; \
            return &custom_list[0]; \
        }
#define LOL_CLOSE_SAFE_ENUM(name) \
    }; \
    typedef SafeEnum<name ## Base> name;

#define LOL_SAFE_ENUM(name, ...) \
    struct name ## Base \
    { \
        enum Type {__VA_ARGS__}; \
    protected: \
        static inline char const *GetDescription()      { return #__VA_ARGS__; } \
        static inline char const **GetCustomString()    { return nullptr; } \
    }; \
    typedef SafeEnum<name ## Base> name;

//-- STRUCT TEMPLATE:
// enum E {
//    DEF_VALUE
//        ADD_VALUE(NEW_VALUE)
//        ADD_VALUE_SET(NEW_VALUE, VALUE_SET)
//    END_E_VALUE
//    DEF_TEXT
//        ADD_TEXT(NEW_VALUE)
//    END_TEXT
//    LOL_DECLARE_ENUM_METHODS(E)
// };

#define LOL_DECLARE_ENUM_METHODS(E) \
    inline E()                       : m_value(MAX) {}           \
    inline E(Value v)                : m_value(v) {}             \
    inline E(const int8_t    & v)    : m_value((Value)v) {}      \
    inline E(const int16_t   & v)    : m_value((Value)v) {}      \
    inline E(const int32_t   & v)    : m_value((Value)v) {}      \
    inline E(const int64_t   & v)    : m_value((Value)v) {}      \
    inline E(const uint8_t   & v)    : m_value((Value)v) {}      \
    inline E(const uint16_t  & v)    : m_value((Value)v) {}      \
    inline E(const uint32_t  & v)    : m_value((Value)v) {}      \
    inline E(const uint64_t  & v)    : m_value((Value)v) {}      \
    inline operator Value() { return m_value; } \
    inline int32_t  I()     { return (int32_t ) m_value; } \
    inline int8_t   I8()    { return (int8_t  ) m_value; } \
    inline int16_t  I16()   { return (int16_t ) m_value; } \
    inline int32_t  I32()   { return (int32_t ) m_value; } \
    inline int64_t  I64()   { return (int64_t ) m_value; } \
    inline uint32_t UI()    { return (uint32_t) m_value; } \
    inline uint8_t  UI8()   { return (uint8_t ) m_value; } \
    inline uint16_t UI16()  { return (uint16_t) m_value; } \
    inline uint32_t UI32()  { return (uint32_t) m_value; } \
    inline uint64_t UI64()  { return (uint64_t) m_value; } \
    bool operator==(const E & v) { return m_value == v.m_value; } \
    bool operator!=(const E & v) { return m_value != v.m_value; } \
    bool operator<(const E & v)  { return m_value < v.m_value; }  \
    bool operator>(const E & v)  { return m_value > v.m_value; }  \
    bool operator<=(const E & v) { return m_value <= v.m_value; } \
    bool operator>=(const E & v) { return m_value >= v.m_value; }

#define DEF_VALUE                           enum Value { INVALID = -1,
#define ADD_VALUE(NEW_VALUE)                NEW_VALUE,
#define ADD_VALUE_SET(NEW_VALUE, VALUE_SET) NEW_VALUE = VALUE_SET,
#define END_E_VALUE                         MAX } m_value;

#define DEF_TEXT \
    inline String C() \
    { \
        static const String text[] = {

#define ADD_TEXT(NEW_TEXT) String(#NEW_TEXT),
#define ADD_TEXT_PADDING   String(""),

#define END_TEXT \
        }; \
        if (m_value < 0 || m_value >= MAX) \
            return "INVALID"; \
        return text[m_value]; \
    };

} /* namespace lol */

#endif // __LOL_BASE_ENUM_H__

