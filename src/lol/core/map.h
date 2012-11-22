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
// The Map class
// -------------
// A very simple Map class.
//

#if !defined __LOL_CORE_MAP_H__
#define __LOL_CORE_MAP_H__

namespace lol
{

/* A stupidly linear map for now */
template<typename K, typename V> class Map : protected Hash<K>
{
public:
    inline V const& operator[] (K const &key) const
    {
        uint32_t hash = ((Hash<K> const &)*this)(key);
        for (int i = 0; i < m_array.Count(); ++i)
            if (m_array[i].m1 == hash)
                if (m_array[i].m2 == key)
                    return m_array[i].m3;
        return V();
    }

    inline V & operator[] (K const &key)
    {
        return const_cast<V &>((const_cast<Map<K,V> const &>(*this))[key]);
    }

    inline V & Set(K const &key, V const &val)
    {
        uint32_t hash = ((Hash<K> const &)*this)(key);
        for (int i = 0; i < m_array.Count(); ++i)
            if (m_array[i].m1 == hash)
                if (m_array[i].m2 == key)
                {
                    m_array[i].m3.~V();
                    return m_array[i].m3 = val;
                }

        m_array.Push(hash, key, val);
        return m_array.Last().m3;
    }

    inline void Remove(K const &key)
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

private:
    Array<uint32_t, K, V> m_array;
};

} /* namespace lol */

#endif // __LOL_CORE_MAP_H__

