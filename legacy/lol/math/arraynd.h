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

#include <lol/math/vector.h> // vec_t

#include <vector>      // std::vector
#include <algorithm>   // std::min
#include <cstring>     // memset
#include <climits>
#include <type_traits> // std::enable_if

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

    void fill_sizes(size_t * sizes)
    {
        *sizes = std::max(*sizes, m_initializers.size());

        for (auto subinitializer : m_initializers)
            subinitializer.fill_sizes(sizes - 1);
    }

    void fill_values(T * origin, size_t prev, size_t * sizes)
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

    void fill_sizes(size_t * sizes)
    {
        *sizes = std::max(*sizes, m_initializers.size());
    }

    void fill_values(T * origin, size_t prev, size_t * sizes)
    {
        int pos = 0;

        for (auto value : m_initializers)
            *(origin + prev * *sizes + pos++) = value;
    }

private:

    std::initializer_list<T> const & m_initializers;
};


template<int N, typename T>
class [[nodiscard]] arraynd
{
public:
    typedef T element_t;

    inline arraynd() = default;

    inline arraynd(vec_t<size_t, N> sizes, element_t e = element_t())
      : m_sizes(sizes)
    {
        resize_data(e);
    }

    /* Additional constructor if size_t != int */
    template<typename T2 = int, typename T3 = typename std::enable_if<!std::is_same<size_t, T2>::value, int>::type>
    inline arraynd(vec_t<T2, N> sizes, element_t e = element_t())
      : m_sizes(vec_t<size_t, N>(sizes))
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
            inner_initializer.fill_values(&m_data[0], pos++, &m_sizes[N - 2]);
    }

    /* Access elements directly using an vec_t<size_t, N> index */
    inline element_t const & operator[](vec_t<size_t, N> const &pos) const
    {
        size_t n = pos[N - 1];
        for (size_t i = N - 1; i > 0; --i)
            n = pos[i - 1] + m_sizes[i] * n;
        return m_data[n];
    }

    inline element_t & operator[](vec_t<size_t, N> const &pos)
    {
        return const_cast<element_t &>(
                   const_cast<arraynd<N, T> const&>(*this)[pos]);
    }

    /* If int != size_t, access elements directly using an ivec2,
     * ivec3 etc. */
    template<typename T2 = int, typename T3 = typename std::enable_if<!std::is_same<size_t, T2>::value, int>::type>
    inline element_t const & operator[](vec_t<T2, N> const &pos) const
    {
        size_t n = pos[N - 1];
        for (size_t i = N - 1; i > 0; --i)
            n = pos[i - 1] + m_sizes[i] * n;
        return m_data[n];
    }

    template<typename T2 = int, typename T3 = typename std::enable_if<!std::is_same<size_t, T2>::value, int>::type>
    inline element_t & operator[](vec_t<T2, N> const &pos)
    {
        return const_cast<element_t &>(
                   const_cast<arraynd<N, T> const&>(*this)[pos]);
    }

    /* Proxy to access slices */
    template<typename ARRAY_TYPE, int L = N - 1>
    class slice
    {
    public:
        typedef slice<ARRAY_TYPE, L - 1> subslice;

        inline slice(ARRAY_TYPE &array, size_t index, size_t accumulator)
          : m_array(array),
            m_index(index),
            m_accumulator(accumulator)
        {
        }

        /* Accessors for the const version of the proxy */
        template<bool V = L != 1 && std::is_const<ARRAY_TYPE>::value>
        inline typename std::enable_if<V, subslice>::type
        operator[](size_t pos) const
        {
            return subslice(m_array, m_index + pos * m_accumulator,
                            m_accumulator * m_array.m_sizes[N - L]);
        }

        template<bool V = L == 1 && std::is_const<ARRAY_TYPE>::value>
        inline typename std::enable_if<V, typename ARRAY_TYPE::element_t>::type
        const & operator[](size_t pos) const
        {
            return m_array.m_data[m_index + pos * m_accumulator];
        }

        /* Accessors for the non-const version of the proxy */
        template<bool V = L != 1 && !std::is_const<ARRAY_TYPE>::value>
        inline typename std::enable_if<V, subslice>::type
        operator[](size_t pos)
        {
            return subslice(m_array, m_index + pos * m_accumulator,
                            m_accumulator * m_array.m_sizes[N - L]);
        }

        template<bool V = L == 1 && !std::is_const<ARRAY_TYPE>::value>
        inline typename std::enable_if<V, typename ARRAY_TYPE::element_t>::type
        & operator[](size_t pos)
        {
            return m_array.m_data[m_index + pos * m_accumulator];
        }

    private:
        ARRAY_TYPE &m_array;
        size_t m_index, m_accumulator;
    };

    /* Access addressable slices, allowing for array[i][j][...] syntax. */
    inline slice<arraynd<N, T> const> operator[](size_t pos) const
    {
        return slice<arraynd<N, T> const>(*this, pos, m_sizes[0]);
    }

    inline slice<arraynd<N, T>> operator[](size_t pos)
    {
        return slice<arraynd<N, T>>(*this, pos, m_sizes[0]);
    }

    /* Resize the array.
     * FIXME: data gets scrambled; should we care? */
    inline void resize(vec_t<int, N> sizes, element_t e = element_t())
    {
        resize_s(vec_t<size_t, N>(sizes), e);
    }

    inline void resize_s(vec_t<size_t, N> sizes, element_t e = element_t())
    {
        m_sizes = sizes;
        resize_data(e);
    }

    inline vec_t<int, N> size() const
    {
        return vec_t<int, N>(this->m_sizes);
    }

public:
    inline element_t *data() { return m_data.data(); }
    inline element_t const *data() const { return m_data.data(); }
    inline int total_size() const { return m_data.size(); }
    inline size_t bytes() const { return total_size() * sizeof(element_t); }

private:
    inline void resize_data(element_t e = element_t())
    {
        size_t total_size = 1;

        /* HACK: we can't use for (auto s : m_sizes) because of an ICE in
         * the Visual Studio compiler. */
        for (int i = 0; i < N; ++i)
            total_size *= m_sizes[i];

        m_data.resize(total_size, e);
    }

    std::vector<T> m_data;
    vec_t<size_t, N> m_sizes { 0 };
};

template<typename T> using array2d = arraynd<2, T>;
template<typename T> using array3d = arraynd<3, T>;

} /* namespace lol */

