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

#include <lol/base/avl_tree.h>
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
        V * value_ptr = nullptr;
        ASSERT(m_tree.TryGetValue(key, value_ptr), "trying to read a nonexistent key in map");

        return *value_ptr;
    }

    template <typename E>
    inline V & operator[] (E const &key)
    {
        /* Look for the hash in our table and return the value if found. */

        K typed_key(key);
        V * value_ptr = nullptr;

        if (!m_tree.TryGetValue(typed_key, value_ptr))
        {
            V default_value = V();
            m_tree.Insert(typed_key, default_value);
        }

        ASSERT(m_tree.TryGetValue(typed_key, value_ptr), "inner tree is messed up in map");

        /* If not found, insert a new value. */
        return *value_ptr;
    }

    template <typename E>
    inline void Remove(E const &key)
    {
        K typed_key(key);
        m_tree.Erase(typed_key);
    }

    template <typename E>
    inline bool HasKey(E const &key)
    {
        K typed_key(key);
        return m_tree.Exists(typed_key);
    }

    template <typename E>
    inline bool TryGetValue(E const &key, V& value)
    {
        K typed_key(key);
        V * value_ptr;
        if (m_tree.TryGetValue(typed_key, value_ptr))
        {
            value = *value_ptr;
            return true;
        }

        return false;
    }

    array<K> Keys() const
    {
        array<K> ret;

        for (auto iterator : m_tree)
            ret.Push(iterator.key);

        return ret;
    }

    inline ptrdiff_t Count() const
    {
        return m_tree.GetCount();
    }

    inline void Empty()
    {
        m_tree.Clear();
    }

private:

    avl_tree<K, V> m_tree;
};

} /* namespace lol */

