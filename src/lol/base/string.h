//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The String class
// ----------------
// A very simple String class, based on Array.
//

#if !defined __LOL_BASE_STRING_H__
#define __LOL_BASE_STRING_H__

#include <lol/base/array.h>

#include <cstring>
#include <cstdarg>

namespace lol
{

class String : protected Array<char>
{
private:
    typedef Array<char> Super;

public:
    inline String()
      : Super()
    {
        Push('\0');
    }

    inline String(char const *str)
      : Super()
    {
        using namespace std;
        Resize((int)strlen(str));
        memcpy(&(*this)[0], str, Count() + 1);
    }

    inline String(char const *str, int count)
      : Super()
    {
        using namespace std;
        Resize(count);
        memcpy(&(*this)[0], str, count);
        (*this)[count] = '\0';
    }

    inline String(String const &s)
      : Super((Super const &)s)
    {
    }

    inline char &operator [](int n)
    {
        return ((Super &)*this)[n];
    }

    inline char const &operator [](int n) const
    {
        return ((Super const &)*this)[n];
    }

    inline char &Last()
    {
        return (*this)[Count() - 1];
    }

    inline char const &Last() const
    {
        return (*this)[Count() - 1];
    }

    inline int Count() const
    {
        return ((Super const &)*this).Count() - 1;
    }

    void Resize(int count)
    {
        ((Super &)*this).Resize(count + 1);
        ((Super &)*this).Last() = '\0';
    }

    inline String operator +(String const &s) const
    {
        String ret(*this);
        return ret += s;
    }

    inline String& operator +=(String const &s)
    {
        /* Ignore the trailing zero we don't want */
        --m_count;
        (Super &)*this += (Super const &)s;
        return *this;
    }

    inline String operator +(char c) const
    {
        String ret(*this);
        return ret += c;
    }

    inline String& operator +=(char c)
    {
        ((Super &)*this).Last() = c;
        ((Super &)*this).Push('\0');
        return *this;
    }

    inline bool operator ==(String const &s) const
    {
        if (this->m_count != s.m_count)
            return false;

        for (int i = 0; i < this->m_count; ++i)
            if ((*this)[i] != s[i])
                return false;

        return true;
    }

    inline bool operator !=(String const &s) const
    {
        return !(*this == s);
    }

#ifdef __GNUC__
#   define LOL_FMT_ATTR(n, p) __attribute__((format(printf, n, p)))
#else
#   define LOL_FMT_ATTR(n, p)
#endif
    static String Printf(char const *format, ...) LOL_FMT_ATTR(1, 2);
#undef LOL_FMT_ATTR
    static String Printf(char const *format, va_list ap);
};

} /* namespace lol */

#endif // __LOL_BASE_STRING_H__

