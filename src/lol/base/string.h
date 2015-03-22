//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2013—2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The String class
// ----------------
// A very simple String class, based on Array.
//

#include <lol/base/assert.h>
#include <lol/base/array.h>

#include <cstring>
#include <cstdarg>

namespace lol
{

class String : protected array<char>
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
        resize(strlen(str));
        memcpy(&(*this)[0], str, count() + 1);
    }

    inline String(char const *str, ptrdiff_t item_count)
      : super()
    {
        using namespace std;
        ASSERT(str);
        resize(item_count);
        memcpy(&(*this)[0], str, item_count);
        ((super &)*this)[item_count] = '\0';
    }

    inline String(String const &s)
      : super((super const &)s)
    {
    }

    inline char &operator [](ptrdiff_t n)
    {
        /* Allow n == count() because we might have reasonable reasons
         * to access that hidden null character. */
        ASSERT(n >= 0);
        ASSERT(n <= count());
        return ((super &)*this)[n];
    }

    inline char const &operator [](ptrdiff_t n) const
    {
        ASSERT(n >= 0);
        ASSERT(n <= count());
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

    inline ptrdiff_t count() const
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
    void resize(ptrdiff_t item_count)
    {
        ASSERT(item_count >= 0);
        ((super &)*this).resize(item_count + 1);
        ((super &)*this).last() = '\0';
    }

    String sub(ptrdiff_t start, ptrdiff_t item_count = -1) const
    {
        ASSERT(start >= 0);
        if (start >= count())
            return String();
        if (item_count < 0 || item_count >= count() - start)
            item_count = count() - start;
        return String(&(*this)[start], item_count);
    }

    bool contains(String const &s) const
    {
        return index_of(s.C()) != INDEX_NONE;
    }

    ptrdiff_t index_of(char token) const
    {
        using namespace std;

        char const *tmp = strchr(C(), token);
        return tmp ? (ptrdiff_t)(tmp - C()) : INDEX_NONE;
    }

    ptrdiff_t index_of(String const& token) const { return index_of(token.C()); }
    ptrdiff_t index_of(char const* token) const
    {
        using namespace std;

        char const *tmp = strstr(C(), token);
        return tmp ? (ptrdiff_t)(tmp - C()) : INDEX_NONE;
    }

    ptrdiff_t last_index_of(char token) const
    {
        using namespace std;

        char const *tmp = strrchr(C(), token);
        return tmp ? (ptrdiff_t)(tmp - C()) : INDEX_NONE;
    }

    ptrdiff_t last_index_of(String const& token) const { return last_index_of(token.C()); }
    ptrdiff_t last_index_of(char const* token) const
    {
        using namespace std;

        ptrdiff_t token_len = strlen(token);
        for (ptrdiff_t i = count() - token_len; i >= 0; --i)
        if (strstr(C() + i, token))
            return i;
        return -1;
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

    inline String& to_lower()
    {
        String ret(*this);
        for (ptrdiff_t i = 0; i < ret.count(); ++i)
        {
            if ('A' <= ret[i] && ret[i] <= 'Z')
                ret[i] += 'a' - 'A';
        }
        *this = ret;
        return *this;
    }

    inline String& to_upper()
    {
        String ret(*this);
        for (ptrdiff_t i = 0; i < ret.count(); ++i)
        {
            if ('a' <= ret[i] && ret[i] <= 'z')
                ret[i] += 'A' - 'a';
        }
        *this = ret;
        return *this;
    }
    inline String& case_change(bool case_to_upper)
    {
        return case_to_upper ? to_upper() : to_lower();
    }

    bool starts_with(String const &s) const
    {
        using namespace std;
        return count() >= s.count()
                && memcmp(C(), s.C(), s.count()) == 0;
    }

    bool ends_with(String const &s) const
    {
        using namespace std;
        return count() >= s.count()
                && memcmp(C() + count() - s.count(), s.C(), s.count()) == 0;
    }

    bool is_alpha() const
    {
        for (ptrdiff_t i = 0; i < m_count; i++)
            if (m_data[i] != '\0' && (m_data[i] < '0' || '9' < m_data[i]))
                return false;
        return true;
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
        ptrdiff_t old_count = count();
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
        ptrdiff_t sz_len = strlen(sz);
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

#ifdef __GNUC__
#   define LOL_FMT_ATTR(n, p) __attribute__((format(printf, n, p)))
#else
#   define LOL_FMT_ATTR(n, p)
#endif
    static String Printf(char const *format, ...) LOL_FMT_ATTR(1, 2);
#undef LOL_FMT_ATTR
    static String VPrintf(char const *format, va_list ap);

    /* TODO: remove these legacy functions one day */
    inline char &Last() { return last(); }
    inline char const &Last() const { return last(); }
    inline void Resize(ptrdiff_t item_count) { return resize(item_count); }
    inline String& ToLower() { return to_lower(); }
    inline String& ToUpper() { return to_upper(); }
    inline String& CaseChange(bool case_to_upper) { return case_to_upper ? ToUpper() : ToLower(); }
    inline String Sub(ptrdiff_t start, ptrdiff_t item_count = -1) const { return sub(start, item_count); }
    inline bool Contains(String const &s) const { return contains(s); }
    inline ptrdiff_t IndexOf(char token) const { return index_of(token); }
    inline ptrdiff_t IndexOf(char const* token) const { return index_of(token); }
    inline ptrdiff_t LastIndexOf(char token) const { return last_index_of(token); }
    inline int Replace(char const old_token, char const new_token, bool all_occurrences = false) { return replace(old_token, new_token, all_occurrences); }
    inline bool EndsWith(String const &s) const { return ends_with(s); }
    inline bool IsAlpha() const { return is_alpha(); }
    inline bool StartsWith(String const &s) const { return starts_with(s); }
    inline ptrdiff_t Count() const { return count(); }
};

class Line
{
public:
    Line(String const& s)
    {
        m_line = s + "\n";
    }
    inline operator String()
    {
        return m_line;
    }
    inline operator const char*()
    {
        return m_line.C();
    }

private:
    String m_line;
};

inline bool operator ==(char const* sz, String const &s)
{
    return s == sz;
}

inline bool operator !=(char const* sz, String const &s)
{
    return s != sz;
}

} /* namespace lol */

