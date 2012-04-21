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

template<typename T1, typename T2 = void, typename T3 = void> class Array
{
public:
    typedef struct { T1 m1; T2 m2; T3 m3; } Element;

    inline Array() : m_data(0), m_count(0), m_reserved(0) {}
    inline ~Array() { delete[] m_data; }

    inline Element& operator[](int n)
    {
        return m_data[n];
    }

    inline Element const& operator[](int n) const
    {
        return m_data[n];
    }

    inline Array<T1, T2, T3> const& operator+=(Element const &x)
    {
        Append(x.m1, x.m2, x.m3);
        return *this;
    }

    inline void Append(T1 const &m1, T2 const &m2, T3 const &m3)
    {
        if (m_count >= m_reserved)
            Reserve(m_count * 13 / 8 + 8);
        m_data[m_count].m1 = m1;
        m_data[m_count].m2 = m2;
        m_data[m_count].m3 = m3;
        ++m_count;
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

        Element *tmp = new Element[count];
        if (m_data)
        {
            memcpy(tmp, m_data, m_count * sizeof(Element));
            delete[] m_data;
        }
        m_data = tmp;
        m_reserved = count;
    }

    inline int Count() const { return m_count; }
    inline int Bytes() const { return m_count * sizeof(Element); }

private:
    Element *m_data;
    int m_count, m_reserved;
};

template<typename T1, typename T2> class Array<T1, T2, void>
{
public:
    typedef struct { T1 m1; T2 m2; } Element;

    inline Array() : m_data(0), m_count(0), m_reserved(0) {}
    inline ~Array() { delete[] m_data; }

    inline Element& operator[](int n)
    {
        return m_data[n];
    }

    inline Element const& operator[](int n) const
    {
        return m_data[n];
    }

    inline Array<T1, T2> const& operator+=(Element const &x)
    {
        Append(x.m1, x.m2);
        return *this;
    }

    inline void Append(T1 const &m1, T2 const &m2)
    {
        if (m_count >= m_reserved)
            Reserve(m_count * 13 / 8 + 8);
        m_data[m_count].m1 = m1;
        m_data[m_count].m2 = m2;
        ++m_count;
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

        Element *tmp = new Element[count];
        if (m_data)
        {
            memcpy(tmp, m_data, m_count * sizeof(Element));
            delete[] m_data;
        }
        m_data = tmp;
        m_reserved = count;
    }

    inline int Count() const { return m_count; }
    inline int Bytes() const { return m_count * sizeof(Element); }

private:
    Element *m_data;
    int m_count, m_reserved;
};

template<typename T1> class Array<T1, void, void>
{
public:
    inline Array() : m_data(0), m_count(0), m_reserved(0) {}
    inline ~Array() { delete[] m_data; }

    inline T1& operator[](int n)
    {
        return m_data[n];
    }

    inline T1 const& operator[](int n) const
    {
        return m_data[n];
    }

    inline Array<T1> const& operator+=(T1 const &x)
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

        T1 *tmp = new T1[count];
        if (m_data)
        {
            memcpy(tmp, m_data, m_count * sizeof(T1));
            delete[] m_data;
        }
        m_data = tmp;
        m_reserved = count;
    }

    inline int Count() const { return m_count; }
    inline int Bytes() const { return m_count * sizeof(T1); }

private:
    T1 *m_data;
    int m_count, m_reserved;
};

} /* namespace lol */

#endif // __LOL_ARRAY_H__

