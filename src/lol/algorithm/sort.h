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

#if !defined __LOL_ALGORITHM_SORT_H__
#define __LOL_ALGORITHM_SORT_H__

#include <lol/base/array.h>

namespace lol
{

template<typename T, typename ARRAY>
void ArrayBase<T, ARRAY>::Sort(int sort)
{
    int s = 0;
    // Classic bubble
    if (s++ == sort)
    {
        int d = 1;
        for (int i = 0; i < Count() - 1; i = lol::max(i + d, 0))
        {
            if (m_data[i] < m_data[i + 1] || i <= 0)
                d = 1;
            if (m_data[i + 1] < m_data[i])
            {
                Swap(i, i + 1);
                d = -1;
            }
        }
    }
    // Quick sort with swap
    else if (s++ == sort)
    {
        SortQuickSwap(0, Count());
    }
}

template<typename T, typename ARRAY>
void ArrayBase<T, ARRAY>::SortQuickSwap(int start, int stop)
{
    int m[3] =
    {
        rand(start, stop),
        rand(start, stop),
        rand(start, stop)
    };

    for (int i = 0; i < 2; )
    {
        if (m_data[m[i+1]] < m_data[m[i]])
        {
            int mt = m[i+1];
            m[i+1] = m[i];
            m[i] = mt;
            i = 0;
        }
        else
            i++;
    }
    //actual stuff
    T median = m_data[m[1]];
    int i0 = start;
    int i1 = stop - 1;
    bool swap = false;
    while (i0 < i1)
    {
        if (m_data[i0] >= median && m_data[i1] < median)
        {
            Swap(i0, i1);
            i0++;
            i1--;
            swap = true;
        }
        else
        {
            if (m_data[i0] < median)
                i0++;
            if (m_data[i1] >= median)
                i1--;
        }
    }
    if (stop - start == 1 || !swap)
        return;
    if (start < i0)
        SortQuickSwap(start, i0);
    if (i0 < stop)
        SortQuickSwap(i0, stop);
}

} /* namespace lol */

#endif // __LOL_ALGORITHM_SORT_H__

