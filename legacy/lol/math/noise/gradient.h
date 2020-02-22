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
class gradient_provider
{
public:
    gradient_provider(int seed = 0)
      : m_seed(seed)
    {
    }

protected:
    vec_t<float, N> get_gradient(vec_t<int, N> origin) const
    {
        /* Quick shuffle table:
         * strings /dev/urandom | grep . -nm256 | sort -k2 -t: | sed 's|:.*|,|'
         * Then just replace “256” with “0”. */
        static int const shuffle[256] =
        {
            111, 14, 180, 186, 221, 114, 219, 79, 66, 46, 152, 81, 246, 200,
            141, 172, 85, 244, 112, 92, 34, 106, 218, 205, 236, 7, 121, 115,
            109, 131, 10, 96, 188, 148, 17, 107, 94, 182, 235, 163, 143, 63,
            248, 202, 52, 154, 37, 241, 53, 129, 25, 159, 242, 38, 171, 213,
            6, 203, 255, 193, 42, 209, 28, 176, 210, 60, 54, 144, 3, 71, 89,
            116, 12, 237, 67, 216, 252, 178, 174, 164, 98, 234, 32, 26, 175,
            24, 130, 128, 113, 99, 212, 62, 11, 75, 185, 73, 93, 31, 30, 44,
            122, 173, 139, 91, 136, 162, 194, 41, 56, 101, 68, 69, 211, 151,
            97, 55, 83, 33, 50, 119, 156, 149, 208, 157, 253, 247, 161, 133,
            230, 166, 225, 204, 224, 13, 110, 123, 142, 64, 65, 155, 215, 9,
            197, 140, 58, 77, 214, 126, 195, 179, 220, 232, 125, 147, 8, 39,
            187, 27, 217, 100, 134, 199, 88, 206, 231, 250, 74, 2, 135, 120,
            21, 245, 118, 243, 82, 183, 238, 150, 158, 61, 4, 177, 146, 153,
            117, 249, 254, 233, 90, 222, 207, 48, 15, 18, 20, 16, 47, 0, 51,
            165, 138, 127, 169, 72, 1, 201, 145, 191, 192, 239, 49, 19, 160,
            226, 228, 84, 181, 251, 36, 87, 22, 43, 70, 45, 105, 5, 189, 95,
            40, 196, 59, 57, 190, 80, 104, 167, 78, 124, 103, 240, 184, 170,
            137, 29, 23, 223, 108, 102, 86, 198, 227, 35, 229, 76, 168, 132,
        };

        /* Generate 2^(N+2) random vectors, but at least 2^5 (32) and not
         * more than 2^20 (~ 1 million). */
        int const gradient_count = 1 << min(max(N + 2, 5), 20);

        static auto build_gradients = [&]()
        {
            array<vec_t<float, N>> ret;
            for (int k = 0; k < gradient_count; ++k)
            {
                vec_t<float, N> v;
                for (int i = 0; i < N; ++i)
                    v[i] = rand(-1.f, 1.f);
                ret << normalize(v);
            }
            return ret;
        };

        static array<vec_t<float, N>> const gradients = build_gradients();

        int idx = m_seed;
        for (int i = 0; i < N; ++i)
            idx ^= shuffle[(idx + origin[i]) & 255];

        idx &= (gradient_count - 1);
#if 0
        // DEBUG: only output a few gradients
        if (idx > 2)
            return vec_t<float, N>(0);
#endif
        return gradients[idx];
    }

private:
    /* A user-provided random seed. Defaults to zero. */
    int m_seed;
};

}

