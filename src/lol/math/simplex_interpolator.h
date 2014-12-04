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

template<int N, /* Dimension of the space */
         typename T = float> /* Interpolated type */
class simplex_interpolator
{
public:

    simplex_interpolator(arraynd<N, T> const & samples) :
        m_samples(samples)
    {
        this->InitBase();
    }

    inline arraynd<N, T> const & GetSamples() const
    {
        return this->m_samples;
    }

    // Single interpolation
    inline T Interp(vec_t<float, N> position) const
    {
        // Computing position in simplex referential
        vec_t<float, N> simplex_position = m_base_inverse * position;

        // Retrieving the closest floor point and decimals
        vec_t<int, N> floor_point;
        vec_t<float, N> decimal_point;

        this->ExtractFloorDecimal(simplex_position, floor_point, decimal_point);

        // Retrieving simplex samples to use
        int sign;

        this->GetReference(floor_point, decimal_point, sign);

        return this->LastInterp(floor_point, decimal_point, sign);
    }

protected:

    inline T LastInterp(vec_t<int, N> const & floor_point,
                        vec_t<float, N> const & decimal_point,
                        int const & sign) const
    {
        T result(0);
        float floor_coeff = 0;
        float divider = 0;

        for (int i = 0 ; i < N ; ++i)
        {
            vec_t<int, N> samples_index = floor_point;
            samples_index[i] = this->Mod(samples_index[i] + sign, i);

            result += decimal_point[i] * this->m_samples[samples_index];
            floor_coeff += decimal_point[i];
            divider += decimal_point[i];
        }

        float sqr_norm = N;

        result += (1 - 2 * floor_coeff / sqr_norm) * this->m_samples[floor_point];
        divider += (1 - 2 * floor_coeff / sqr_norm);

        return result / divider;
    }

    inline int Mod(int value, int index) const
    {
        int const dim = this->m_samples.GetSize()[index];
        int const ret = value % dim;
        return ret >= 0 ? ret : ret + dim;
    }

    inline void GetReference(vec_t<int, N> & floor_point, vec_t<float, N> & decimal_point, int & sign) const
    {
        // Choosing the reference point which determines in which simplex we are working
        // ie. (0, 0, 0, …) and upper or (1, 1, 1, …) and lower

        float cumul = 0;
        for (int i = 0 ; i < N ; ++i)
            cumul += decimal_point[i];

        if (cumul < (sqrt((float)N) / 2))
            sign = 1;
        else
        {
            sign = -1;

            decimal_point = vec_t<float, N>(1) - decimal_point;
            floor_point = floor_point + vec_t<int, N>(1);
        }
    }

    inline void ExtractFloorDecimal(vec_t<float, N> const & simplex_position, vec_t<int, N> & floor_point, vec_t<float, N> & decimal_point) const
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

    inline void InitBase()
    {
        /*  Matrix coordinate transformation to skew simplex referential is done
            by inversing the base matrix M which is written as follows: 

            M = | a b b b … |        M^(-1) = | c d d d … |
                | b a b b … |                 | d c d d … |
                | b b a b … |                 | d d c d … |
                | …         |                 | …         |

            where a and b are computed below ↴
        */

        T b = (1 - sqrt((T)(N+1)))/sqrt((T)(N * N * N));
        T a = b + sqrt((N+1)/(T)N);

        // determinant of matrix M
        T det = a * (a + (N-2) * b) - (b * b) * (N-1);

        T c = (a + (N-2)*b) / det;
        T d = -b / det;

        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                m_base[i][j] = (i == j ? a : b);
                m_base_inverse[i][j] = (i == j ? c : d);
            }
        }
    }

    mat_t<float, N, N> m_base, m_base_inverse;
    arraynd<N, T> m_samples;
};

}
