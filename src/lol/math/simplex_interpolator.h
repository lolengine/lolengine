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
    inline T Interp(vec_t<T, N> position)
    {
        // Computing position in simplex referential
        vec_t<T, N> simplex_position = this->ToSimplexRef(position);

        // Retrieving the closest floor point and decimals
        vec_t<int, N> floor_point;
        vec_t<T, N> decimal_point;

        this->ExtractFloorDecimal(simplex_position, floor_point, decimal_point);

        // Retrieving simplex samples to use
        int sign;

        this->GetReference(floor_point, decimal_point, sign);

        return this->LastInterp(floor_point, decimal_point, sign);
    }

protected:

    inline T LastInterp(vec_t<int, N> const & floor_point, vec_t<T, N> const & decimal_point, int const & sign)
    {
        T result = 0;
        T divider = 0;

        for (int i = 0 ; i < N ; ++i)
        {
            vec_t<int, N> samples_index = floor_point;
            samples_index[i] = this->Mod(samples_index[i] + sign, i);

            result += decimal_point[i] * this->samples[samples_index];
            divider += decimal_point[i];
        }

        T floor_coeff = 0;
        for (int i = 0 ; i < N ; ++i)
            floor_coeff += (1 - decimal_point[i]);

        result += (2 * sqrt(floor_coeff) / sqrt((float)N)) * this->samples[floor_point];
        divider += (2 * sqrt(floor_coeff) / sqrt((float)N));

        return result / divider;
    }

    inline int Mod(int value, int index)
    {
        return value %= this->samples.GetSize()[index];
    }

    inline void GetReference(vec_t<int, N> & floor_point, vec_t<T, N> & decimal_point, int & sign)
    {
        // Choosing the reference point which determines in which simplex we are working
        // ie. (0, 0, 0, …) and upper or (this->diagonal) and lower
        vec_t<int, N> ones(1);
        vec_t<int, N> zeros(0);

        T cumul = 0;
        for (int i = 0 ; i < N ; ++i)
            cumul += decimal_point[i];

        if (cumul < (sqrt((float)N) / 2))
        {
            sign = 1;
        }
        else
        {
            sign = -1;

            for (int i = 0 ; i < N ; ++i)
            {
                decimal_point[i] = 1 - decimal_point[i];
                floor_point[i] += 1;
            }
        }
    }

    inline void ExtractFloorDecimal(vec_t<T, N> const & simplex_position, vec_t<int, N> & floor_point, vec_t<T, N> & decimal_point)
    {
        // Finding floor point index
        for (int i = 0 ; i < N ; ++i)
            floor_point[i] = (int) simplex_position[i];

        // Extracting decimal part from simplex sample
        for (int i = 0 ; i < N ; ++i)
            decimal_point[i] = simplex_position[i] - floor_point[i];

        // Never exceed the size of the samples and loop on it
        for (int i = 0 ; i < N ; ++i)
            floor_point[i] = this->Mod(floor_point[i], i);
    }

    inline vec_t<T, N> ToSimplexRef(vec_t<T, N> const & position)
    {
        vec_t<T, N> result;

        for (int i = 0 ; i < N ; ++i)
        {
            result[i] = dot(base_inverse[i], position);
        }

        return result;
    }

    inline void InitBase()
    {
        this->samples.SetSize(vec_t<int, 2>(N, N));
        this->samples.SetSize(vec_t<int, 2>(N, N));

        for (int i = 0 ; i < N ; ++i)
        {
            for (int j = i ; j < N ; ++j)
            {
                this->base[i][j] = sqrt((i+2)/((float)(2*i+2))) / (j > i ? i+2 : 1);
                this->base_inverse[i][j] = sqrt((2*j+2) / ((float)(j+2))) * (j > i ? (1 / (float)(j+1)) : 1);
                this->diagonal[i] += (this->base[i][j]);
            }
        }
    }

    vec_t<vec_t<T, N>, N> base;
    vec_t<vec_t<T, N>, N> base_inverse;

    arraynd<N, T> samples;

    vec_t<T, N> diagonal;
};

}
