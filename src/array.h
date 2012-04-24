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
// A very simple Array class not unlike the std::vector, with some nice
// additional features, eg. Array<int,float> for automatic arrays of structs.
//

#if !defined __LOL_ARRAY_H__
#define __LOL_ARRAY_H__

#include <stdint.h>

namespace lol
{

/*
 * The base array type
 * FIXME: only works properly with POD types for now
 */

template<typename T> class ArrayBase
{
public:
    typedef T Element;

    inline ArrayBase() : m_data(0), m_count(0), m_reserved(0) {}
    inline ~ArrayBase() { delete[] m_data; }

    ArrayBase(ArrayBase const& that) : m_data(0), m_count(0), m_reserved(0)
    {
        Reserve(that.m_reserved);
        memcpy(m_data, that.m_data, that.m_count * sizeof(Element));
        m_count = that.m_count;
    }

    ArrayBase& operator=(ArrayBase const& that)
    {
        /* FIXME: delete old data!! */
        m_data = 0;
        m_count = 0;
        m_reserved = 0;
        Reserve(that.m_reserved);
        memcpy(m_data, that.m_data, that.m_count * sizeof(Element));
        m_count = that.m_count;
        return *this;
    }

    inline Element& operator[](int n)
    {
        return m_data[n];
    }

    inline Element const& operator[](int n) const
    {
        return m_data[n];
    }

    inline ArrayBase<T> const& operator<<(T const &x)
    {
        if (m_count >= m_reserved)
        {
            T tmp = x;
            Reserve(m_count * 13 / 8 + 8);
            m_data[m_count++] = tmp;
        }
        else
        {
            m_data[m_count++] = x;
        }
        return *this;
    }

    inline void Append(T const &x)
    {
        *this << x;
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

protected:
    Element *m_data;
    int m_count, m_reserved;
};

/*
 * Element types
 */

template<typename T1, typename T2, typename T3 = void, typename T4 = void,
         typename T5 = void, typename T6 = void, typename T7 = void,
         typename T8 = void>
class ArrayElement
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5; T6 m6; T7 m7; T8 m8;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6, typename T7>
class ArrayElement<T1, T2, T3, T4, T5, T6, T7, void>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5; T6 m6; T7 m7;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6>
class ArrayElement<T1, T2, T3, T4, T5, T6, void, void>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5; T6 m6;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
class ArrayElement<T1, T2, T3, T4, T5, void, void, void>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5;
};

template<typename T1, typename T2, typename T3, typename T4>
class ArrayElement<T1, T2, T3, T4, void, void, void, void>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4;
};

template<typename T1, typename T2, typename T3>
class ArrayElement<T1, T2, T3, void, void, void, void, void>
{
public:
    T1 m1; T2 m2; T3 m3;
};

template<typename T1, typename T2>
class ArrayElement<T1, T2, void, void, void, void, void, void>
{
public:
    T1 m1; T2 m2;
};

/*
 * Array specialisations implementing specific setters
 */

template<typename T1, typename T2 = void, typename T3 = void,
         typename T4 = void, typename T5 = void, typename T6 = void,
         typename T7 = void, typename T8 = void>
class Array : public ArrayBase<ArrayElement<T1, T2, T3, T4, T5, T6, T7, T8> >
{
public:
    inline void Append(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4,
                       T5 const &m5, T6 const &m6, T7 const &m7, T8 const &m8)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            T5 tmp5 = m5; T6 tmp6 = m6; T7 tmp7 = m7; T8 tmp8 = m8;
            Reserve(this->m_count * 13 / 8 + 8);
            this->m_data[this->m_count].m1 = tmp1;
            this->m_data[this->m_count].m2 = tmp2;
            this->m_data[this->m_count].m3 = tmp3;
            this->m_data[this->m_count].m4 = tmp4;
            this->m_data[this->m_count].m5 = tmp5;
            this->m_data[this->m_count].m6 = tmp6;
            this->m_data[this->m_count].m7 = tmp7;
            this->m_data[this->m_count].m8 = tmp8;
        }
        else
        {
            this->m_data[this->m_count].m1 = m1;
            this->m_data[this->m_count].m2 = m2;
            this->m_data[this->m_count].m3 = m3;
            this->m_data[this->m_count].m4 = m4;
            this->m_data[this->m_count].m5 = m5;
            this->m_data[this->m_count].m6 = m6;
            this->m_data[this->m_count].m7 = m7;
            this->m_data[this->m_count].m8 = m8;
        }
        ++this->m_count;
    }
};

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6, typename T7>
class Array<T1, T2, T3, T4, T5, T6, T7, void>
  : public ArrayBase<ArrayElement<T1, T2, T3, T4, T5, T6, T7, void> >
{
public:
    inline void Append(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4,
                       T5 const &m5, T6 const &m6, T7 const &m7)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            T5 tmp5 = m5; T6 tmp6 = m6; T7 tmp7 = m7;
            Reserve(this->m_count * 13 / 8 + 8);
            this->m_data[this->m_count].m1 = tmp1;
            this->m_data[this->m_count].m2 = tmp2;
            this->m_data[this->m_count].m3 = tmp3;
            this->m_data[this->m_count].m4 = tmp4;
            this->m_data[this->m_count].m5 = tmp5;
            this->m_data[this->m_count].m6 = tmp6;
            this->m_data[this->m_count].m7 = tmp7;
        }
        else
        {
            this->m_data[this->m_count].m1 = m1;
            this->m_data[this->m_count].m2 = m2;
            this->m_data[this->m_count].m3 = m3;
            this->m_data[this->m_count].m4 = m4;
            this->m_data[this->m_count].m5 = m5;
            this->m_data[this->m_count].m6 = m6;
            this->m_data[this->m_count].m7 = m7;
        }
        ++this->m_count;
    }
};

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6>
class Array<T1, T2, T3, T4, T5, T6, void, void>
  : public ArrayBase<ArrayElement<T1, T2, T3, T4, T5, T6, void, void> >
{
public:
    inline void Append(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4,
                       T5 const &m5, T6 const &m6)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            T5 tmp5 = m5; T6 tmp6 = m6;
            Reserve(this->m_count * 13 / 8 + 8);
            this->m_data[this->m_count].m1 = tmp1;
            this->m_data[this->m_count].m2 = tmp2;
            this->m_data[this->m_count].m3 = tmp3;
            this->m_data[this->m_count].m4 = tmp4;
            this->m_data[this->m_count].m5 = tmp5;
            this->m_data[this->m_count].m6 = tmp6;
        }
        else
        {
            this->m_data[this->m_count].m1 = m1;
            this->m_data[this->m_count].m2 = m2;
            this->m_data[this->m_count].m3 = m3;
            this->m_data[this->m_count].m4 = m4;
            this->m_data[this->m_count].m5 = m5;
            this->m_data[this->m_count].m6 = m6;
        }
        ++this->m_count;
    }
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
class Array<T1, T2, T3, T4, T5, void, void, void>
  : public ArrayBase<ArrayElement<T1, T2, T3, T4, T5, void, void, void> >
{
public:
    inline void Append(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4,
                       T5 const &m5)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            T5 tmp5 = m5;
            Reserve(this->m_count * 13 / 8 + 8);
            this->m_data[this->m_count].m1 = tmp1;
            this->m_data[this->m_count].m2 = tmp2;
            this->m_data[this->m_count].m3 = tmp3;
            this->m_data[this->m_count].m4 = tmp4;
            this->m_data[this->m_count].m5 = tmp5;
        }
        else
        {
            this->m_data[this->m_count].m1 = m1;
            this->m_data[this->m_count].m2 = m2;
            this->m_data[this->m_count].m3 = m3;
            this->m_data[this->m_count].m4 = m4;
            this->m_data[this->m_count].m5 = m5;
        }
        ++this->m_count;
    }
};

template<typename T1, typename T2, typename T3, typename T4>
class Array<T1, T2, T3, T4, void, void, void, void>
  : public ArrayBase<ArrayElement<T1, T2, T3, T4, void, void, void, void> >
{
public:
    inline void Append(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            Reserve(this->m_count * 13 / 8 + 8);
            this->m_data[this->m_count].m1 = tmp1;
            this->m_data[this->m_count].m2 = tmp2;
            this->m_data[this->m_count].m3 = tmp3;
            this->m_data[this->m_count].m4 = tmp4;
        }
        else
        {
            this->m_data[this->m_count].m1 = m1;
            this->m_data[this->m_count].m2 = m2;
            this->m_data[this->m_count].m3 = m3;
            this->m_data[this->m_count].m4 = m4;
        }
        ++this->m_count;
    }
};

template<typename T1, typename T2, typename T3>
class Array<T1, T2, T3, void, void, void, void, void>
  : public ArrayBase<ArrayElement<T1, T2, T3, void, void, void, void, void> >
{
public:
    inline void Append(T1 const &m1, T2 const &m2, T3 const &m3)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3;
            Reserve(this->m_count * 13 / 8 + 8);
            this->m_data[this->m_count].m1 = tmp1;
            this->m_data[this->m_count].m2 = tmp2;
            this->m_data[this->m_count].m3 = tmp3;
        }
        else
        {
            this->m_data[this->m_count].m1 = m1;
            this->m_data[this->m_count].m2 = m2;
            this->m_data[this->m_count].m3 = m3;
        }
        ++this->m_count;
    }
};

template<typename T1, typename T2>
class Array<T1, T2, void, void, void, void, void, void>
  : public ArrayBase<ArrayElement<T1, T2, void, void, void, void, void, void> >
{
public:
    inline void Append(T1 const &m1, T2 const &m2)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2;
            Reserve(this->m_count * 13 / 8 + 8);
            this->m_data[this->m_count].m1 = tmp1;
            this->m_data[this->m_count].m2 = tmp2;
        }
        else
        {
            this->m_data[this->m_count].m1 = m1;
            this->m_data[this->m_count].m2 = m2;
        }
        ++this->m_count;
    }
};

template<typename T>
class Array<T, void, void, void, void, void, void, void> : public ArrayBase<T>
{
};

} /* namespace lol */

#endif // __LOL_ARRAY_H__

