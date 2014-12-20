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

template<int N>
class simplex_interpolator
{
public:

    simplex_interpolator() :
        m_gradients()
    {
        this->InitBase();
    }

    inline void SetGradients(arraynd<N, vec_t<float, N> > const & gradients)
    {
        this->m_gradients = gradients;
    }

    inline arraynd<N, vec_t<float, N> > const & GetGradients() const
    {
        return this->m_gradients;
    }

    // Single interpolation
    inline float Interp(vec_t<float, N> position) const
    {
        // Computing position in simplex referential
        vec_t<float, N> simplex_position = m_base_inverse * position;

        // Retrieving the closest floor point and decimals
        vec_t<int, N> floor_point;
        vec_t<float, N> decimal_point;

        this->ExtractFloorDecimal(simplex_position, floor_point, decimal_point);

        vec_t<int, N> index_order = this->GetIndexOrder(decimal_point);

        // Resetting decimal point in regular orthonormal coordinates
        //decimal_point = m_base * decimal_point;

        return this->LastInterp(floor_point, decimal_point, index_order);
    }

protected:

    inline float LastInterp(vec_t<int, N> & floor_point,
                        vec_t<float, N> const & decimal_point,
                        vec_t<int, N> index_order) const
    {
        vec_t<float, N> point_of_interest(0);
        float result = 0;

        for (int i = 0 ; i < N+1 ; ++i)
        {
            float dist = 0.5 - 0.75f * sqlength(this->m_base * (decimal_point - point_of_interest));
            vec_t<float, N> gradient = this->m_gradients[floor_point];

            if (dist > 0)
                result += dist * dist * dist * dist * dot(gradient, this->m_base * (decimal_point - point_of_interest));

            if (i < N)
            {
                point_of_interest[index_order[i]] += 1;
                floor_point[index_order[i]] = this->Mod(floor_point[index_order[i]] + 1, index_order[i]);
            }
        }

        return result;
    }

    inline vec_t<int, N> GetIndexOrder(vec_t<float, N> const & decimal_point) const
    {
        vec_t<int, N> result;
        for (int i = 0 ; i < N ; ++i)
            result[i] = i;

        for (int i = 0 ; i < N ; ++i)
        {
            for (int j = i + 1 ; j < N ; ++j)
            {
                if (decimal_point[result[i]] < decimal_point[result[j]])
                {
                    // just swapping…
                    result[i] ^= result[j];
                    result[j] ^= result[i];
                    result[i] ^= result[j];
                }
            }
        }

        return result;
    }

    inline int Mod(int value, int index) const
    {
        int const dim = this->m_gradients.GetSize()[index];
        int const ret = value % dim;
        return ret >= 0 ? ret : ret + dim;
    }

    inline void ExtractFloorDecimal(vec_t<float, N> const & simplex_position, vec_t<int, N> & floor_point, vec_t<float, N> & decimal_point) const
    {
        // Finding floor point index
        for (int i = 0 ; i < N ; ++i)
            floor_point[i] = (int) simplex_position[i];

        // Extracting decimal part from simplex sample
        for (int i = 0 ; i < N ; ++i)
            decimal_point[i] = simplex_position[i] - floor_point[i];

        // Never exceed the size of the gradients and loop on it
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

            where a, b, c, d are computed below ↴
        */

        float b = (1.f - lol::sqrt(N + 1.f)) / lol::sqrt((float)N * N * N);
        float a = b + lol::sqrt((N + 1.f) / N);

        // determinant of matrix M
        float det = a * (a + (N - 2) * b) - (b * b) * (N - 1);

        float c = (a + (N - 2) * b) / det;
        float d = -b / det;

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
    arraynd<N, vec_t<float, N> > m_gradients;
};

}
