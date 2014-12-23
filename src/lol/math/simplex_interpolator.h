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

#include <functional>

namespace lol
{

template<int N>
class simplex_interpolator
{
public:
    simplex_interpolator(int seed = 0)
      : m_seed(seed)
    {
        /* Matrix coordinate transformation to skew simplex referential is done
         * by inverting the base matrix M which is written as follows:
         *
         *  M = | a b b b … |        M^(-1) = | c d d d … |
         *      | b a b b … |                 | d c d d … |
         *      | b b a b … |                 | d d c d … |
         *      | …         |                 | …         |
         *
         *  where a, b, c, d are computed below ↴
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
        /* “corner” will traverse the simplex along its edges in
         * orthonormal coordinates. */
        vec_t<float, N> corner(0);
        float result = 0;

        for (int i = 0; i < N + 1; ++i)
        {
            vec_t<float, N> const delta = pos - corner;
            vec_t<float, N> const &gradient = GetGradient(origin);

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
                origin[index_order[i]] += 1;
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

        /* Naïve bubble sort — enough for now */
        for (int i = 0; i < N; ++i)
            for (int j = i + 1; j < N; ++j)
                if (pos[result[i]] < pos[result[j]])
                    std::swap(result[i], result[j]);

        return result;
    }

    inline vec_t<float, N> GetGradient(vec_t<int, N> origin) const
    {
        /* Quick shuffle table:
         * strings /dev/urandom | grep . -nm256 | sort -k2 -t: | sed 's|:.*|,|'
         * Then just replace “256” with “0”. */
        static int const shuffle[256] =
        {
            111, 14, 180, 186, 221, 114, 17, 79, 66, 46, 11, 81, 246, 200, 141,
            172, 85, 244, 112, 92, 34, 106, 218, 205, 236, 7, 121, 115, 109,
            131, 10, 96, 188, 148, 219, 107, 94, 182, 235, 163, 143, 213, 248,
            202, 52, 154, 37, 241, 53, 129, 25, 60, 242, 38, 171, 63, 203, 255,
            193, 6, 42, 209, 28, 176, 210, 159, 54, 144, 3, 71, 89, 116, 12,
            237, 67, 216, 252, 178, 174, 164, 98, 234, 32, 26, 175, 24, 130,
            128, 113, 99, 212, 62, 152, 75, 185, 73, 93, 31, 30, 151, 122, 173,
            139, 91, 136, 162, 194, 208, 56, 101, 68, 69, 211, 44, 97, 55, 83,
            33, 50, 119, 156, 149, 41, 157, 253, 247, 161, 47, 230, 166, 225,
            204, 224, 13, 110, 123, 142, 64, 65, 155, 215, 120, 197, 140, 58,
            77, 214, 126, 195, 179, 220, 232, 125, 147, 8, 39, 187, 27, 217,
            100, 134, 199, 88, 206, 231, 250, 74, 2, 135, 9, 245, 118, 21, 243,
            82, 183, 238, 87, 158, 61, 4, 177, 146, 153, 117, 249, 254, 233,
            90, 222, 207, 48, 15, 18, 20, 239, 133, 0, 165, 138, 127, 169, 72,
            1, 201, 145, 191, 192, 16, 49, 19, 95, 226, 228, 84, 181, 251, 36,
            150, 22, 43, 70, 45, 105, 5, 189, 160, 196, 40, 59, 57, 190, 80,
            104, 167, 78, 124, 103, 240, 184, 170, 137, 29, 23, 223, 108, 102,
            86, 198, 227, 35, 229, 76, 168, 132, 51,
        };

        /* 16 random vectors; this should be enough for small dimensions */
        auto v = [&]()
        {
            vec_t<float, N> ret;
            for (int i = 0; i < N; ++i)
                ret[i] = rand(-1.f, 1.f);
            return normalize(ret);
        };

        static vec_t<float, N> const gradients[16] =
        {
            v(), v(), v(), v(), v(), v(), v(), v(),
            v(), v(), v(), v(), v(), v(), v(), v(),
        };

        int idx = m_seed;
        for (int i = 0; i < N; ++i)
            idx = shuffle[(idx + origin[i]) & 255];

        return gradients[(idx ^ (idx >> 4)) & 15];
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
    }

    mat_t<float, N, N> m_base, m_base_inverse;
    int m_seed;
};

}

