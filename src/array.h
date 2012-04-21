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
// The Array class
// ---------------
// A very simple Array class not unlike the std::vector.
//

#if !defined __LOL_ARRAY_H__
#define __LOL_ARRAY_H__

#include <stdint.h>

namespace lol
{

template<typename T> class Array
{
public:
    inline Array() : m_data(0), m_count(0), m_reserved(0) {}
    inline ~Array() { delete[] m_data; }

    inline T& operator[](int n)
    {
        return m_data[n];
    }

    inline T const& operator[](int n) const
    {
        return m_data[n];
    }

    inline Array<T> const& operator+=(T const &x)
    {
        if (m_count >= m_reserved)
            Reserve(m_count * 13 / 8 + 8);
        m_data[m_count++] = x;
        return *this;
    }

    void Remove(int pos)
    {
        memmove(m_data + pos, m_data + pos + 1, m_count - pos - 1);
        m_count--;
    }

    void Remove(int pos, int count)
    {
        memmove(m_data + pos, m_data + pos + count, m_count - pos - count);
        m_count -= count;
    }

    void Reserve(int count)
    {
        if (count <= (int)m_reserved)
            return;

        T *tmp = new T[count];
        if (m_data)
        {
            memcpy(tmp, m_data, m_count * sizeof(T));
            delete[] m_data;
        }
        m_data = tmp;
        m_reserved = count;
    }

    inline size_t Count() const { return m_count; }
    inline size_t Bytes() const { return m_count * sizeof(T); }

private:
    T *m_data;
    size_t m_count, m_reserved;
};

} /* namespace lol */

#endif // __LOL_ARRAY_H__

