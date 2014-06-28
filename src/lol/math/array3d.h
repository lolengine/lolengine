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
// The array3d class
// -----------------
// A very simple 3D array class allowing var[i][j][k] indexing, with some nice
// additional features, eg. array3d<int,float> for automatic arrays of tuples.
//

//
// FIXME: This file is in lol/math/ instead of lol/base/ because it
// uses ivec3.
//

#if !defined __LOL_MATH_ARRAY3D_H__
#define __LOL_MATH_ARRAY3D_H__

#include <lol/base/array.h>
#include <lol/base/assert.h>

namespace lol
{

template<typename T1, typename T2 = void, typename T3 = void,
         typename T4 = void, typename T5 = void, typename T6 = void,
         typename T7 = void, typename T8 = void>
class array3d : protected array<T1, T2, T3, T4, T5, T6, T7, T8>
{
public:
    typedef array<T1, T2, T3, T4, T5, T6, T7, T8> super;
    typedef typename super::element_t element_t;

    inline array3d()
      : m_size(0, 0, 0)
    {
    }

    inline array3d(std::initializer_list<
                   std::initializer_list<
                   std::initializer_list<element_t>>> const &list)
      : m_size(list.size() && (*list.begin()).size() ?
                   (int)(*(*list.begin()).begin()).size() : 0,
               list.size() ? (int)(*list.begin()).size() : 0,
               (int)list.size())
    {
        super::Reserve(m_size.x * m_size.y * m_size.z);
        for (auto ll : list)
            for (auto l : ll)
                for (auto elem : l)
                    super::Push(elem);
    }

    inline array3d(int w, int h, int d)
    {
        SetSize(ivec3(w, h, d));
    }

    inline array3d(ivec3 size)
    {
        SetSize(size);
    }

    /* Access elements directly using an ivec3 index */
    inline element_t const &operator [](ivec3 pos) const
    {
        ASSERT(pos.x >= 0);
        ASSERT(pos.y >= 0);
        ASSERT(pos.z >= 0);
        ASSERT(pos.x < m_size.x);
        ASSERT(pos.y < m_size.y);
        ASSERT(pos.z < m_size.z);

        int n = (pos.z * m_size.y + pos.y) * m_size.x + pos.x;
        ASSERT(n >= 0);
        ASSERT(n < this->m_count);

        return this->m_data[n];
    }

    inline element_t &operator [](ivec3 pos)
    {
        ASSERT(pos.x >= 0);
        ASSERT(pos.y >= 0);
        ASSERT(pos.z >= 0);
        ASSERT(pos.x < m_size.x);
        ASSERT(pos.y < m_size.y);
        ASSERT(pos.z < m_size.z);

        int n = (pos.z * m_size.y + pos.y) * m_size.x + pos.x;
        ASSERT(n >= 0);
        ASSERT(n < this->m_count);

        return this->m_data[n];
    }

    class Slice
    {
    public:
        inline Slice(array3d &array, int i)
          : m_array(array),
            m_slice(i)
        {
        }

        class Line
        {
        public:
            inline Line(array3d &array, ivec2 ij)
              : m_array(array),
                m_line(ij)
            {
            }

            inline element_t &operator [](int k)
            {
                ASSERT(k >= 0);
                ASSERT(k < m_array.m_size.z);
                return m_array[ivec3(m_line, k)];
            }

        private:
            array3d<T1, T2, T3, T4, T5, T6, T7, T8> &m_array;
            ivec2 m_line;
        };

        inline class Line operator [](int j)
        {
            ASSERT(j >= 0);
            ASSERT(j < m_array.m_size.y);
            return Line(m_array, ivec2(m_slice, j));
        }

    private:
        array3d<T1, T2, T3, T4, T5, T6, T7, T8> &m_array;
        int m_slice;
    };

    class ConstSlice
    {
    public:
        inline ConstSlice(array3d const &array, int i)
          : m_array(array),
            m_slice(i)
        {
        }

        class Line
        {
        public:
            inline Line(array3d const &array, ivec2 ij)
              : m_array(array),
                m_line(ij)
            {
            }

            inline element_t const &operator [](int k) const
            {
                ASSERT(k >= 0);
                ASSERT(k < m_array.m_size.z);
                return m_array[ivec3(m_line, k)];
            }

        private:
            array3d<T1, T2, T3, T4, T5, T6, T7, T8> const &m_array;
            ivec2 m_line;
        };

        inline class Line operator [](int j) const
        {
            ASSERT(j >= 0);
            ASSERT(j < m_array.m_size.y);
            return Line(m_array, ivec2(m_slice, j));
        }

    private:
        array3d<T1, T2, T3, T4, T5, T6, T7, T8> const &m_array;
        int m_slice;
    };

    /* Access addressable slices, allowing for array[i][j][k] syntax. */
    inline class Slice operator [](int i)
    {
        ASSERT(i >= 0);
        ASSERT(i < m_size.x);
        return Slice(*this, i);
    }

    inline class ConstSlice operator [](int i) const
    {
        ASSERT(i >= 0);
        ASSERT(i < m_size.x);
        return ConstSlice(*this, i);
    }

    /* Resize the array.
     * FIXME: data gets scrambled; should we care? */
    inline void SetSize(ivec3 size, element_t e = element_t())
    {
        this->Resize(size.x * size.y * size.z, e);
        m_size = size;
    }

    inline ivec3 GetSize() const
    {
        return m_size;
    }

public:
    inline element_t *Data() { return super::Data(); }
    inline element_t const *Data() const { return super::Data(); }
    inline int Count() const { return super::Count(); }
    inline int Bytes() const { return super::Bytes(); }

private:
    ivec3 m_size;
};

} /* namespace lol */

#endif // __LOL_MATH_ARRAY3D_H__

