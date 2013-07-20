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
// The Map class
// -------------
// A very simple Map class.
//

#if !defined __LOL_BASE_MAP_H__
#define __LOL_BASE_MAP_H__

namespace lol
{

/* A stupidly linear map for now. */
template<typename K, typename V> class Map : protected Hash<K>
{
public:
    /* If E is different from K, Hash<K> must implement operator()(E const&)
     * and an equality operator between K and E must exist in order to use this method.
      */

    /* I choose to make this inline because passing the key by reference
     * is usually suboptimal. */
    template <typename E>
    inline V const& operator[] (E const &key) const
    {
        /* Look for the hash in our table and return the value. */
        uint32_t hash = ((Hash<K> const &)*this)(key);
        for (int i = 0; i < m_array.Count(); ++i)
            if (m_array[i].m1 == hash)
                if (m_array[i].m2 == key)
                    return m_array[i].m3;
        /* XXX: this in an error! */
        ASSERT(0, "trying to read a nonexistent key in map");
        return V();
    }

    template <typename E>
    inline V & operator[] (E const &key)
    {
        /* Look for the hash in our table and return the value if found. */
        uint32_t hash = ((Hash<K> const &)*this)(key);
        for (int i = 0; i < m_array.Count(); ++i)
            if (m_array[i].m1 == hash)
                if (m_array[i].m2 == key)
                    return m_array[i].m3;
        /* If not found, insert a new value. */
        m_array.Push(hash, key, V());
        return m_array.Last().m3;
    }

    template <typename E>
    inline void Remove(E const &key)
    {
        uint32_t hash = ((Hash<K> const &)*this)(key);
        for (int i = 0; i < m_array.Count(); ++i)
            if (m_array[i].m1 == hash)
                if (m_array[i].m2 == key)
                {
                    m_array.Remove(i);
                    return;
                }
    }

    template <typename E>
    inline bool HasKey(E const &key)
    {
        uint32_t hash = ((Hash<K> const &)*this)(key);
        for (int i = 0; i < m_array.Count(); ++i)
            if (m_array[i].m1 == hash)
                if (m_array[i].m2 == key)
                    return true;
        return false;
    }

    template <typename E>
    inline bool TryGetValue(E const &key, V& value)
    {
        uint32_t hash = ((Hash<K> const &)*this)(key);
        for (int i = 0; i < m_array.Count(); ++i)
            if (m_array[i].m1 == hash)
                if (m_array[i].m2 == key)
                {
                    value = m_array[i].m3;
                    return true;
                }

        return false;
    }

    inline int Count() const { return m_array.Count(); }

private:
    Array<uint32_t, K, V> m_array;
};

} /* namespace lol */

#endif // __LOL_BASE_MAP_H__

