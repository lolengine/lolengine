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
// The array class
// ---------------
// A very simple array class not unlike the std::vector, with some nice
// additional features, eg. array<int,float> for automatic arrays of tuples.
//

#if !defined __LOL_BASE_ARRAY_H__
#define __LOL_BASE_ARRAY_H__

#include <lol/base/assert.h>

#include <new> /* for placement new */
#include <algorithm> /* for std::swap */
#include <stdint.h>
#include <initializer_list>

namespace lol
{

#define INDEX_NONE -1

/*
 * The base array type.
 *
 * Contains an m_data memory array of Elements, of which only the first
 * m_count are allocated. The rest is uninitialised memory.
 */

template<typename T, typename ARRAY> class array_base
{
public:
    typedef T element_t;

    inline array_base() : m_data(0), m_count(0), m_reserved(0)
    {
    }

    inline array_base(std::initializer_list<element_t> const &list)
      : m_data(0),
        m_count(0),
        m_reserved(0)
    {
        Reserve((int)list.size());
        for (auto elem : list)
            Push(elem);
    }

    inline ~array_base()
    {
        for (int i = 0; i < m_count; i++)
            m_data[i].~element_t();
        delete[] reinterpret_cast<uint8_t *>(m_data);
    }

    array_base(array_base const& that) : m_data(0), m_count(0), m_reserved(0)
    {
        /* Reserve the exact number of values instead of what the other
         * array had reserved. Just a method for not wasting too much. */
        Reserve(that.m_count);
        for (int i = 0; i < that.m_count; i++)
            new(&m_data[i]) element_t(that[i]);
        m_count = that.m_count;
    }

    array_base& operator=(array_base const& that)
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
                    m_data[i] = element_t(that[i]);
                for (int i = m_count; i < that.m_count; i++)
                    new(&m_data[i]) element_t(that[i]);
            }
            else
            {
                /* If enough space, overwrite the common elements, then
                 * use placement new for the elements in the other array
                 * that we do not have, and finally destroy the remaining
                 * elements. */
                for (int i = 0; i < m_count && i < that.m_count; i++)
                    m_data[i] = element_t(that[i]);
                for (int i = m_count; i < that.m_count; i++)
                    new(&m_data[i]) element_t(that[i]);
                for (int i = that.m_count; i < m_count; i++)
                    m_data[i].~element_t();
            }
            m_count = that.m_count;
        }
        return *this;
    }

    array_base& operator+=(array_base const &that)
    {
        int todo = that.m_count;
        Reserve(m_count + that.m_count);
        for (int i = 0; i < todo; i++)
            new(&m_data[m_count + i]) element_t(that[i]);
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

    inline element_t& operator[](int n)
    {
        /* Allow array[0] even if size is zero so that people can
         * always use &array[0] to get a pointer to the data. */
        ASSERT(n >= 0);
        ASSERT((unsigned)n < (unsigned)m_count || (!n && !m_count));
        return m_data[n];
    }

    inline element_t const& operator[](int n) const
    {
        ASSERT(n >= 0);
        ASSERT(n < m_count || (!n && !m_count));
        return m_data[n];
    }

    inline element_t& operator[](uint32_t n)
    {
        /* Allow array[0] even if size is zero so that people can
         * always use &array[0] to get a pointer to the data. */
        ASSERT(n >= 0);
        ASSERT((uint32_t)n < (uint32_t)m_count || (!n && !m_count));
        return m_data[n];
    }

    inline element_t const& operator[](uint32_t n) const
    {
        ASSERT(n >= 0);
        ASSERT(n < m_count || (!n && !m_count));
        return m_data[n];
    }

    inline element_t& operator[](uint64_t n)
    {
        /* Allow array[0] even if size is zero so that people can
         * always use &array[0] to get a pointer to the data. */
        ASSERT(n >= 0);
        ASSERT((uint64_t)n < (uint64_t)m_count || (!n && !m_count));
        return m_data[n];
    }

    inline element_t const& operator[](uint64_t n) const
    {
        ASSERT(n >= 0);
        ASSERT(n < m_count || (!n && !m_count));
        return m_data[n];
    }

    inline element_t& Last()
    {
        ASSERT(m_count > 0);
        return m_data[m_count - 1];
    }

    inline element_t *Data()
    {
        return m_data;
    }

    inline element_t const *Data() const
    {
        return m_data;
    }

    inline element_t const& Last() const
    {
        ASSERT(m_count > 0);
        return m_data[m_count - 1];
    }

    inline array_base& operator<<(T const &x)
    {
        if (m_count >= m_reserved)
        {
            T tmp = x;
            Grow();
            new (&m_data[m_count]) element_t(tmp);
        }
        else
        {
            new (&m_data[m_count]) element_t(x);
        }
        ++m_count;
        return *this;
    }

    inline array_base& operator>>(T const &x)
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
        if (Find(x) != INDEX_NONE)
            return false;

        Push(x);
        return true;
    }

    inline void Insert(T const &x, int pos)
    {
        ASSERT(pos >= 0);
        ASSERT(pos <= m_count);

        if (m_count >= m_reserved)
            Grow();

        for (int i = m_count; i > pos; --i)
        {
            new (&m_data[i]) element_t(m_data[i - 1]);
            m_data[i - 1].~element_t();
        }
        new (&m_data[pos]) element_t(x);
        ++m_count;
    }

    inline bool InsertUnique(T const &x, int pos)
    {
        ASSERT(pos >= 0);
        ASSERT(pos <= m_count);

        if (Find(x) != INDEX_NONE)
            return false;

        Insert(x, pos);
        return true;
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
        element_t tmp = Last();
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
            std::swap((*this)[pos1], (*this)[pos2]);
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
            m_data[i].~element_t();
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
            m_data[m_count - 1 - i].~element_t();
        }
        m_count -= todelete;
    }

    void Resize(int count, element_t e = element_t())
    {
        ASSERT(count >= 0);
        Reserve(count);

        /* Too many elements? Remove them. */
        for (int i = count; i < m_count; ++i)
            m_data[i].~element_t();

        /* Not enough elements? Add some. */
        for (int i = m_count; i < count; ++i)
            new(&m_data[i]) element_t(e);

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
        element_t *tmp = reinterpret_cast<element_t *>(reinterpret_cast<uintptr_t>
                               (new uint8_t[sizeof(element_t) * toreserve]));
        ASSERT(tmp, "out of memory in array class");
        for (int i = 0; i < m_count; i++)
        {
            new(&tmp[i]) element_t(m_data[i]);
            m_data[i].~element_t();
        }
        delete[] reinterpret_cast<uint8_t *>(m_data);
        m_data = tmp;
        m_reserved = toreserve;
    }

    void Sort(int sort);
    void SortQuickSwap(int start, int stop);

    /* Support C++11 range-based for loops */
    class ConstIterator
    {
    public:
        ConstIterator(array_base const *array, int pos)
          : m_pos(pos),
            m_array(array)
        { }

        bool operator !=(const ConstIterator& that) const
        {
            return m_pos != that.m_pos;
        }

        element_t const & operator *() const
        {
            return (*m_array)[m_pos];
        }

        ConstIterator const & operator ++()
        {
            ++m_pos;
            return *this;
        }

    private:
        int m_pos;
        array_base const *m_array;
    };

    class Iterator
    {
    public:
        Iterator(array_base *array, int pos)
          : m_pos(pos),
            m_array(array)
        { }

        bool operator !=(const Iterator& that) const
        {
            return m_pos != that.m_pos;
        }

        element_t operator *()
        {
            return (*m_array)[m_pos];
        }

        Iterator const & operator ++()
        {
            ++m_pos;
            return *this;
        }

    private:
        int m_pos;
        array_base *m_array;
    };

    ConstIterator begin() const { return ConstIterator(this, 0); }
    ConstIterator end() const { return ConstIterator(this, m_count); }
    Iterator begin() { return Iterator(this, 0); }
    Iterator end() { return Iterator(this, m_count); }

public:
    inline int Count() const { return m_count; }
    inline int Bytes() const { return m_count * sizeof(element_t); }

protected:
    void Grow()
    {
        Reserve(m_count * 13 / 8 + 8);
    }

    element_t *m_data;
    int m_count, m_reserved;
};

/*
 * element_t types
 */

template<typename T1, typename T2, typename T3 = void, typename T4 = void,
         typename T5 = void, typename T6 = void, typename T7 = void,
         typename T8 = void>
class array_element
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5; T6 m6; T7 m7; T8 m8;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6, typename T7>
class array_element<T1, T2, T3, T4, T5, T6, T7, void>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5; T6 m6; T7 m7;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6>
class array_element<T1, T2, T3, T4, T5, T6, void, void>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5; T6 m6;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
class array_element<T1, T2, T3, T4, T5, void, void, void>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5;
};

template<typename T1, typename T2, typename T3, typename T4>
class array_element<T1, T2, T3, T4, void, void, void, void>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4;
};

template<typename T1, typename T2, typename T3>
class array_element<T1, T2, T3, void, void, void, void, void>
{
public:
    T1 m1; T2 m2; T3 m3;
};

template<typename T1, typename T2>
class array_element<T1, T2, void, void, void, void, void, void>
{
public:
    T1 m1; T2 m2;
};

/*
 * array specialisations implementing specific setters
 */

template<typename T1, typename T2 = void, typename T3 = void,
         typename T4 = void, typename T5 = void, typename T6 = void,
         typename T7 = void, typename T8 = void>
class array : public array_base<array_element<T1, T2, T3, T4, T5, T6, T7, T8>,
                               array<T1, T2, T3, T4, T5, T6, T7, T8>>
{
#if LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
    using array_base<array_element<T1, T2, T3, T4, T5, T6, T7, T8>,
                    array<T1, T2, T3, T4, T5, T6, T7, T8>>::array_base;
#else
public:
    typedef array_element<T1, T2, T3, T4, T5, T6, T7, T8> element_t;

    inline array()
      : array_base<element_t,
                  array<T1, T2, T3, T4, T5, T6, T7, T8>>::array_base() {}

    inline array(std::initializer_list<element_t> const &list)
      : array_base<element_t,
                  array<T1, T2, T3, T4, T5, T6, T7, T8>>::array_base(list) {}
#endif

public:
    inline void Push(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4,
                     T5 const &m5, T6 const &m6, T7 const &m7, T8 const &m8)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            T5 tmp5 = m5; T6 tmp6 = m6; T7 tmp7 = m7; T8 tmp8 = m8;
            this->Grow();
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
class array<T1, T2, T3, T4, T5, T6, T7, void>
  : public array_base<array_element<T1, T2, T3, T4, T5, T6, T7, void>,
                     array<T1, T2, T3, T4, T5, T6, T7>>
{
#if LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
    using array_base<array_element<T1, T2, T3, T4, T5, T6, T7, void>,
                    array<T1, T2, T3, T4, T5, T6, T7>>::array_base;
#else
public:
    typedef array_element<T1, T2, T3, T4, T5, T6, T7, void> element_t;

    inline array()
      : array_base<element_t,
                  array<T1, T2, T3, T4, T5, T6, T7>>::array_base() {}

    inline array(std::initializer_list<element_t> const &list)
      : array_base<element_t,
                  array<T1, T2, T3, T4, T5, T6, T7>>::array_base(list) {}
#endif

public:
    inline void Push(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4,
                     T5 const &m5, T6 const &m6, T7 const &m7)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            T5 tmp5 = m5; T6 tmp6 = m6; T7 tmp7 = m7;
            this->Grow();
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
class array<T1, T2, T3, T4, T5, T6, void, void>
  : public array_base<array_element<T1, T2, T3, T4, T5, T6, void, void>,
                     array<T1, T2, T3, T4, T5, T6>>
{
#if LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
    using array_base<array_element<T1, T2, T3, T4, T5, T6, void, void>,
                    array<T1, T2, T3, T4, T5, T6>>::array_base;
#else
public:
    typedef array_element<T1, T2, T3, T4, T5, T6, void, void> element_t;

    inline array()
      : array_base<element_t,
                  array<T1, T2, T3, T4, T5, T6>>::array_base() {}

    inline array(std::initializer_list<element_t> const &list)
      : array_base<element_t,
                  array<T1, T2, T3, T4, T5, T6>>::array_base(list) {}
#endif

public:
    inline void Push(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4,
                     T5 const &m5, T6 const &m6)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            T5 tmp5 = m5; T6 tmp6 = m6;
            this->Grow();
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
class array<T1, T2, T3, T4, T5, void, void, void>
  : public array_base<array_element<T1, T2, T3, T4, T5, void, void, void>,
                     array<T1, T2, T3, T4, T5>>
{
#if LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
    using array_base<array_element<T1, T2, T3, T4, T5, void, void, void>,
                    array<T1, T2, T3, T4, T5>>::array_base;
#else
public:
    typedef array_element<T1, T2, T3, T4, T5, void, void, void> element_t;

    inline array()
      : array_base<element_t,
                  array<T1, T2, T3, T4, T5>>::array_base() {}

    inline array(std::initializer_list<element_t> const &list)
      : array_base<element_t,
                  array<T1, T2, T3, T4, T5>>::array_base(list) {}
#endif

public:
    inline void Push(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4,
                     T5 const &m5)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            T5 tmp5 = m5;
            this->Grow();
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
class array<T1, T2, T3, T4, void, void, void, void>
  : public array_base<array_element<T1, T2, T3, T4, void, void, void, void>,
                     array<T1, T2, T3, T4>>
{
#if LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
    using array_base<array_element<T1, T2, T3, T4, void, void, void, void>,
                    array<T1, T2, T3, T4>>::array_base;
#else
public:
    typedef array_element<T1, T2, T3, T4, void, void, void, void> element_t;

    inline array()
      : array_base<element_t,
                  array<T1, T2, T3, T4>>::array_base() {}

    inline array(std::initializer_list<element_t> const &list)
      : array_base<element_t,
                  array<T1, T2, T3, T4>>::array_base(list) {}
#endif

public:
    inline void Push(T1 const &m1, T2 const &m2, T3 const &m3, T4 const &m4)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3; T4 tmp4 = m4;
            this->Grow();
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
class array<T1, T2, T3, void, void, void, void, void>
  : public array_base<array_element<T1, T2, T3, void, void, void, void, void>,
                     array<T1, T2, T3>>
{
#if LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
    using array_base<array_element<T1, T2, T3, void, void, void, void, void>,
                    array<T1, T2, T3>>::array_base;
#else
public:
    typedef array_element<T1, T2, T3, void, void, void, void, void> element_t;

    inline array()
      : array_base<element_t,
                  array<T1, T2, T3>>::array_base() {}

    inline array(std::initializer_list<element_t> const &list)
      : array_base<element_t,
                  array<T1, T2, T3>>::array_base(list) {}
#endif

public:
    inline void Push(T1 const &m1, T2 const &m2, T3 const &m3)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2; T3 tmp3 = m3;
            this->Grow();
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
class array<T1, T2, void, void, void, void, void, void>
  : public array_base<array_element<T1, T2, void, void, void, void, void, void>,
                     array<T1, T2>>
{
#if LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
    using array_base<array_element<T1, T2, void, void, void, void, void, void>,
                    array<T1, T2>>::array_base;
#else
public:
    typedef array_element<T1, T2, void, void, void, void, void, void> element_t;

    inline array()
      : array_base<element_t,
                  array<T1, T2>>::array_base() {}

    inline array(std::initializer_list<element_t> const &list)
      : array_base<element_t,
                  array<T1, T2>>::array_base(list) {}
#endif

public:
    inline void Push(T1 const &m1, T2 const &m2)
    {
        if (this->m_count >= this->m_reserved)
        {
            T1 tmp1 = m1; T2 tmp2 = m2;
            this->Grow();
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
class array<T, void, void, void, void, void, void, void>
  : public array_base<T,
                     array<T>>
{
#if LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
    using array_base<T,
                    array<T>>::array_base;
#else
public:
    typedef T element_t;

    inline array()
      : array_base<T,
                  array<T>>::array_base() {}

    inline array(std::initializer_list<element_t> const &list)
      : array_base<T,
                  array<T>>::array_base(list) {}
#endif
};

/* Transitional alias for the camelcase version of lol::array */
template<typename T1, typename T2 = void, typename T3 = void,
         typename T4 = void, typename T5 = void, typename T6 = void,
         typename T7 = void, typename T8 = void>
using Array = array<T1, T2, T3, T4, T5, T6, T7, T8>;

} /* namespace lol */

#endif // __LOL_BASE_ARRAY_H__

