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
// The arrayNd class
// -----------------
// A N-Dimensional array class allowing var[i][j][k]... indexing,
//

//
// FIXME: This file is in lol/math/ instead of lol/base/ because it
// uses ivec3.
//

#if !defined __LOL_MATH_ARRAYND_H__
#define __LOL_MATH_ARRAYND_H__

#include <lol/base/array.h>
#include <lol/base/assert.h>

namespace lol
{

template<size_t N, typename T1, typename T2 = void, typename T3 = void,
         typename T4 = void, typename T5 = void, typename T6 = void,
         typename T7 = void, typename T8 = void>
class arraynd : protected array<T1, T2, T3, T4, T5, T6, T7, T8>
{
public:
    typedef array<T1, T2, T3, T4, T5, T6, T7, T8> super;
    typedef typename super::element_t element_t;

    inline arraynd() :
        m_sizes()
    {
    }

    inline arraynd(vec_t<size_t, N> sizes, element_t e = element_t()) :
        m_sizes(sizes)
    {
        SetSize(m_sizes, e);
    }

    /* Access elements directly using an ivec3 index */
    inline element_t const &operator [](ivec3 pos) const
    {
        return element_t(); // TODO
    }

    inline size_t ComputeTotalSize(vec_t<size_t, N> sizes)
    {
        size_t total_size = 1;

        for (auto size : sizes)
            total_size *= size;

        return total_size;
    }

    /* Resize the array.
     * FIXME: data gets scrambled; should we care? */
    inline void SetSize(vec_t<size_t, N> sizes, element_t e = element_t())
    {
        this->Resize(ComputeTotalSize(sizes), e);
    }

    inline vec_t<size_t, N> GetSize() const
    {
        return ComputeTotalSize(this->m_sizes);
    }

public:
    inline element_t *Data() { return super::Data(); }
    inline element_t const *Data() const { return super::Data(); }
    inline int Count() const { return super::Count(); }
    inline int Bytes() const { return super::Bytes(); }

private:
    vec_t<size_t, N> m_sizes;
};

} /* namespace lol */

#endif // __LOL_MATH_ARRAYND_H__

