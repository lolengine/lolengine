//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//            (c) 2013-2014 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2013-2014 Guillaume Bittoun <guillaume.bittoun@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The arraynd class
// -----------------
// A N-Dimensional array class allowing var[i][j][k]... indexing,
//

//
// FIXME: This file is in lol/math/ instead of lol/base/ because it
// uses vec_t.
//

#if !defined __LOL_MATH_ARRAYND_H__
#define __LOL_MATH_ARRAYND_H__

#include <lol/base/array.h>
#include <lol/base/assert.h>

#include <type_traits>

namespace lol
{


template<typename T, ptrdiff_t L>
class arraynd_initializer
{
public:

    arraynd_initializer(std::initializer_list<arraynd_initializer<T, L - 1> > const & initializers) :
        m_initializers(initializers)
    {
    }

    void FillSizes(ptrdiff_t * sizes)
    {
        *sizes = std::max(*sizes, (ptrdiff_t)(m_initializers.size()));

        for (auto subinitializer : m_initializers)
            subinitializer.FillSizes(sizes - 1);
    }

    void FillValues(T * origin, ptrdiff_t prev, ptrdiff_t * sizes)
    {
        ptrdiff_t pos = 0;

        for (auto subinitializer : m_initializers)
            subinitializer.FillValues(origin, pos++ + prev * *sizes, sizes - 1);
    }

private:

    std::initializer_list<arraynd_initializer<T, L - 1> > const & m_initializers;
};


template<typename T>
class arraynd_initializer<T, 1>
{
public:

    arraynd_initializer(std::initializer_list<T> const & initializers) :
        m_initializers(initializers)
    {
    }

    void FillSizes(ptrdiff_t * sizes)
    {
        *sizes = std::max(*sizes, (ptrdiff_t)(m_initializers.size()));
    }

    void FillValues(T * origin, ptrdiff_t prev, ptrdiff_t * sizes)
    {
        UNUSED(sizes);

        ptrdiff_t pos = 0;

        for (auto value : m_initializers)
            *(origin + prev * *sizes + pos++) = value;
    }

private:

    std::initializer_list<T> const & m_initializers;
};


template<ptrdiff_t N, typename... T>
class arraynd : protected array<T...>
{
public:
    typedef array<T...> super;
    typedef typename super::element_t element_t;

    inline arraynd()
    {
    }

    inline arraynd(vec_t<ptrdiff_t, N> sizes, element_t e = element_t())
      : m_sizes(sizes)
    {
        FixSizes(e);
    }

    inline arraynd(std::initializer_list<arraynd_initializer<element_t, N - 1> > initializer)
    {
        m_sizes[N - 1] = initializer.size();

        for (auto inner_initializer : initializer)
            inner_initializer.FillSizes(&m_sizes[N - 2]);

        FixSizes();

        ptrdiff_t pos = 0;

        for (auto inner_initializer : initializer)
            inner_initializer.FillValues(&super::operator[](0), pos++, &m_sizes[N - 2]);
    }

    /* Access elements directly using an ivec2, ivec3 etc. index */
    inline element_t const & operator[](vec_t<int, N> const &pos) const
    {
        ptrdiff_t n = pos[N - 1];
        for (ptrdiff_t i = N - 2; i >= 0; --i)
            n = pos[i] + m_sizes[i + 1] * n;
        return super::operator[](n);
    }

    inline element_t & operator[](vec_t<ptrdiff_t, N> const &pos)
    {
        return const_cast<element_t &>(
                   const_cast<arraynd<N, T...> const&>(*this)[pos]);
    }

    /* Proxy to access slices */
    template<typename ARRAY_TYPE, ptrdiff_t L = N - 1>
    class slice
    {
    public:
        typedef slice<ARRAY_TYPE, L - 1> subslice;

        inline slice(ARRAY_TYPE &array, ptrdiff_t index, ptrdiff_t accumulator)
          : m_array(array),
            m_index(index),
            m_accumulator(accumulator)
        {
        }

        /* Accessors for the const version of the proxy */
        template<bool V = L != 1 && std::is_const<ARRAY_TYPE>::value>
        inline typename std::enable_if<V, subslice>::type
        operator[](ptrdiff_t pos) const
        {
            return subslice(m_array, m_index + pos * m_accumulator,
                            m_accumulator * m_array.m_sizes[N - L]);
        }

        template<bool V = L == 1 && std::is_const<ARRAY_TYPE>::value>
        inline typename std::enable_if<V, typename ARRAY_TYPE::element_t>::type
        const & operator[](ptrdiff_t pos) const
        {
            return m_array.super::operator[](m_index + pos * m_accumulator);
        }

        /* Accessors for the non-const version of the proxy */
        template<bool V = L != 1 && !std::is_const<ARRAY_TYPE>::value>
        inline typename std::enable_if<V, subslice>::type
        operator[](ptrdiff_t pos)
        {
            return subslice(m_array, m_index + pos * m_accumulator,
                            m_accumulator * m_array.m_sizes[N - L]);
        }

        template<bool V = L == 1 && !std::is_const<ARRAY_TYPE>::value>
        inline typename std::enable_if<V, typename ARRAY_TYPE::element_t>::type
        & operator[](ptrdiff_t pos)
        {
            return m_array.super::operator[](m_index + pos * m_accumulator);
        }

    private:
        ARRAY_TYPE &m_array;
        ptrdiff_t m_index, m_accumulator;
    };

    /* Access addressable slices, allowing for array[i][j][...] syntax. */
    inline slice<arraynd<N, T...> const> operator[](ptrdiff_t pos) const
    {
        return slice<arraynd<N, T...> const>(*this, pos, m_sizes[0]);
    }

    inline slice<arraynd<N, T...>> operator[](ptrdiff_t pos)
    {
        return slice<arraynd<N, T...>>(*this, pos, m_sizes[0]);
    }

    /* Resize the array.
     * FIXME: data gets scrambled; should we care? */
    inline void SetSize(vec_t<ptrdiff_t, N> sizes, element_t e = element_t())
    {
        m_sizes = sizes;
        FixSizes(e);
    }

    inline vec_t<ptrdiff_t, N> GetSize() const
    {
        return this->m_sizes;
    }

public:
    inline element_t *Data() { return super::Data(); }
    inline element_t const *Data() const { return super::Data(); }
    inline ptrdiff_t Count() const { return super::Count(); }
    inline ptrdiff_t Bytes() const { return super::Bytes(); }

private:
    inline void FixSizes(element_t e = element_t())
    {
        ptrdiff_t total_size = 1;

        for (auto size : m_sizes)
            total_size *= size;

        this->Resize(total_size, e);
    }

    vec_t<ptrdiff_t, N> m_sizes;
};

} /* namespace lol */

#endif // __LOL_MATH_ARRAYND_H__

