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

template<typename T, typename ARRAY>
void array_base<T, ARRAY>::Shuffle()
{
    auto n = Count();
    auto ni = n;
    while (n > 0)
    {
        ni = lol::rand(n--) | 0;
        Swap(ni, n);
    }
}

template<typename T, typename ARRAY>
void array_base<T, ARRAY>::Sort(SortAlgorithm algorithm)
{
#if !SORT_WORKS // yeah cause it's shite.
    algorithm = SortAlgorithm::Bubble;
#endif

    // Classic bubble
    if (algorithm == SortAlgorithm::Bubble)
    {
        int d = 1;
        for (ptrdiff_t i = 0; i < Count() - 1; i = lol::max(i + d, (ptrdiff_t)0))
        {
            if (i <= 0 || m_data[i] < m_data[i + 1])
                d = 1;
            if (m_data[i + 1] < m_data[i])
            {
                Swap(i, i + 1);
                d = -1;
            }
        }
    }
    // Quick sort with swap
    else if (algorithm == SortAlgorithm::QuickSwap)
    {
        SortQuickSwap(0, Count());
    }
}

template<typename T, typename ARRAY>
void array_base<T, ARRAY>::SortQuickSwap(ptrdiff_t start, ptrdiff_t stop)
{
    ptrdiff_t m[3] =
    {
        rand(start, stop),
        rand(start, stop),
        rand(start, stop)
    };

    for (int i = 0; i < 2; )
    {
        if (m_data[m[i+1]] < m_data[m[i]])
        {
            ptrdiff_t mt = m[i+1];
            m[i+1] = m[i];
            m[i] = mt;
            i = 0;
        }
        else
            i++;
    }
    //actual stuff
    T median = m_data[m[1]];
    ptrdiff_t i0 = start;
    ptrdiff_t i1 = stop - 1;
    bool b_swap = false;
    while (i0 < i1)
    {
        if (!(m_data[i0] < median) && m_data[i1] < median)
        {
            Swap(i0, i1);
            i0++;
            i1--;
            b_swap = true;
        }
        else
        {
            if (m_data[i0] < median)
                i0++;
            if (!(m_data[i1] < median))
                i1--;
        }
    }
    if (stop - start == 1 || !b_swap)
        return;
    if (start < i0)
        SortQuickSwap(start, i0);
    if (i0 < stop)
        SortQuickSwap(i0, stop);
}

} /* namespace lol */

