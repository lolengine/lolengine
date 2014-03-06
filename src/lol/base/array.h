//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//            (c) 2013-2014 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Array class
// ---------------
// A very simple Array class not unlike the std::vector, with some nice
// additional features, eg. Array<int,float> for automatic arrays of structs.
//

#if !defined __LOL_BASE_ARRAY_H__
#define __LOL_BASE_ARRAY_H__

#include <lol/base/assert.h>

#include <new>
#include <stdint.h>

namespace lol
{

#define INDEX_NONE -1

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
            /* FIXME: there is an opportunity for optimisation here if we
             * find a way to ask Reserve not to create new elements, since
             * we're going to overwrite them anyway. */
            if (m_reserved < that.m_count)
            {
                /* If not enough space, reserve memory, overwrite the first
                 * elements, then use placement new directly for the
                 * remaining elements. */
                Reserve(that.m_count);
                for (int i = 0; i < m_count && i < that.m_count; i++)
                    m_data[i] = Element(that[i]);
                for (int i = m_count; i < that.m_count; i++)
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

    ArrayBase& operator+=(ArrayBase const &that)
    {
        int todo = that.m_count;
        Reserve(m_count + that.m_count);
        for (int i = 0; i < todo; i++)
            new(&m_data[m_count + i]) Element(that[i]);
        m_count += todo;
        return *this;
    }

    ARRAY operator+(ARRAY const &that) const
    {
        ARRAY ret;
        ret.Reserve(m_count + that.m_count);
        ret += *this;
        ret += that;
        return ret;
    }

    inline Element& operator[](int n)
    {
        /* Allow array[0] even if size is zero so that people can
         * always use &array[0] to get a pointer to the data. */
        ASSERT(n >= 0);
        ASSERT((unsigned)n < (unsigned)m_count || (!n && !m_count));
        return m_data[n];
    }

    inline Element const& operator[](int n) const
    {
        ASSERT(n >= 0);
        ASSERT(n < m_count || (!n && !m_count));
        return m_data[n];
    }

    inline Element& Last()
    {
        ASSERT(m_count > 0);
        return m_data[m_count - 1];
    }

    inline Element *Data()
    {
        return m_data;
    }

    inline Element const *Data() const
    {
        return m_data;
    }

    inline Element const& Last() const
    {
        ASSERT(m_count > 0);
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

    inline ArrayBase& operator>>(T const &x)
    {
        RemoveItem(x);
        return *this;
    }

    inline void Push(T const &x)
    {
        *this << x;
    }

    inline bool PushUnique(T const &x)
    {
        int idx = Find(x);
        if (idx == INDEX_NONE)
        {
            Push(x);
            return true;
        }
        return false;
    }

    inline int Find(T const &x)
    {
        for (int i = 0; i < m_count; ++i)
            if (m_data[i] == x)
                return i;
        return INDEX_NONE;
    }

    bool RemoveItem(T const &x)
    {
        int idx = Find(x);
        if (idx != INDEX_NONE)
        {
            Remove(idx);
            return true;
        }
        return false;
    }

    bool RemoveSwapItem(T const &x)
    {
        int idx = Find(x);
        if (idx != INDEX_NONE)
        {
            RemoveSwap(idx);
            return true;
        }
        return false;
    }

    bool SwapItem(T const &x1, T const &x2)
    {
        int idx1 = Find(x1);
        int idx2 = Find(x2);
        if (idx1 != INDEX_NONE && idx2 != INDEX_NONE)
        {
            Swap(idx1, idx2);
            return true;
        }
        return false;
    }

    inline T Pop()
    {
        ASSERT(m_count > 0);
        Element tmp = Last();
        Remove(m_count - 1, 1);
        return tmp;
    }

    void Swap(int pos1, int pos2)
    {
        ASSERT(pos1 >= 0);
        ASSERT(pos2 >= 0);
        ASSERT(pos1 < m_count);
        ASSERT(pos2 < m_count);

        if (pos1 != pos2)
        {
            Element tmp = (*this)[pos1];
            (*this)[pos1] = (*this)[pos2];
            (*this)[pos2] = tmp;
        }
    }

    void Remove(int pos, int todelete = 1)
    {
        ASSERT(todelete >= 0);
        ASSERT(pos - todelete >= -m_count - 1);
        ASSERT(pos + todelete <= m_count);
        if (pos < 0)
            pos = m_count + pos;

        for (int i = pos; i + todelete < m_count; i++)
            m_data[i] = m_data[i + todelete];
        for (int i = m_count - todelete; i < m_count; i++)
            m_data[i].~Element();
        m_count -= todelete;
    }

    void RemoveSwap(int pos, int todelete = 1)
    {
        ASSERT(todelete >= 0);
        ASSERT(pos - todelete >= -m_count - 1);
        ASSERT(pos + todelete <= m_count);
        if (pos < 0)
            pos = m_count + pos;

        for (int i = 0; i < todelete; i++)
        {
            if (pos + i < m_count - 1 - i)
                m_data[pos + i] = m_data[m_count - 1 - i];
            m_data[m_count - 1 - i].~Element();
        }
        m_count -= todelete;
    }

    void Resize(int count, Element e = Element())
    {
        ASSERT(count >= 0);
        Reserve(count);

        /* Too many elements? Remove them. */
        for (int i = count; i < m_count; ++i)
            m_data[i].~Element();

        /* Not enough elements? Add some. */
        for (int i = m_count; i < count; ++i)
            new(&m_data[i]) Element(e);

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
        ASSERT(tmp, "out of memory in Array class");
        for (int i = 0; i < m_count; i++)
        {
            new(&tmp[i]) Element(m_data[i]);
            m_data[i].~Element();
        }
        delete[] reinterpret_cast<uint8_t *>(m_data);
        m_data = tmp;
        m_reserved = toreserve;
    }

    void Sort(int sort);
    void SortQuickSwap(int start, int stop);

public:
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
            new (&this->m_data[this->m_count].m1) T1(tmp1);
            new (&this->m_data[this->m_count].m2) T2(tmp2);
            new (&this->m_data[this->m_count].m3) T3(tmp3);
            new (&this->m_data[this->m_count].m4) T4(tmp4);
            new (&this->m_data[this->m_count].m5) T5(tmp5);
            new (&this->m_data[this->m_count].m6) T6(tmp6);
            new (&this->m_data[this->m_count].m7) T7(tmp7);
            new (&this->m_data[this->m_count].m8) T8(tmp8);
        }
        else
        {
            new (&this->m_data[this->m_count].m1) T1(m1);
            new (&this->m_data[this->m_count].m2) T2(m2);
            new (&this->m_data[this->m_count].m3) T3(m3);
            new (&this->m_data[this->m_count].m4) T4(m4);
            new (&this->m_data[this->m_count].m5) T5(m5);
            new (&this->m_data[this->m_count].m6) T6(m6);
            new (&this->m_data[this->m_count].m7) T7(m7);
            new (&this->m_data[this->m_count].m8) T8(m8);
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
            new (&this->m_data[this->m_count].m1) T1(tmp1);
            new (&this->m_data[this->m_count].m2) T2(tmp2);
            new (&this->m_data[this->m_count].m3) T3(tmp3);
            new (&this->m_data[this->m_count].m4) T4(tmp4);
            new (&this->m_data[this->m_count].m5) T5(tmp5);
            new (&this->m_data[this->m_count].m6) T6(tmp6);
            new (&this->m_data[this->m_count].m7) T7(tmp7);
        }
        else
        {
            new (&this->m_data[this->m_count].m1) T1(m1);
            new (&this->m_data[this->m_count].m2) T2(m2);
            new (&this->m_data[this->m_count].m3) T3(m3);
            new (&this->m_data[this->m_count].m4) T4(m4);
            new (&this->m_data[this->m_count].m5) T5(m5);
            new (&this->m_data[this->m_count].m6) T6(m6);
            new (&this->m_data[this->m_count].m7) T7(m7);
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
            new (&this->m_data[this->m_count].m1) T1(tmp1);
            new (&this->m_data[this->m_count].m2) T2(tmp2);
            new (&this->m_data[this->m_count].m3) T3(tmp3);
            new (&this->m_data[this->m_count].m4) T4(tmp4);
            new (&this->m_data[this->m_count].m5) T5(tmp5);
            new (&this->m_data[this->m_count].m6) T6(tmp6);
        }
        else
        {
            new (&this->m_data[this->m_count].m1) T1(m1);
            new (&this->m_data[this->m_count].m2) T2(m2);
            new (&this->m_data[this->m_count].m3) T3(m3);
            new (&this->m_data[this->m_count].m4) T4(m4);
            new (&this->m_data[this->m_count].m5) T5(m5);
            new (&this->m_data[this->m_count].m6) T6(m6);
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
            new (&this->m_data[this->m_count].m1) T1(tmp1);
            new (&this->m_data[this->m_count].m2) T2(tmp2);
            new (&this->m_data[this->m_count].m3) T3(tmp3);
            new (&this->m_data[this->m_count].m4) T4(tmp4);
            new (&this->m_data[this->m_count].m5) T5(tmp5);
        }
        else
        {
            new (&this->m_data[this->m_count].m1) T1(m1);
            new (&this->m_data[this->m_count].m2) T2(m2);
            new (&this->m_data[this->m_count].m3) T3(m3);
            new (&this->m_data[this->m_count].m4) T4(m4);
            new (&this->m_data[this->m_count].m5) T5(m5);
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
            new (&this->m_data[this->m_count].m1) T1(tmp1);
            new (&this->m_data[this->m_count].m2) T2(tmp2);
            new (&this->m_data[this->m_count].m3) T3(tmp3);
            new (&this->m_data[this->m_count].m4) T4(tmp4);
        }
        else
        {
            new (&this->m_data[this->m_count].m1) T1(m1);
            new (&this->m_data[this->m_count].m2) T2(m2);
            new (&this->m_data[this->m_count].m3) T3(m3);
            new (&this->m_data[this->m_count].m4) T4(m4);
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
            new (&this->m_data[this->m_count].m1) T1(tmp1);
            new (&this->m_data[this->m_count].m2) T2(tmp2);
            new (&this->m_data[this->m_count].m3) T3(tmp3);
        }
        else
        {
            new (&this->m_data[this->m_count].m1) T1(m1);
            new (&this->m_data[this->m_count].m2) T2(m2);
            new (&this->m_data[this->m_count].m3) T3(m3);
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
            new (&this->m_data[this->m_count].m1) T1(tmp1);
            new (&this->m_data[this->m_count].m2) T2(tmp2);
        }
        else
        {
            new (&this->m_data[this->m_count].m1) T1(m1);
            new (&this->m_data[this->m_count].m2) T2(m2);
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

#endif // __LOL_BASE_ARRAY_H__

