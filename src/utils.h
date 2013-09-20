//
// Lol Engine
//
// Copyright: (c) 2012-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2012-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// Various basic convenience functions
// ------------------
//

#if !defined __LOL_UTILS_H__
#define __LOL_UTILS_H__

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
template< class T > inline int GetRandom(Array<T> src)
{
    float r_total = 0.f;
    float r_alpha = rand(1.f);
    float r_value = 0.f;
    int r_j = 0;

    int i = 0;
    for (; i < src.Count(); ++i)
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

//Gets the value for the given safe-enum, considering you have implemented what follows :
//NEEDS : Contructor(Value)
//NEEDS : static char const *GetName(Value v)
//NEEDS : T::Max
template< class T > inline T FindValue(const char* name)
{
    String n = name;
    n.ToLower();
    for (int i = 0; i < T::Max; ++i)
    {
        String s = T::GetName(T::Value(i));
        if (s.ToLower().IndexOf(n.C()) > -1)
            return T(T::Value(i));
    }
    return T::Max;
}

} /* namespace lol */

#endif /* __LOL_UTILS_H__ */

