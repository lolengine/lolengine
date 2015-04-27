//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2013—2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
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
 * Sorting algorithm list
 */

enum class SortAlgorithm : uint8_t
{
    QuickSwap,
    Bubble,
};

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
        reserve(list.size());
        for (auto elem : list)
            push(elem);
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
        reserve(that.m_count);
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
                reserve(that.m_count);
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
        reserve(m_count + todo);
        for (ptrdiff_t i = 0; i < todo; i++)
            new(&m_data[m_count + i]) element_t(that[i]);
        m_count += todo;
        return *this;
    }

    ARRAY operator+(ARRAY const &that) const
    {
        ARRAY ret;
        ret.reserve(m_count + that.m_count);
        ret += *this;
        ret += that;
        return ret;
    }

    inline element_t& operator[](ptrdiff_t n)
    {
        /* Allow array[0] even if size is zero so that people can
         * always use &array[0] to get a pointer to the data. */
        ASSERT(n >= 0 && (n < m_count || (!n && !m_count)),
               "cannot access index %ld in array of size %ld",
               (long int)n, (long int)m_count);
        return m_data[n];
    }

    inline element_t const& operator[](ptrdiff_t n) const
    {
        ASSERT(n >= 0 && (n < m_count || (!n && !m_count)),
               "cannot access index %ld in array of size %ld",
               (long int)n, (long int)m_count);
        return m_data[n];
    }

    inline element_t& last()
    {
        ASSERT(m_count > 0);
        return m_data[m_count - 1];
    }

    inline element_t *data()
    {
        return m_data;
    }

    inline element_t const *data() const
    {
        return m_data;
    }

    inline element_t const& last() const
    {
        ASSERT(m_count > 0);
        return m_data[m_count - 1];
    }

    inline array_base& operator<<(T const &x)
    {
        if (m_count >= m_reserved)
        {
            T tmp = x;
            grow();
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
        remove_item(x);
        return *this;
    }

    inline void push(T const &x)
    {
        *this << x;
    }

    inline bool push_unique(T const &x)
    {
        if (find(x) != INDEX_NONE)
            return false;

        push(x);
        return true;
    }

    inline void insert(T const &x, ptrdiff_t pos)
    {
        ASSERT(pos >= 0 && pos <= m_count,
               "cannot insert at index %ld in array of size %ld",
               (long int)pos, (long int)m_count);

        if (m_count >= m_reserved)
            grow();

        for (ptrdiff_t i = m_count; i > pos; --i)
        {
            new (&m_data[i]) element_t(m_data[i - 1]);
            m_data[i - 1].~element_t();
        }
        new (&m_data[pos]) element_t(x);
        ++m_count;
    }

    inline bool insert_unique(T const &x, ptrdiff_t pos)
    {
        ASSERT(pos >= 0 && pos <= m_count,
               "cannot insert at index %ld in array of size %ld",
               (long int)pos, (long int)m_count);

        if (find(x) != INDEX_NONE)
            return false;

        insert(x, pos);
        return true;
    }

    inline ptrdiff_t find(T const &x)
    {
        for (ptrdiff_t i = 0; i < m_count; ++i)
            if (m_data[i] == x)
                return i;
        return INDEX_NONE;
    }

    bool remove_item(T const &x)
    {
        ptrdiff_t idx = find(x);
        if (idx != INDEX_NONE)
        {
            remove(idx);
            return true;
        }
        return false;
    }

    bool remove_swap_item(T const &x)
    {
        ptrdiff_t idx = find(x);
        if (idx != INDEX_NONE)
        {
            remove_swap(idx);
            return true;
        }
        return false;
    }

    inline T pop()
    {
        ASSERT(m_count > 0);
        element_t tmp = last();
        remove(m_count - 1, 1);
        return tmp;
    }

    inline void swap(ptrdiff_t i, ptrdiff_t j)
    {
        ASSERT(i >= 0 && i < m_count && j >= 0 && j < m_count,
               "cannot swap elements %ld and %ld in array of size %ld",
               (long int)i, (long int)j, (long int)m_count);

        std::swap(m_data[i], m_data[j]);
    }

    void remove(ptrdiff_t pos, ptrdiff_t todelete = 1)
    {
        ASSERT(todelete >= 0);
        ASSERT(pos - todelete >= -m_count - 1 && pos + todelete <= m_count,
               "cannot remove %ld elements at %ld in array of size %ld",
               (long int)todelete, (long int)pos, (long int)m_count);

        if (pos < 0)
            pos = m_count + pos;

        for (ptrdiff_t i = pos; i + todelete < m_count; i++)
            m_data[i] = m_data[i + todelete];
        for (ptrdiff_t i = m_count - todelete; i < m_count; i++)
            m_data[i].~element_t();
        m_count -= todelete;
    }

    void remove_swap(ptrdiff_t pos, ptrdiff_t todelete = 1)
    {
        ASSERT(todelete >= 0);
        ASSERT(pos - todelete >= -m_count - 1 && pos + todelete <= m_count,
               "cannot remove %ld elements at %ld in array of size %ld",
               (long int)todelete, (long int)pos, (long int)m_count);

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

    void resize(ptrdiff_t item_count, element_t e = element_t())
    {
        ASSERT(item_count >= 0);
        reserve(item_count);

        /* Too many elements? Remove them. */
        for (ptrdiff_t i = item_count; i < m_count; ++i)
            m_data[i].~element_t();

        /* Not enough elements? Add some. */
        for (ptrdiff_t i = m_count; i < item_count; ++i)
            new(&m_data[i]) element_t(e);

        m_count = item_count;
    }

    inline void empty()
    {
        remove(0, m_count);
    }

    void reserve(ptrdiff_t toreserve)
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

    void shuffle();

    void sort(SortAlgorithm algorithm);

    /* Support C++11 range-based for loops */
    class const_iterator
    {
    public:
        const_iterator(array_base const *that, ptrdiff_t pos)
          : m_pos(pos),
            m_array(that)
        { }

        bool operator !=(const const_iterator& that) const
        {
            return m_pos != that.m_pos;
        }

        element_t const & operator *() const
        {
            return (*m_array)[m_pos];
        }

        const_iterator const & operator ++()
        {
            ++m_pos;
            return *this;
        }

    private:
        ptrdiff_t m_pos;
        array_base const *m_array;
    };

    class iterator
    {
    public:
        iterator(array_base *that, ptrdiff_t pos)
          : m_pos(pos),
            m_array(that)
        { }

        bool operator !=(const iterator& that) const
        {
            return m_pos != that.m_pos;
        }

        element_t & operator *()
        {
            return (*m_array)[m_pos];
        }

        iterator const & operator ++()
        {
            ++m_pos;
            return *this;
        }

    private:
        ptrdiff_t m_pos;
        array_base *m_array;
    };

public:
    inline int count() const { return (int)m_count; }
    inline int bytes() const { return (int)(m_count * sizeof(element_t)); }

    inline ptrdiff_t count_s() const { return m_count; }
    inline ptrdiff_t bytes_s() const { return m_count * sizeof(element_t); }

protected:
    void grow()
    {
        reserve(m_count * 13 / 8 + 8);
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
public:
    /* GCC needs this but Clang doesn’t */
    typedef tuple<T...> element_t;

#if LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
private:
    using array_base<tuple<T...>, array<T...>>::array_base;
#else
public:
    inline array()
      : array_base<element_t, array<T...>>::array_base()
    {}

    inline array(std::initializer_list<element_t> const &list)
      : array_base<element_t, array<T...>>::array_base(list)
    {}
#endif

public:
    inline void push(T... args)
    {
        if (this->m_count >= this->m_reserved)
        {
            tuple<T...> tmp = { args... };
            this->grow();
            new (&this->m_data[this->m_count].m1) tuple<T...>(tmp);
        }
        else
        {
            new (&this->m_data[this->m_count]) tuple<T...>({ args... });
        }
        ++this->m_count;
    }

    inline void insert(ptrdiff_t pos, T... args)
    {
        ASSERT(pos >= 0 && pos <= this->m_count,
               "cannot insert at index %ld in array of size %ld",
               (long int)pos, (long int)this->m_count);

        if (this->m_count >= this->m_reserved)
            this->grow();

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
typename array<T...>::iterator begin(array<T...> &a)
{
    return typename array<T...>::iterator(&a, 0);
}

template<typename... T>
typename array<T...>::iterator end(array<T...> &a)
{
    return typename array<T...>::iterator(&a, a.count());
}

template<typename... T>
typename array<T...>::const_iterator begin(array<T...> const &a)
{
    return typename array<T...>::const_iterator(&a, 0);
}

template<typename... T>
typename array<T...>::const_iterator end(array<T...> const &a)
{
    return typename array<T...>::const_iterator(&a, a.count());
}

} /* namespace lol */

