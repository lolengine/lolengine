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

#include <lol/base/assert.h>
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
        ASSERT(str);
        Resize((int)strlen(str));
        memcpy(&(*this)[0], str, Count() + 1);
    }

    inline String(char const *str, int count)
      : Super()
    {
        using namespace std;
        ASSERT(str);
        Resize(count);
        memcpy(&(*this)[0], str, count);
        ((Super &)*this)[count] = '\0';
    }

    inline String(String const &s)
      : Super((Super const &)s)
    {
    }

    inline char &operator [](int n)
    {
        /* Allow n == Count() because we might have reasonable reasons
         * to access that hidden null character. */
        ASSERT(n >= 0);
        ASSERT((unsigned)n <= (unsigned)Count());
        return ((Super &)*this)[n];
    }

    inline char const &operator [](int n) const
    {
        ASSERT(n >= 0);
        ASSERT(n <= Count());
        return ((Super const &)*this)[n];
    }

    inline char &Last()
    {
        ASSERT(Count() > 0);
        return (*this)[Count() - 1];
    }

    inline char const &Last() const
    {
        ASSERT(Count() > 0);
        return (*this)[Count() - 1];
    }

    inline int Count() const
    {
        return ((Super const &)*this).Count() - 1;
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
    void Resize(int count)
    {
        ASSERT(count >= 0);
        ((Super &)*this).Resize(count + 1);
        ((Super &)*this).Last() = '\0';
    }

    String Sub(int start, int count) const
    {
        ASSERT(start >= 0);
        ASSERT(count >= 0);
        ASSERT(start + count <= Count());
        return String(&(*this)[start], count);
    }

    int IndexOf(char token) const
    {
        using namespace std;

        char const *tmp = strchr(C(), token);
        return tmp ? (int)(intptr_t)(tmp - C()) : INDEX_NONE;
    }

    int IndexOf(char const* token) const
    {
        using namespace std;

        char const *tmp = strstr(C(), token);
        return tmp ? (int)(intptr_t)(tmp - C()) : INDEX_NONE;
    }

    bool Contains(String const &s) const
    {
        return IndexOf(s.C()) != INDEX_NONE;
    }

    int LastIndexOf(char token) const
    {
        using namespace std;

        char const *tmp = strrchr(C(), token);
        return tmp ? (int)(intptr_t)(tmp - C()) : INDEX_NONE;
    }

    int Replace(const char old_token, const char new_token, bool all_occurence=false)
    {
        using namespace std;

        int res = 0;
        char *tmp = NULL;
        while ((tmp = strrchr(C(), old_token)))
        {
            *tmp = new_token;
            res++;
            if (!all_occurence)
                break;
        }
        return res;
    }

    inline String& ToLower()
    {
        char* p = C();
        for (int i = 0; i < Count(); ++i)
            if ('A' <= p[i] && p[i] <= 'Z')
                p[i] += 'a' - 'A';
        return *this;
    }

    inline String& ToUpper()
    {
        char* p = C();
        for (int i = 0; i < Count(); ++i)
            if ('a' <= p[i] && p[i] <= 'z')
                p[i] += 'A' - 'a';
        return *this;
    }

    int LastIndexOf(char const* token) const
    {
        using namespace std;

        int token_len = (int)strlen(token);
        for (int i = Count() - token_len; i >= 0; --i)
            if (strstr(C() + i, token))
                return i;
        return -1;
    }

    bool StartsWith(String const &s) const
    {
        using namespace std;
        return Count() >= s.Count()
                && memcmp(C(), s.C(), s.Count()) == 0;
    }

    bool EndsWith(String const &s) const
    {
        using namespace std;
        return Count() >= s.Count()
                && memcmp(C() + Count() - s.Count(), s.C(), s.Count()) == 0;
    }

    bool IsAlpha()
    {
        for (int i = 0; i < m_count; i++)
            if (m_data[i] != '\0' && (m_data[i] < '0' || '9' < m_data[i]))
                return false;
        return true;
    }

    inline String operator +(String const &s) const
    {
        String ret(*this);
        return ret += s;
    }

    inline String& operator +=(String const &s)
    {
        using namespace std;
        int old_count = Count();
        Resize(Count() + s.Count());
        memcpy(&(*this)[old_count], &s[0], Count() - old_count);
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
        using namespace std;
        return Count() == s.Count()
                && memcmp(C(), s.C(), Count()) == 0;
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
        return Count() == sz_len
                && memcmp(C(), sz, sz_len) == 0;
    }

    inline bool operator !=(char const* sz) const
    {
        return !(*this == sz);
    }

#ifdef __GNUC__
#   define LOL_FMT_ATTR(n, p) __attribute__((format(printf, n, p)))
#else
#   define LOL_FMT_ATTR(n, p)
#endif
    static String Printf(char const *format, ...) LOL_FMT_ATTR(1, 2);
#undef LOL_FMT_ATTR
    static String VPrintf(char const *format, va_list ap);
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

#endif // __LOL_BASE_STRING_H__

