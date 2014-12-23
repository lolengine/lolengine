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
        vec_t<int, N> origin;
        vec_t<float, N> pos;

        this->ExtractFloorDecimal(simplex_position, origin, pos);

        vec_t<int, N> index_order = this->GetIndexOrder(pos);

        // Resetting decimal point in regular orthonormal coordinates
        pos = m_base * pos;

        return this->LastInterp(origin, pos, index_order);
    }

protected:

    inline float LastInterp(vec_t<int, N> origin,
                            vec_t<float, N> const & pos,
                            vec_t<int, N> const & index_order) const
    {
        // “corner” will traverse the simplex along its edges in
        // orthonormal coordinates
        vec_t<float, N> corner(0);
        float result = 0;

        for (int i = 0; i < N + 1; ++i)
        {
            vec_t<float, N> const delta = pos - corner;
            vec_t<float, N> const &gradient = this->m_gradients[origin];

            /* We use 4/3 because the maximum radius of influence for
             * a given corner is sqrt(3/4). FIXME: check whether this
             * holds for higher dimensions. */
            float dist = 1.0f - 4.f / 3.f * sqlength(delta);
            if (dist > 0)
            {
                dist *= dist;
                result += dist * dist * dot(gradient, delta);
            }

            if (i < N)
            {
                corner += m_base[index_order[i]];
                origin[index_order[i]] = this->Mod(origin[index_order[i]] + 1,
                                                   index_order[i]);
            }
        }

        // FIXME: another paper uses the value 70 for dimension 2, 32 for
        // dimension 3, and 27 for dimension 4; find where this comes from.
        return result * 70.f / 16.f;
    }

    /* For a given position [0…1]^n inside a square/cube/hypercube etc.,
     * find the simplex which contains that position, and return the path
     * from (0,0,…,0) to (1,1,…,1) that describes that simplex. */
    inline vec_t<int, N> GetIndexOrder(vec_t<float, N> const & pos) const
    {
        vec_t<int, N> result;
        for (int i = 0; i < N; ++i)
            result[i] = i;

        for (int i = 0; i < N; ++i)
            for (int j = i + 1; j < N; ++j)
                if (pos[result[i]] < pos[result[j]])
                    std::swap(result[i], result[j]);

        return result;
    }

    inline int Mod(int value, int index) const
    {
        int const dim = this->m_gradients.GetSize()[index];
        int const ret = value % dim;
        return ret >= 0 ? ret : ret + dim;
    }

    /* For a given world position, extract grid coordinates (origin) and
     * the corresponding delta position (pos). */
    inline void ExtractFloorDecimal(vec_t<float, N> const & simplex_position,
                                    vec_t<int, N> & origin,
                                    vec_t<float, N> & pos) const
    {
        // Finding floor point index
        for (int i = 0; i < N; ++i)
            origin[i] = ((int) simplex_position[i]) - (simplex_position[i] < 0 ? 1 : 0);

        // Extracting decimal part from simplex sample
        pos = simplex_position - (vec_t<float, N>)origin;

        // Never exceed the size of the gradients and loop on it
        for (int i = 0; i < N; ++i)
            origin[i] = this->Mod(origin[i], i);
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
