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

//
// The map class
// -------------
// A very simple map class.
//

#include <lol/base/avl_tree.h>
#include <lol/base/hash.h>

namespace lol
{

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
        V *value_ptr = nullptr;
        bool found = m_tree.try_get(key, value_ptr);

        ASSERT(found, "trying to read a nonexistent key in map");

        return *value_ptr;
    }

    template <typename E>
    inline V & operator[] (E const &key)
    {
        /* Look for the hash in our table and return the value if found. */
        K typed_key(key);
        V *value_ptr = nullptr;

        bool found = m_tree.try_get(key, value_ptr);
        if (!found)
        {
            /* If not found, insert a new value. */
            m_tree.insert(typed_key, V());
            found = m_tree.try_get(key, value_ptr);
        }

        /* This may happen if the key operator < does not behave as
         * a comparator (i.e. doesn’t enforce a<b => !b<a) */
        ASSERT(found, "inserted key can’t be retrieved");

        return *value_ptr;
    }

    template <typename E>
    inline void remove(E const &key)
    {
        K typed_key(key);
        m_tree.erase(typed_key);
    }

    template <typename E>
    inline bool has_key(E const &key)
    {
        K typed_key(key);
        return m_tree.exists(typed_key);
    }

    template <typename E>
    inline bool try_get(E const &key, V& value)
    {
        K typed_key(key);
        V *value_ptr;
        if (m_tree.try_get(typed_key, value_ptr))
        {
            value = *value_ptr;
            return true;
        }

        return false;
    }

    array<K> keys() const
    {
        array<K> ret;

        for (auto iterator : m_tree)
            ret.Push(iterator.key);

        return ret;
    }

    inline ptrdiff_t count() const
    {
        return m_tree.count();
    }

    inline void empty()
    {
        m_tree.clear();
    }

private:
    avl_tree<K, V> m_tree;
};

} /* namespace lol */

