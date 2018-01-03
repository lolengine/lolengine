//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//            © 2013—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The string tools
// ----------------
// Contains some utilities to work with std::string objects.
//

#include <string>

namespace lol
{

/* Split a string along a single separator */
array<std::string> split(std::string const &s, char sep = '\n');

/* Split a string along multiple separators */
array<std::string> split(std::string const &s, std::string const &seps);

/* Check whether a string starts or ends with a given substring */
bool starts_with(std::string const &s, std::string const &prefix);
bool ends_with(std::string const &s, std::string const &suffix);

/* Convert a string to lowercase or uppercase */
std::string tolower(std::string const &s);
std::string toupper(std::string const &s);

/* Format a string, printf-style */
std::string format(char const *format, ...) LOL_ATTR_FORMAT(1, 2);
std::string vformat(char const *format, va_list ap);

} /* namespace lol */

//
// The deprecated String class
// ---------------------------
// A very simple String class, based on Array. The most interesting
// thing in there was LOL_ATTR_NODISCARD but apart from that there was
// no real point in using our own class. Phase this out.
//

#include <lol/base/assert.h>
#include <lol/base/array.h>

#include <cstring>
#include <cstdarg>

namespace lol
{

class LOL_ATTR_NODISCARD String : protected array<char>
{
private:
    typedef array<char> super;

public:
    inline String()
      : super()
    {
        push('\0');
    }

    inline String(char const *str)
      : super()
    {
        using namespace std;
        ASSERT(str);
        resize((int)strlen(str));
        memcpy(&(*this)[0], str, count() + 1);
    }

    inline String(char const *str, int item_count)
      : super()
    {
        using namespace std;
        ASSERT(str);
        resize(item_count);
        memcpy(&(*this)[0], str, item_count);
        ((super &)*this)[item_count] = '\0';
    }

    /* Legacy constructor for std::string */
    inline String(std::string const &s)
      : String(s.c_str())
    {
    }

    inline String(String const &s)
      : super((super const &)s)
    {
    }

    inline char &operator [](int n)
    {
        /* Allow n == count() because we might have reasonable reasons
         * to access that hidden null character. We cast to unsigned so
         * as to avoid a harmless message from the GCC optimiser. */
        ASSERT(n >= 0);
        ASSERT((unsigned)n <= (unsigned)count());
        return ((super &)*this)[n];
    }

    inline char const &operator [](int n) const
    {
        ASSERT(n >= 0);
        ASSERT((unsigned)n <= (unsigned)count());
        return ((super const &)*this)[n];
    }

    inline char &last()
    {
        ASSERT(count() > 0);
        return (*this)[count() - 1];
    }

    inline char const &last() const
    {
        ASSERT(count() > 0);
        return (*this)[count() - 1];
    }

    inline int count() const
    {
        return ((super const &)*this).count() - 1;
    }

    /* Return a C string */
    inline char const *C() const
    {
        return &(*this)[0];
    }

    /* Non-const version; make sure to not overflow the internal array */
    inline char *C()
    {
        return &(*this)[0];
    }

    /* Does not initialise the newly allocated characters */
    void resize(int item_count)
    {
        ASSERT(item_count >= 0);
        ((super &)*this).resize(item_count + 1);
        ((super &)*this).last() = '\0';
    }

    String sub(int start, int item_count = -1) const
    {
        ASSERT(start >= 0);
        if (start >= count())
            return String();
        if (item_count < 0 || item_count >= count() - start)
            item_count = count() - start;
        return String(&(*this)[start], item_count);
    }

    int replace(char const old_token, char const new_token,
                bool all_occurrences = false)
    {
        using namespace std;

        int res = 0;
        char *tmp = NULL;
        while ((tmp = strrchr(C(), old_token)))
        {
            *tmp = new_token;
            res++;
            if (!all_occurrences)
                break;
        }
        return res;
    }

    inline String operator +(String const &s) const
    {
        String ret(*this);
        return ret += s;
    }

    inline String operator +(char c) const
    {
        String ret(*this);
        return ret += c;
    }

    inline String& operator +=(String const &s)
    {
        using namespace std;
        int old_count = count();
        resize(count() + s.count());
        memcpy(&(*this)[old_count], &s[0], count() - old_count);
        return *this;
    }

    inline String& operator +=(char c)
    {
        ((super &)*this).last() = c;
        ((super &)*this).push('\0');
        return *this;
    }

    inline bool operator ==(String const &s) const
    {
        using namespace std;
        return count() == s.count()
                && memcmp(C(), s.C(), count()) == 0;
    }

    inline bool operator !=(String const &s) const
    {
        return !(*this == s);
    }

    inline bool operator ==(char const* sz) const
    {
        /* We parse the C string twice because of strlen + memcmp
         * but it's probably still faster than doing it by hand. */
        using namespace std;
        int sz_len = (int)strlen(sz);
        return count() == sz_len
                && memcmp(C(), sz, sz_len) == 0;
    }

    inline bool operator !=(char const* sz) const
    {
        return !(*this == sz);
    }

    inline bool operator <(String const & s) const
    {
        using namespace std;
        int res = memcmp(C(), s.C(), count() < s.count() ? count() : s.count());

        if (!res)
            return count() < s.count();

        return res < 0;
    }
};

inline String operator +(char c, String const &s)
{
    return String() + c + s;
}

inline String operator +(char const *sz, String const &s)
{
    return String(sz) + s;
}

inline bool operator ==(char const* sz, String const &s)
{
    return s == sz;
}

inline bool operator !=(char const* sz, String const &s)
{
    return s != sz;
}

} /* namespace lol */

