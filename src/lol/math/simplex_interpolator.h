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

#pragma once

template<int N /* Dimension of the space */, typename T = float /* floating type used for interpolation */>
class simplex_interpolator
{
public:

    simplex_interpolator()
    {
        this->InitBase();
    }

    inline arraynd<N, T> & GetSamples()
    {
        return this->samples;
    }

    // Single interpolation
    inline T interp(vec_t<T, N> position)
    {
        vec_t<T, N> simplex_position = this->ToSimplexRef(position);

        // TODO

        return 0;
    }

protected:

    inline void InitBase()
    {
        base.SetSize(vec_t<int, 2>(N, N));
        base_inverse.SetSize(vec_t<int, 2>(N, N));

        for (int i = 0 ; i < N ; ++i)
        {
            for (int j = i ; j < N ; ++j)
            {
                base[i][j] = sqrt((i+2)/(2*i + 2)) / (j > i ? i + 2 : 1);
                base_inverse[i][j] = sqrt((2*j + 2)/(j+2)) * (j > i ? (1 / (float)j) : 1);
            }
        }
    }

    inline vec_t<T, N> ToSimplexRef(vec_t<T, N> const & position)
    {
        vec_t<T, N> result;

        for (int i = 0 ; i < N ; ++i)
        {
            for (int j = i ; j < N ; ++j)
            {
                result[i] += base_inverse[i][j] * position[j];
            }
        }

        return result;
    }

    arraynd<2, T> base;
    arraynd<2, T> base_inverse;

    arraynd<N, T> samples;
};
