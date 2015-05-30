//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2012—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// Various basic convenience functions
// ------------------
//

#include <lol/base/string.h>

namespace lol
{

//Swaps two given values.
template< class T > inline void Swap( T& A, T& B )
{
    const T Temp = A;
    A = B;
    B = Temp;
}

//TODO: random struct
//Gets a random Element from the given array<T>, considering you have implemented what follows :
//NEEDS : float T::m_weight; //if m_weight is 0, it automatically assumes that this step is ignored.
template< class T > inline int GetRandom(array<T> src)
{
    float r_total = 0.f;
    float r_alpha = rand(1.f);
    float r_value = 0.f;
    int r_j = 0;

    for (int i = 0; i < src.count(); ++i)
    {
        T& tmp = src[i];
        if (tmp.m_weight > .0f)
        {
            r_total += tmp.m_weight;
            float r_tmp = r_alpha * r_total;
            while (r_tmp > r_value + src[r_j].m_weight && r_j < i)
            {
                r_value += src[r_j].m_weight;
                r_j++;
            }
        }
    }

    return (r_total > .0f)?(r_j):(-1);
}

// Gets the value for the given enum type.
template<class T> inline T FindValue(const char* name)
{
    String n = name;
    n.to_lower();
    for (int i = 0; i < T::Max; ++i)
    {
        String s = T(i).ToString().to_lower();
        if (s.contains(n))
            return T(i);
    }
    return T::Max;
}
template<class T> inline T FindValue(String const& name)
{
    return FindValue<T>(name.C());
}

} /* namespace lol */

