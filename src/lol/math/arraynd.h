//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//            © 2013—2014 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//            © 2013—2014 Guillaume Bittoun <guillaume.bittoun@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The arraynd class
// -----------------
// A N-Dimensional array class allowing var[i][j][k]... indexing,
//

//
// XXX: This file is in lol/math/ instead of lol/base/ because it uses vec_t.
//

#include <lol/base/array.h>
#include <lol/base/assert.h>

#include <cstddef>
#include <climits>
#include <type_traits>

namespace lol
{

template<typename T, int L>
class arraynd_initializer
{
public:

    arraynd_initializer(std::initializer_list<arraynd_initializer<T, L - 1> > const & initializers) :
        m_initializers(initializers)
    {
    }

    void fill_sizes(ptrdiff_t * sizes)
    {
        *sizes = max(*sizes, (ptrdiff_t)m_initializers.size());

        for (auto subinitializer : m_initializers)
            subinitializer.fill_sizes(sizes - 1);
    }

    void fill_values(T * origin, ptrdiff_t prev, ptrdiff_t * sizes)
    {
        int pos = 0;

        for (auto subinitializer : m_initializers)
            subinitializer.fill_values(origin, pos++ + prev * *sizes, sizes - 1);
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

    void fill_sizes(ptrdiff_t * sizes)
    {
        *sizes = max(*sizes, (ptrdiff_t)m_initializers.size());
    }

    void fill_values(T * origin, ptrdiff_t prev, ptrdiff_t * sizes)
    {
        int pos = 0;

        for (auto value : m_initializers)
            *(origin + prev * *sizes + pos++) = value;
    }

private:

    std::initializer_list<T> const & m_initializers;
};


template<int N, typename... T>
class [[nodiscard]] arraynd : protected array<T...>
{
public:
    typedef array<T...> super;
    typedef typename super::element_t element_t;

    inline arraynd() = default;

    inline arraynd(vec_t<ptrdiff_t, N> sizes, element_t e = element_t())
      : m_sizes(sizes)
    {
        resize_data(e);
    }

    /* Additional constructor if ptrdiff_t != int */
    template<typename T2 = int, typename T3 = typename std::enable_if<!std::is_same<ptrdiff_t, T2>::value, int>::type>
    inline arraynd(vec_t<T2, N> sizes, element_t e = element_t())
      : m_sizes(vec_t<ptrdiff_t, N>(sizes))
    {
        resize_data(e);
    }

    inline arraynd(std::initializer_list<arraynd_initializer<element_t, N - 1> > initializer)
    {
        m_sizes[N - 1] = initializer.size();

        for (auto inner_initializer : initializer)
            inner_initializer.fill_sizes(&m_sizes[N - 2]);

        resize_data();

        int pos = 0;

        for (auto inner_initializer : initializer)
            inner_initializer.fill_values(&super::operator[](0), pos++, &m_sizes[N - 2]);
    }

    /* Access elements directly using an vec_t<ptrdiff_t, N> index */
    inline element_t const & operator[](vec_t<ptrdiff_t, N> const &pos) const
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

    /* If int != ptrdiff_t, access elements directly using an ivec2,
     * ivec3 etc. */
    template<typename T2 = int, typename T3 = typename std::enable_if<!std::is_same<ptrdiff_t, T2>::value, int>::type>
    inline element_t const & operator[](vec_t<T2, N> const &pos) const
    {
        ptrdiff_t n = pos[N - 1];
        for (ptrdiff_t i = N - 2; i >= 0; --i)
            n = pos[i] + m_sizes[i + 1] * n;
        return super::operator[](n);
    }

    template<typename T2 = int, typename T3 = typename std::enable_if<!std::is_same<ptrdiff_t, T2>::value, int>::type>
    inline element_t & operator[](vec_t<T2, N> const &pos)
    {
        return const_cast<element_t &>(
                   const_cast<arraynd<N, T...> const&>(*this)[pos]);
    }

    /* Proxy to access slices */
    template<typename ARRAY_TYPE, int L = N - 1>
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
    inline void resize(vec_t<int, N> sizes, element_t e = element_t())
    {
        resize_s(vec_t<ptrdiff_t, N>(sizes), e);
    }

    inline void resize_s(vec_t<ptrdiff_t, N> sizes, element_t e = element_t())
    {
        m_sizes = sizes;
        resize_data(e);
    }

    inline vec_t<int, N> size() const
    {
        return vec_t<int, N>(this->m_sizes);
    }

    inline vec_t<ptrdiff_t, N> size_s() const
    {
        return this->m_sizes;
    }

public:
    inline element_t *data() { return super::data(); }
    inline element_t const *data() const { return super::data(); }
    inline int count() const { return super::count(); }
    inline int bytes() const { return super::bytes(); }
    inline ptrdiff_t count_s() const { return super::count_s(); }
    inline ptrdiff_t bytes_s() const { return super::bytes_s(); }

private:
    inline void resize_data(element_t e = element_t())
    {
        ptrdiff_t total_size = 1;

        /* HACK: we can't use for (auto s : m_sizes) because of an ICE in
         * the Visual Studio compiler. */
        for (int i = 0; i < N; ++i)
            total_size *= m_sizes[i];

        this->array<T...>::resize(total_size, e);
    }

    vec_t<ptrdiff_t, N> m_sizes { 0 };
};

template<typename... T> using array2d = arraynd<2, T...>;
template<typename... T> using array3d = arraynd<3, T...>;

} /* namespace lol */

