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
        // pos = m_base * pos;

        return this->LastInterp(origin, pos, index_order);
    }

protected:

    inline float LastInterp(vec_t<int, N> & origin,
                            vec_t<float, N> const & pos,
                            vec_t<int, N> index_order) const
    {
        // “corner” will traverse the simplex along its edges
        vec_t<float, N> corner(0);
        float result = 0;

        for (int i = 0; i < N + 1; ++i)
        {
            float dist = 0.5f - 0.75f * sqlength(this->m_base * (pos - corner));
            vec_t<float, N> const &gradient = this->m_gradients[origin];

            if (dist > 0)
                result += dist * dist * dist * dist * dot(gradient, this->m_base * (pos - corner));

            if (i < N)
            {
                corner[index_order[i]] += 1;
                origin[index_order[i]] = this->Mod(origin[index_order[i]] + 1, index_order[i]);
            }
        }

        // Approximative scaling factor “100” seems to work well
        // ie. gives a max value of 1 (a bit more in fact) for normed gradients
        return result * 100;
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
