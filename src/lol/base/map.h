//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The map class
// -------------
// A very simple map class.
//

#include <lol/base/hash.h>

namespace lol
{

/* A stupidly linear map for now. */
template<typename K, typename V> class map : protected hash<K>
{
public:
    /* If E is different from K, hash<K> must implement operator()(E const&)
     * and an equality operator between K and E must exist in order to use
     * this method. */

    /* I choose to make this inline because passing the key by reference
     * is usually suboptimal. */
    template <typename E>
    inline V const& operator[] (E const &key) const
    {
        /* Look for the hash in our table and return the value. */
        ptrdiff_t i = FindIndex(key);
        ASSERT(i >= 0, "trying to read a nonexistent key in map");
        return m_array[i].m3;
    }

    template <typename E>
    inline V & operator[] (E const &key)
    {
        /* Look for the hash in our table and return the value if found. */
        uint32_t hashed = ((hash<K> const &)*this)(key);
        ptrdiff_t i = FindIndex(key, hashed);
        if (i >= 0)
            return m_array[i].m3;

        /* If not found, insert a new value. */
        m_array.Push(hashed, key, V());
        return m_array.Last().m3;
    }

    template <typename E>
    inline void Remove(E const &key)
    {
        ptrdiff_t i = FindIndex(key);
        if (i >= 0)
            m_array.Remove(i);
    }

    template <typename E>
    inline bool HasKey(E const &key)
    {
        return FindIndex(key) >= 0;
    }

    template <typename E>
    inline bool TryGetValue(E const &key, V& value)
    {
        ptrdiff_t i = FindIndex(key);
        if (i >= 0)
        {
            value = m_array[i].m3;
            return true;
        }

        return false;
    }

    array<K> Keys() const
    {
        array<K> ret;
        for (auto it : m_array)
            ret.Push(it.m2);
        return ret;
    }

    inline ptrdiff_t Count() const
    {
        return m_array.Count();
    }

    inline void Empty()
    {
        m_array.Empty();
    }

private:
    template <typename E>
    inline ptrdiff_t FindIndex(E const &key, uint32_t hashed)
    {
        for (ptrdiff_t i = 0; i < m_array.Count(); ++i)
            if (m_array[i].m1 == hashed)
                if (m_array[i].m2 == key)
                    return i;
        return -1;
    }

    template <typename E>
    inline ptrdiff_t FindIndex(E const &key)
    {
        uint32_t hashed = ((hash<K> const &)*this)(key);
        return FindIndex(key, hashed);
    }

    array<uint32_t, K, V> m_array;
};

} /* namespace lol */

