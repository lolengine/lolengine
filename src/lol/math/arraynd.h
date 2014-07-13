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

namespace lol
{

template<size_t N, size_t L, typename ARRAY_TYPE>
class arraynd_proxy
{
public:

    typedef arraynd_proxy<N, L - 1, ARRAY_TYPE> subproxy;

    inline arraynd_proxy(ARRAY_TYPE * array, vec_t<size_t, N> const & sizes, size_t index, size_t accumulator) :
        m_array(array),
        m_sizes(sizes),
        m_index(index),
        m_accumulator(accumulator)
    {
    }

    inline subproxy operator[](size_t pos)
    {
        return subproxy(m_array, this->m_sizes, m_index + pos * m_accumulator, m_accumulator * m_sizes[N - L]);
    }

private:

    ARRAY_TYPE * m_array;
    vec_t<size_t, N> const & m_sizes;
    size_t m_index;
    size_t m_accumulator;
};


template<size_t N, typename ARRAY_TYPE>
class arraynd_proxy<N, 1, ARRAY_TYPE>
{
public:

    inline arraynd_proxy(ARRAY_TYPE * array, vec_t<size_t, N> const & sizes, size_t index, size_t accumulator) :
        m_array(array),
        m_sizes(sizes),
        m_index(index),
        m_accumulator(accumulator)
    {
    }

    inline typename ARRAY_TYPE::element_t & operator[](size_t pos)
    {
        return m_array[m_index + pos * m_accumulator];
    }

private:

    ARRAY_TYPE * m_array;
    vec_t<size_t, N> const & m_sizes;
    size_t m_index;
    size_t m_accumulator;
};


template<typename T, size_t L>
class arraynd_initializer
{
public:

    arraynd_initializer(std::initializer_list<arraynd_initializer<T, L - 1> > const & initializers) :
        m_initializers(initializers)
    {
    }

    void FillSizes(size_t * sizes)
    {
        *sizes = std::max(*sizes, m_initializers.size());

        for (auto subinitializer : m_initializers)
            subinitializer.FillSizes(sizes + 1);
    }

    void FillValues(T * values, size_t * sizes, size_t accumulator)
    {
        int pos = 0;

        for (auto subinitializer : m_initializers)
        {
            subinitializer.FillValues(values + pos * accumulator, sizes + 1, accumulator * *sizes);
            ++pos;
        }
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

    void FillSizes(size_t * sizes)
    {
        *sizes = std::max(*sizes, m_initializers.size());
    }

    void FillValues(T * values, size_t * sizes, size_t accumulator)
    {
        int pos = 0;

        for (auto value : m_initializers)
        {
            *(values + pos * accumulator) = value;
            ++pos;
        }
    }

private:

    std::initializer_list<T> const & m_initializers;
};


template<size_t N, typename... T>
class arraynd : protected array<T...>
{
public:
    typedef array<T...> super;
    typedef typename super::element_t element_t;
    typedef arraynd_proxy<N, N - 1, super> proxy;

    inline arraynd() :
        super(),
        m_sizes()
    {
    }

    inline arraynd(vec_t<size_t, N> sizes, element_t e = element_t()) :
        super(),
        m_sizes(sizes)
    {
        SetSize(m_sizes, e);
    }

    inline arraynd(arraynd_initializer<element_t, N> initializer) :
        super(),
        m_sizes()
    {
        initializer.FillSizes(&m_sizes[0]);
        SetSize(m_sizes);
        initializer.FillValues(&super::operator[](0), &m_sizes[0], 1);
    }

    /* Access elements directly using index position */
    inline proxy operator[](size_t pos) const
    {
        return proxy(this, m_sizes, pos, m_sizes[0]);
    }

    /* Resize the array.
     * FIXME: data gets scrambled; should we care? */
    inline void SetSize(vec_t<size_t, N> sizes, element_t e = element_t())
    {
        size_t total_size = 1;

        for (auto size : sizes)
            total_size *= size;

        this->Resize(total_size, e);
    }

    inline vec_t<size_t, N> GetSize() const
    {
        return this->m_sizes;
    }

public:
    inline element_t *Data() { return super::Data(); }
    inline element_t const *Data() const { return super::Data(); }
    inline int Count() const { return super::Count(); }
    inline int Bytes() const { return super::Bytes(); }

private:
    vec_t<size_t, N> m_sizes;
};


template<typename... T>
class arraynd<1, T...> : protected array<T...>
{
public:
    typedef array<T...> super;
    typedef typename super::element_t element_t;

    inline arraynd() :
        super(),
        m_sizes()
    {
    }

    inline arraynd(vec_t<size_t, 1> sizes, element_t e = element_t()) :
        super(),
        m_sizes(sizes)
    {
        SetSize(m_sizes, e);
    }

    inline arraynd(std::initializer_list<element_t> initializer) :
        super(),
        m_sizes()
    {
        m_sizes[0] = initializer.size();
        SetSize(m_sizes[0]);

        size_t pos = 0;
        for (auto element : initializer)
            (*this)[pos++] = element;
    }

    /* Access elements directly using index position */
    inline element_t & operator[](size_t pos) const
    {
        return super::operator[](pos);
    }

    /* Resize the array.
     * FIXME: data gets scrambled; should we care? */
    inline void SetSize(vec_t<size_t, 1> sizes, element_t e = element_t())
    {
        this->Resize(m_sizes[0], e);
    }

    inline vec_t<size_t, 1> GetSize() const
    {
        return this->m_sizes;
    }

public:
    inline element_t *Data() { return super::Data(); }
    inline element_t const *Data() const { return super::Data(); }
    inline int Count() const { return super::Count(); }
    inline int Bytes() const { return super::Bytes(); }

private:
    vec_t<size_t, 1> m_sizes;
};

} /* namespace lol */

#endif // __LOL_MATH_ARRAYND_H__

