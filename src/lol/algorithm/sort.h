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

#include <lol/base/array.h>

namespace lol
{

/*
 * Shuffle an array.
 */

template<typename T, typename ARRAY>
void array_base<T, ARRAY>::shuffle()
{
    auto n = count();
    auto ni = n;
    while (n > 0)
    {
        ni = lol::rand(n--) | 0;
        this->swap(ni, n);
    }
}

/*
 * Sort an array
 */

template<typename T, typename ARRAY>
static void quick_swap_sort(array_base<T, ARRAY> &a,
                            ptrdiff_t start, ptrdiff_t stop);

template<typename T, typename ARRAY>
void array_base<T, ARRAY>::sort(SortAlgorithm algorithm)
{
#if !SORT_WORKS // yeah cause it's shite.
    algorithm = SortAlgorithm::Bubble;
#endif

    // Classic bubble
    if (algorithm == SortAlgorithm::Bubble)
    {
        int d = 1;
        for (ptrdiff_t i = 0; i < count_s() - 1; i = lol::max(i + d, (ptrdiff_t)0))
        {
            if (i <= 0 || m_data[i] < m_data[i + 1])
                d = 1;
            if (m_data[i + 1] < m_data[i])
            {
                this->swap(i, i + 1);
                d = -1;
            }
        }
    }
    // Quick sort with swap
    else if (algorithm == SortAlgorithm::QuickSwap)
    {
        quick_swap_sort(*this, 0, count_s());
    }
}

template<typename T, typename ARRAY>
static void quick_swap_sort(array_base<T, ARRAY> &a,
                            ptrdiff_t start, ptrdiff_t stop)
{
    ptrdiff_t m[3] =
    {
        rand(start, stop),
        rand(start, stop),
        rand(start, stop)
    };

    for (int i = 0; i < 2; )
    {
        if (a[m[i+1]] < a[m[i]])
        {
            ptrdiff_t mt = m[i+1];
            m[i+1] = m[i];
            m[i] = mt;
            i = 0;
        }
        else
            i++;
    }

    // actual stuff
    T median = a[m[1]];
    ptrdiff_t i0 = start, i1 = stop - 1;
    bool b_swap = false;
    while (i0 < i1)
    {
        if (!(a[i0] < median) && a[i1] < median)
        {
            a.swap(i0, i1);
            i0++;
            i1--;
            b_swap = true;
        }
        else
        {
            if (a[i0] < median)
                i0++;
            if (!(a[i1] < median))
                i1--;
        }
    }
    if (stop - start == 1 || !b_swap)
        return;
    if (start < i0)
        quick_swap_sort(a, start, i0);
    if (i0 < stop)
        quick_swap_sort(a, i0, stop);
}

} /* namespace lol */

