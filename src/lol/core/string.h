//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The String class
// ----------------
// A very simple String class, based on Array.
//

#if !defined __LOL_STRING_H__
#define __LOL_STRING_H__

#include <lol/core/array.h>

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
        do
        {
            Push(*str);
        }
        while (*str++);
    }

    inline char &operator [](int n)
    {
        return ((Super &)*this)[n];
    }

    inline char const &operator [](int n) const
    {
        return ((Super const &)*this)[n];
    }

    inline String operator +(String const &s)
    {
        String ret(*this);
        return ret += s;
    }

    inline String operator +=(String const &s)
    {
        /* Be careful, we have a trailing zero we don't want! */
        --m_count;
        (Super &)*this += (Super const &)s;
        return *this;
    }
};

} /* namespace lol */

#endif // __LOL_STRING_H__

