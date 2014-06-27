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

template<typename T1, typename T2 = void, typename T3 = void,
         typename T4 = void, typename T5 = void, typename T6 = void,
         typename T7 = void, typename T8 = void>
class array2d : protected Array<T1, T2, T3, T4, T5, T6, T7, T8>
{
public:
    typedef Array<T1, T2, T3, T4, T5, T6, T7, T8> Super;
    typedef typename Super::Element Element;

    inline array2d()
      : m_size(0, 0)
    {
    }

    inline array2d(std::initializer_list<
                   std::initializer_list<Element>> const &list)
      : m_size(list.size() ? (int)(*list.begin()).size() : 0,
               (int)list.size())
    {
        Super::Reserve(m_size.x * m_size.y);
        for (auto l : list)
            for (auto elem : l)
                Super::Push(elem);
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
    inline Element const &operator [](ivec2 pos) const
    {
        ASSERT(pos.x >= 0);
        ASSERT(pos.y >= 0);
        ASSERT(pos.x < m_size.x);
        ASSERT(pos.y < m_size.y);

        int n = pos.y * m_size.x + pos.x;
        ASSERT(n >= 0);
        ASSERT(n < this->m_count);

        return this->m_data[n];
    }

    inline Element &operator [](ivec2 pos)
    {
        ASSERT(pos.x >= 0);
        ASSERT(pos.y >= 0);
        ASSERT(pos.x < m_size.x);
        ASSERT(pos.y < m_size.y);

        int n = pos.y * m_size.x + pos.x;
        ASSERT(n >= 0);
        ASSERT(n < this->m_count);

        return this->m_data[n];
    }

    class Column
    {
    public:
        inline Column(array2d &array, int i)
          : m_array(array),
            m_column(i)
        {
        }

        inline Element &operator [](int j)
        {
            ASSERT(j >= 0);
            ASSERT(j < m_array.m_size.y);
            return m_array[ivec2(m_column, j)];
        }

    private:
        array2d<T1, T2, T3, T4, T5, T6, T7, T8> &m_array;
        int m_column;
    };

    class ConstColumn
    {
    public:
        inline ConstColumn(array2d const &array, int i)
          : m_array(array),
            m_column(i)
        {
        }

        inline Element const &operator [](int j) const
        {
            ASSERT(j >= 0);
            ASSERT(j < m_array.m_size.y);
            return m_array[ivec2(m_column, j)];
        }

    private:
        array2d<T1, T2, T3, T4, T5, T6, T7, T8> const &m_array;
        int m_column;
    };

    /* Access addressable columns, allowing for array[i][j] syntax. */
    inline class Column operator [](int i)
    {
        ASSERT(i >= 0);
        ASSERT(i < m_size.x);
        return Column(*this, i);
    }

    inline class ConstColumn operator [](int i) const
    {
        ASSERT(i >= 0);
        ASSERT(i < m_size.x);
        return ConstColumn(*this, i);
    }

    /* Resize the array.
     * FIXME: data gets scrambled; should we care? */
    inline void SetSize(ivec2 size, Element e = Element())
    {
        this->Resize(size.x * size.y, e);
        m_size = size;
    }

    inline ivec2 GetSize() const
    {
        return m_size;
    }

public:
    inline Element *Data() { return Super::Data(); }
    inline Element const *Data() const { return Super::Data(); }
    inline int Count() const { return Super::Count(); }
    inline int Bytes() const { return Super::Bytes(); }

private:
    ivec2 m_size;
};

} /* namespace lol */

#endif // __LOL_MATH_ARRAY2D_H__

