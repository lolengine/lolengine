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

namespace lol
{

// Vector helpers
template<int N>
vec_t<int, N> GetUniformPoint(int value) // constexpr ?
{
    vec_t<int, N> result;
    for (int i = 0 ; i < N ; ++i)
        result[i] = value;

    return result;
}

template<typename T, typename U, int N>
float SquareDistance(vec_t<T, N> const & a, vec_t<U, N> const & b)
{
    float result = 0;

    for (int i = 0 ; i < N ; ++i)
        result += (a[i] - b[i]) * (a[i] - b[i]);

    return result;
}

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
        // Computing position in simplex referential
        vec_t<T, N> simplex_position = this->ToSimplexRef(position);


        // Retrieving the closest floor point and decimals
        vec_t<int, N> & floor_point;
        vec_t<T, N> & decimal_point;

        this->ExtractFloorDecimal(simplex_position, floor_point, decimal_point);

        // Retrieving simplex samples to use
        vec_t<int, N> reference;
        int sign;

        this->GetReference(decimal_point, reference, sign);

        // TODO : Last interpolation step

        return 0;
    }

protected:

    void GetReference(vec_t<T, N> const & decimal_point, vec_t<int, N> & reference, int & sign)
    {
        // Choosing the reference point which determines in which simplex we are working
        // ie. (0, 0, 0, …) and upper or (1, 1, 1, …) and lower
        vec_t<int, N> diagonal_point = GetUniformPoint(1);
        vec_t<int, N> zero_point = GetUniformPoint(0);


        if (SquareDistance(zero_point, floor_point) < SquareDistance(diagonal_point, floor_point))
        {
            reference = zero_point;
            sign = 1;
        }
        else
        {
            reference = diagonal_point;
            sign = -1;
        }
    }

    void ExtractFloorDecimal(vec_t<T, N> const & simplex_position, vec_t<int, N> & floor_point, vec_t<T, N> & decimal_point)
    {
        // Finding floor point index
        for (int i = 0 ; i < N ; ++i)
            floor_point[i] = (int) simplex_position[i];

        // Extracting decimal part from simplex sample
        for (int i = 0 ; i < N ; ++i)
            decimal_point[i] = simplex_position[i] - floor_point[i];
    }

    inline void InitBase()
    {
        base.SetSize(vec_t<int, 2>(N, N));
        base_inverse.SetSize(vec_t<int, 2>(N, N));

        for (int i = 0 ; i < N ; ++i)
        {
            for (int j = i ; j < N ; ++j)
            {
                base[i][j] = sqrt((i+2)/(2*i+2)) / (j > i ? i+2 : 1);
                base_inverse[i][j] = sqrt((2*j+2)/(j+2)) * (j > i ? (1 / (float)(j+1)) : 1);
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

}
