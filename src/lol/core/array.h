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

#if !defined __LOL_CORE_ARRAY_H__
#define __LOL_CORE_ARRAY_H__

#include <new>
#include <stdint.h>

namespace lol
{

/*
 * The base array type.
 *
 * Contains an m_data memory array of Elements, of which only the first
 * m_count are allocated. The rest is uninitialised memory.
 */

template<typename T, typename ARRAY> class ArrayBase
{
public:
    typedef T Element;

    inline ArrayBase() : m_data(0), m_count(0), m_reserved(0)
    {
    }

    inline ~ArrayBase()
    {
        for (int i = 0; i < m_count; i++)
            m_data[i].~Element();
        delete[] reinterpret_cast<uint8_t *>(m_data);
    }

    ArrayBase(ArrayBase const& that) : m_data(0), m_count(0), m_reserved(0)
    {
        /* Reserve the exact number of values instead of what the other
         * array had reserved. Just a method for not wasting too much. */
        Reserve(that.m_count);
        for (int i = 0; i < that.m_count; i++)
            new(&m_data[i]) Element(that[i]);
        m_count = that.m_count;
    }

    ArrayBase& operator=(ArrayBase const& that)
    {
        if ((uintptr_t)this != (uintptr_t)&that)
        {
            if (m_reserved < that.m_count)
            {
                /* If not enough space, reserve memory and use placement
                 * new directly for all elements. */
                Reserve(that.m_count);
                for (int i = 0; i < that.m_count; i++)
                    new(&m_data[i]) Element(that[i]);
            }
            else
            {
                /* If enough space, overwrite the common elements, then
                 * use placement new for the elements in the other array
                 * that we do not have, and finally destroy the remaining
                 * elements. */
                for (int i = 0; i < m_count && i < that.m_count; i++)
                    m_data[i] = Element(that[i]);
                for (int i = m_count; i < that.m_count; i++)
                    new(&m_data[i]) Element(that[i]);
                for (int i = that.m_count; i < m_count; i++)
                    m_data[i].~Element();
            }
            m_count = that.m_count;
        }
        return *this;
    }

    ArrayBase& operator+=(ARRAY const &that)
    {
        int todo = that.m_count;
        Reserve(m_count + that.m_count);
        for (int i = 0; i < todo; i++)
            *this << that[i];
        return *this;
    }

    ARRAY operator+(ARRAY const &that) const
    {
        /* FIXME: upon return, this makes a copy of the temporary object;
         * use either C++11 move semantics, or add a special flag to the
         * object indicating we're a temporary about to be destroyed */
        ARRAY ret;
        ret.Reserve(m_count + that.m_count);
        for (int i = 0; i < m_count; i++)
            ret << (*this)[i];
        for (int i = 0; i < that.m_count; i++)
            ret << that[i];
        return ret;
    }

    inline Element& operator[](int n)
    {
        return m_data[n];
    }

    inline Element const& operator[](int n) const
    {
        return m_data[n];
    }

    inline Element& Last()
    {
        return m_data[m_count - 1];
    }

    inline Element const& Last() const
    {
        return m_data[m_count - 1];
    }

    inline ArrayBase& operator<<(T const &x)
    {
        if (m_count >= m_reserved)
        {
            T tmp = x;
            Reserve(m_count * 13 / 8 + 8);
            new (&m_data[m_count++]) Element(tmp);
        }
        else
        {
            new (&m_data[m_count++]) Element(x);
        }
        return *this;
    }

    inline void Push(T const &x)
    {
        *this << x;
    }

    inline void Pop()
    {
        Remove(m_count - 1, 1);
    }

    void Remove(int pos, int todelete = 1)
    {
        /* FIXME: we need to call dtors for the first
         * todelete elements here */
        for (int i = pos; i + todelete < m_count; i++)
            m_data[i] = m_data[i + todelete];
        for (int i = m_count - todelete; i < m_count; i++)
            m_data[i].~Element();
        m_count -= todelete;
    }

    void Resize(int count, Element e = Element())
    {
        Reserve(count);

        /* Too many elements? Remove them. */
        for (int i = count; i < m_count; ++i)
            m_data[i].~Element();

        /* Not enough elements? Add some. */
        for (int i = m_count; i < count; ++i)
            m_data[i] = e;

        m_count = count;
    }

    inline void Empty()
    {
        Remove(0, m_count);
    }

    void Reserve(int toreserve)
    {
        if (toreserve <= (int)m_reserved)
            return;

        /* This cast is not very nice, because we kill any alignment
         * information we could have. But until C++ gives us the proper
         * tools to deal with it, we assume new uint8_t[] returns properly
         * aligned data. */
        Element *tmp = reinterpret_cast<Element *>(reinterpret_cast<uintptr_t>
                               (new uint8_t[sizeof(Element) * toreserve]));
        for (int i = 0; i < m_count; i++)
        {
            new(&tmp[i]) Element(m_data[i]);
            m_data[i].~Element();
        }
        if (m_data)
            delete[] reinterpret_cast<uint8_t *>(m_data);
        m_data = tmp;
        m_reserved = toreserve;
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
class Array : public ArrayBase<ArrayElement<T1, T2, T3, T4, T5, T6, T7, T8>,
                               Array<T1, T2, T3, T4, T5, T6, T7, T8> >
{
public:
    inline void Push(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4,
                     T5 const &m5, T6 const &m6, T7 const &m7, T8 const &m8)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            T5 tmp5 = m5; T6 tmp6 = m6; T7 tmp7 = m7; T8 tmp8 = m8;
            this->Reserve(this->m_count * 13 / 8 + 8);
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
  : public ArrayBase<ArrayElement<T1, T2, T3, T4, T5, T6, T7, void>,
                     Array<T1, T2, T3, T4, T5, T6, T7> >
{
public:
    inline void Push(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4,
                     T5 const &m5, T6 const &m6, T7 const &m7)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            T5 tmp5 = m5; T6 tmp6 = m6; T7 tmp7 = m7;
            this->Reserve(this->m_count * 13 / 8 + 8);
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
  : public ArrayBase<ArrayElement<T1, T2, T3, T4, T5, T6, void, void>,
                     Array<T1, T2, T3, T4, T5, T6> >
{
public:
    inline void Push(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4,
                     T5 const &m5, T6 const &m6)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            T5 tmp5 = m5; T6 tmp6 = m6;
            this->Reserve(this->m_count * 13 / 8 + 8);
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
  : public ArrayBase<ArrayElement<T1, T2, T3, T4, T5, void, void, void>,
                     Array<T1, T2, T3, T4, T5> >
{
public:
    inline void Push(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4,
                     T5 const &m5)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            T5 tmp5 = m5;
            this->Reserve(this->m_count * 13 / 8 + 8);
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
  : public ArrayBase<ArrayElement<T1, T2, T3, T4, void, void, void, void>,
                     Array<T1, T2, T3, T4> >
{
public:
    inline void Push(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            this->Reserve(this->m_count * 13 / 8 + 8);
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
  : public ArrayBase<ArrayElement<T1, T2, T3, void, void, void, void, void>,
                     Array<T1, T2, T3> >
{
public:
    inline void Push(T1 const &m1, T2 const &m2, T3 const &m3)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3;
            this->Reserve(this->m_count * 13 / 8 + 8);
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
  : public ArrayBase<ArrayElement<T1, T2, void, void, void, void, void, void>,
                     Array<T1, T2> >
{
public:
    inline void Push(T1 const &m1, T2 const &m2)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2;
            this->Reserve(this->m_count * 13 / 8 + 8);
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
class Array<T, void, void, void, void, void, void, void>
  : public ArrayBase<T,
                     Array<T> >
{
};

} /* namespace lol */

#endif // __LOL_CORE_ARRAY_H__

