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
// The array2d class
// -----------------
// A very simple 2D array class allowing var[i][j] indexing, with some nice
// additional features, eg. array2d<int,float> for automatic arrays of tuples.
//

//
// FIXME: This file is in lol/math/ instead of lol/base/ because it
// uses ivec2.
//

#if !defined __LOL_MATH_ARRAY2D_H__
#define __LOL_MATH_ARRAY2D_H__

#include <lol/base/array.h>
#include <lol/base/assert.h>

namespace lol
{

template<typename... T>
class array2d : protected array<T...>
{
public:
    typedef array<T...> super;
    typedef typename super::element_t element_t;

    inline array2d()
      : m_size(0, 0)
    {
    }

    inline array2d(std::initializer_list<
                   std::initializer_list<element_t>> const &list)
      : m_size(list.size() ? (int)(*list.begin()).size() : 0,
               (int)list.size())
    {
        super::Reserve(m_size.x * m_size.y);
        for (auto l : list)
            for (auto elem : l)
                super::Push(elem);
    }

    inline array2d(int w, int h)
    {
        SetSize(ivec2(w, h));
    }

    inline array2d(ivec2 size)
    {
        SetSize(size);
    }

    /* Access elements directly using an ivec2 index */
    inline element_t const &operator [](ivec2 pos) const
    {
        ASSERT(pos.x >= 0);
        ASSERT(pos.y >= 0);
        ASSERT(pos.x < m_size.x);
        ASSERT(pos.y < m_size.y);

        ptrdiff_t n = pos.y * m_size.x + pos.x;
        ASSERT(n >= 0);
        ASSERT(n < this->m_count);

        return this->m_data[n];
    }

    inline element_t &operator [](ivec2 pos)
    {
        ASSERT(pos.x >= 0);
        ASSERT(pos.y >= 0);
        ASSERT(pos.x < m_size.x);
        ASSERT(pos.y < m_size.y);

        ptrdiff_t n = pos.y * m_size.x + pos.x;
        ASSERT(n >= 0);
        ASSERT(n < this->m_count);

        return this->m_data[n];
    }

    class Column
    {
    public:
        inline Column(array2d &array, ptrdiff_t i)
          : m_array(array),
            m_column(i)
        {
        }

        inline element_t &operator [](ptrdiff_t j)
        {
            ASSERT(j >= 0);
            ASSERT(j < m_array.m_size.y);
            return m_array[ivec2(m_column, j)];
        }

    private:
        array2d<T...> &m_array;
        ptrdiff_t m_column;
    };

    class ConstColumn
    {
    public:
        inline ConstColumn(array2d const &array, ptrdiff_t i)
          : m_array(array),
            m_column(i)
        {
        }

        inline element_t const &operator [](ptrdiff_t j) const
        {
            ASSERT(j >= 0);
            ASSERT(j < m_array.m_size.y);
            return m_array[ivec2(m_column, j)];
        }

    private:
        array2d<T...> const &m_array;
        ptrdiff_t m_column;
    };

    /* Access addressable columns, allowing for array[i][j] syntax. */
    inline class Column operator [](ptrdiff_t i)
    {
        ASSERT(i >= 0);
        ASSERT(i < m_size.x);
        return Column(*this, i);
    }

    inline class ConstColumn operator [](ptrdiff_t i) const
    {
        ASSERT(i >= 0);
        ASSERT(i < m_size.x);
        return ConstColumn(*this, i);
    }

    /* Resize the array.
     * FIXME: data gets scrambled; should we care? */
    inline void SetSize(ivec2 size, element_t e = element_t())
    {
        this->Resize(size.x * size.y, e);
        m_size = size;
    }

    inline ivec2 GetSize() const
    {
        return m_size;
    }

public:
    inline element_t *Data() { return super::Data(); }
    inline element_t const *Data() const { return super::Data(); }
    inline ptrdiff_t Count() const { return super::Count(); }
    inline ptrdiff_t Bytes() const { return super::Bytes(); }

private:
    ivec2 m_size;
};

} /* namespace lol */

#endif // __LOL_MATH_ARRAY2D_H__

