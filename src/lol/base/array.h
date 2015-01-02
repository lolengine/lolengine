//
// Lol Engine
//
// Copyright: (c) 2010-2015 Sam Hocevar <sam@hocevar.net>
//            (c) 2013-2014 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The array class
// ---------------
// A very simple array class not unlike the std::vector, with some nice
// additional features, eg. array<int,float> for automatic arrays of tuples.
//

#include <lol/base/assert.h>
#include <lol/base/tuple.h>

#include <new> /* for placement new */
#include <algorithm> /* for std::swap */
#include <stdint.h>
#include <initializer_list>

namespace lol
{

static ptrdiff_t const INDEX_NONE = -1;

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
        Reserve(list.size());
        for (auto elem : list)
            Push(elem);
    }

    inline ~array_base()
    {
        for (ptrdiff_t i = 0; i < m_count; i++)
            m_data[i].~element_t();
        delete[] reinterpret_cast<uint8_t *>(m_data);
    }

    array_base(array_base const& that) : m_data(0), m_count(0), m_reserved(0)
    {
        /* Reserve the exact number of values instead of what the other
         * array had reserved. Just a method for not wasting too much. */
        Reserve(that.m_count);
        for (ptrdiff_t i = 0; i < that.m_count; i++)
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
                for (ptrdiff_t i = 0; i < m_count && i < that.m_count; i++)
                    m_data[i] = element_t(that[i]);
                for (ptrdiff_t i = m_count; i < that.m_count; i++)
                    new(&m_data[i]) element_t(that[i]);
            }
            else
            {
                /* If enough space, overwrite the common elements, then
                 * use placement new for the elements in the other array
                 * that we do not have, and finally destroy the remaining
                 * elements. */
                for (ptrdiff_t i = 0; i < m_count && i < that.m_count; i++)
                    m_data[i] = element_t(that[i]);
                for (ptrdiff_t i = m_count; i < that.m_count; i++)
                    new(&m_data[i]) element_t(that[i]);
                for (ptrdiff_t i = that.m_count; i < m_count; i++)
                    m_data[i].~element_t();
            }
            m_count = that.m_count;
        }
        return *this;
    }

    array_base& operator+=(array_base const &that)
    {
        ptrdiff_t todo = that.m_count;
        Reserve(m_count + todo);
        for (ptrdiff_t i = 0; i < todo; i++)
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

    inline element_t& operator[](ptrdiff_t n)
    {
        /* Allow array[0] even if size is zero so that people can
         * always use &array[0] to get a pointer to the data. */
        ASSERT(n >= 0 && (n < m_count || (!n && !m_count)),
               "cannot access index %lld in array of size %lld",
               (long long int)n, (long long int)m_count);
        return m_data[n];
    }

    inline element_t const& operator[](ptrdiff_t n) const
    {
        ASSERT(n >= 0 && (n < m_count || (!n && !m_count)),
               "cannot access index %lld in array of size %lld",
               (long long int)n, (long long int)m_count);
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

    inline void Insert(T const &x, ptrdiff_t pos)
    {
        ASSERT(pos >= 0 && pos <= m_count,
               "cannot insert at index %lld in array of size %lld",
               (long long int)pos, (long long int)m_count);

        if (m_count >= m_reserved)
            Grow();

        for (ptrdiff_t i = m_count; i > pos; --i)
        {
            new (&m_data[i]) element_t(m_data[i - 1]);
            m_data[i - 1].~element_t();
        }
        new (&m_data[pos]) element_t(x);
        ++m_count;
    }

    inline bool InsertUnique(T const &x, ptrdiff_t pos)
    {
        ASSERT(pos >= 0 && pos <= m_count,
               "cannot insert at index %lld in array of size %lld",
               (long long int)pos, (long long int)m_count);

        if (Find(x) != INDEX_NONE)
            return false;

        Insert(x, pos);
        return true;
    }

    inline ptrdiff_t Find(T const &x)
    {
        for (ptrdiff_t i = 0; i < m_count; ++i)
            if (m_data[i] == x)
                return i;
        return INDEX_NONE;
    }

    bool RemoveItem(T const &x)
    {
        ptrdiff_t idx = Find(x);
        if (idx != INDEX_NONE)
        {
            Remove(idx);
            return true;
        }
        return false;
    }

    bool RemoveSwapItem(T const &x)
    {
        ptrdiff_t idx = Find(x);
        if (idx != INDEX_NONE)
        {
            RemoveSwap(idx);
            return true;
        }
        return false;
    }

    bool SwapItem(T const &x1, T const &x2)
    {
        ptrdiff_t idx1 = Find(x1);
        ptrdiff_t idx2 = Find(x2);
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

    void Swap(ptrdiff_t pos1, ptrdiff_t pos2)
    {
        ASSERT(pos1 >= 0 && pos1 <= m_count,
               "cannot swap index %lld in array of size %lld",
               (long long int)pos1, (long long int)m_count);
        ASSERT(pos2 >= 0 && pos2 <= m_count,
               "cannot swap index %lld in array of size %lld",
               (long long int)pos2, (long long int)m_count);

        if (pos1 != pos2)
            std::swap((*this)[pos1], (*this)[pos2]);
    }

    void Remove(ptrdiff_t pos, ptrdiff_t todelete = 1)
    {
        ASSERT(todelete >= 0);
        ASSERT(pos - todelete >= -m_count - 1 && pos + todelete <= m_count,
               "cannot remove %lld elements at %lld in array of size %lld",
               (long long int)todelete, (long long int)pos,
               (long long int)m_count);

        if (pos < 0)
            pos = m_count + pos;

        for (ptrdiff_t i = pos; i + todelete < m_count; i++)
            m_data[i] = m_data[i + todelete];
        for (ptrdiff_t i = m_count - todelete; i < m_count; i++)
            m_data[i].~element_t();
        m_count -= todelete;
    }

    void RemoveSwap(ptrdiff_t pos, ptrdiff_t todelete = 1)
    {
        ASSERT(todelete >= 0);
        ASSERT(pos - todelete >= -m_count - 1 && pos + todelete <= m_count,
               "cannot remove %lld elements at %lld in array of size %lld",
               (long long int)todelete, (long long int)pos,
               (long long int)m_count);

        if (pos < 0)
            pos = m_count + pos;

        for (ptrdiff_t i = 0; i < todelete; i++)
        {
            if (pos + i < m_count - 1 - i)
                m_data[pos + i] = m_data[m_count - 1 - i];
            m_data[m_count - 1 - i].~element_t();
        }
        m_count -= todelete;
    }

    void Resize(ptrdiff_t count, element_t e = element_t())
    {
        ASSERT(count >= 0);
        Reserve(count);

        /* Too many elements? Remove them. */
        for (ptrdiff_t i = count; i < m_count; ++i)
            m_data[i].~element_t();

        /* Not enough elements? Add some. */
        for (ptrdiff_t i = m_count; i < count; ++i)
            new(&m_data[i]) element_t(e);

        m_count = count;
    }

    inline void Empty()
    {
        Remove(0, m_count);
    }

    void Reserve(ptrdiff_t toreserve)
    {
        if (toreserve <= m_reserved)
            return;

        /* This cast is not very nice, because we kill any alignment
         * information we could have. But until C++ gives us the proper
         * tools to deal with it, we assume new uint8_t[] returns properly
         * aligned data. */
        element_t *tmp = reinterpret_cast<element_t *>(reinterpret_cast<uintptr_t>
                               (new uint8_t[sizeof(element_t) * toreserve]));
        ASSERT(tmp, "out of memory in array class");
        for (ptrdiff_t i = 0; i < m_count; i++)
        {
            new(&tmp[i]) element_t(m_data[i]);
            m_data[i].~element_t();
        }
        delete[] reinterpret_cast<uint8_t *>(m_data);
        m_data = tmp;
        m_reserved = toreserve;
    }

    void Shuffle();
    void Sort(int sort);
    void SortQuickSwap(ptrdiff_t start, ptrdiff_t stop);

    /* Support C++11 range-based for loops */
    class ConstIterator
    {
    public:
        ConstIterator(array_base const *that, ptrdiff_t pos)
          : m_pos(pos),
            m_array(that)
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
        ptrdiff_t m_pos;
        array_base const *m_array;
    };

    class Iterator
    {
    public:
        Iterator(array_base *that, ptrdiff_t pos)
          : m_pos(pos),
            m_array(that)
        { }

        bool operator !=(const Iterator& that) const
        {
            return m_pos != that.m_pos;
        }

        element_t & operator *()
        {
            return (*m_array)[m_pos];
        }

        Iterator const & operator ++()
        {
            ++m_pos;
            return *this;
        }

    private:
        ptrdiff_t m_pos;
        array_base *m_array;
    };

public:
    inline ptrdiff_t Count() const { return m_count; }
    inline ptrdiff_t Bytes() const { return m_count * sizeof(element_t); }

protected:
    void Grow()
    {
        Reserve(m_count * 13 / 8 + 8);
    }

    element_t *m_data;
    ptrdiff_t m_count, m_reserved;
};

/*
 * array specialisations implementing specific setters
 */

template<typename... T>
class array : public array_base<tuple<T...>, array<T...>>
{
#if LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
    using array_base<tuple<T...>, array<T...>>::array_base;
#else
public:
    typedef tuple<T...> element_t;

    inline array()
      : array_base<element_t, array<T...>>::array_base()
    {}

    inline array(std::initializer_list<element_t> const &list)
      : array_base<element_t, array<T...>>::array_base(list)
    {}
#endif

public:
    inline void Push(T... args)
    {
        if (this->m_count >= this->m_reserved)
        {
            tuple<T...> tmp = { args... };
            this->Grow();
            new (&this->m_data[this->m_count].m1) tuple<T...>(tmp);
        }
        else
        {
            new (&this->m_data[this->m_count]) tuple<T...>({ args... });
        }
        ++this->m_count;
    }

    inline void Insert(ptrdiff_t pos, T... args)
    {
        ASSERT(pos >= 0 && pos <= this->m_count,
               "cannot insert at index %lld in array of size %lld",
               (long long int)pos, (long long int)this->m_count);

        if (this->m_count >= this->m_reserved)
            this->Grow();

        for (ptrdiff_t i = this->m_count; i > pos; --i)
        {
            new (&this->m_data[i]) element_t(this->m_data[i - 1]);
            this->m_data[i - 1].~element_t();
        }
        new (&this->m_data[pos]) tuple<T...>({ args... });
        ++this->m_count;
    }
};

template<typename T>
class array<T>
  : public array_base<T, array<T>>
{
#if LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
    using array_base<T, array<T>>::array_base;
#else
public:
    typedef T element_t;

    inline array()
      : array_base<T, array<T>>::array_base()
    {}

    inline array(std::initializer_list<element_t> const &list)
      : array_base<T, array<T>>::array_base(list)
    {}
#endif
};

/*
 * C++11 iterators
 */

template<typename... T>
typename array<T...>::Iterator begin(array<T...> &a)
{
    return typename array<T...>::Iterator(&a, 0);
}

template<typename... T>
typename array<T...>::Iterator end(array<T...> &a)
{
    return typename array<T...>::Iterator(&a, a.Count());
}

template<typename... T>
typename array<T...>::ConstIterator begin(array<T...> const &a)
{
    return typename array<T...>::ConstIterator(&a, 0);
}

template<typename... T>
typename array<T...>::ConstIterator end(array<T...> const &a)
{
    return typename array<T...>::ConstIterator(&a, a.Count());
}

/*
 * Transitional alias for the deprecated camelcase version of lol::array
 */

#if LOL_FEATURE_CXX11_TEMPLATE_ALIASES
template<typename... T> using Array = array<T...>;
#else
#   define Array array
#endif

} /* namespace lol */

