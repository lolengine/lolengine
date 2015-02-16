//
//  Lol Engine
//
//  Copyright © 2010-2015 Sam Hocevar <sam@hocevar.net>
//            © 2013-2015 Guillaume Bittoun <guillaume.bittoun@gmail.com>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#include <lol/base/map.h>

namespace lol
{

////MyType --------------------------------------------------------------------
//struct MyTypeBase : public StructSafeEnum
//{
//    enum Type
//    {
//    };
//protected:
//    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
//    {
//        enum_map[] = "";
//        return true;
//    }
//};
//typedef SafeEnum<MyTypeBase> MyType;

//-----------------------------------------------------------------------------
struct StructSafeEnum
{
protected:
    /* Convert to string stuff */
    virtual bool BuildEnumMap(map<int64_t, String>&) { return false; }
};
//-----------------------------------------------------------------------------
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

	/* Convert to string stuff */
    inline class String ToString()
    {
		/* FIXME: we all know this isn’t thread safe. But is it really
		* a big deal? */
		static map<int64_t, String> enum_map;
		static bool ready = false;

		if (ready || this->BuildEnumMap(enum_map))
		{
			ready = true;
			if (enum_map.has_key((int64_t)m_value))
				return enum_map[(int64_t)m_value];
		}
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

} /* namespace lol */

