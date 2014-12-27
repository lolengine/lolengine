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

#include <lol/math/noise/gradient.h>

namespace lol
{

template<int N>
class perlin_noise : public gradient_provider<N>
{
public:
    perlin_noise()
      : gradient_provider<N>()
    {
    }

    perlin_noise(int seed)
      : gradient_provider<N>(seed)
    {
    }

    /* Evaluate noise at a given point */
    inline float eval(vec_t<float, N> position) const
    {
        int const cells = 1 << N;

        /* Retrieve the containing hypercube origin */
        vec_t<int, N> origin;
        for (int i = 0; i < N; ++i)
            origin[i] = (int)position[i] - (position[i] < 0);

        vec_t<float, N> delta = position - (vec_t<float, N>)origin;

        vec_t<float, N> t = delta;
        /* DEBUG: original Perlin noise polynomial */
        //t = (vec_t<float, N>(3.f) - 2.f * t) * t * t;
        /* Improved polynomial (with null second derivative) */
        t = ((6.f * t - vec_t<float, N>(15.f))
                  * t + vec_t<float, N>(10.f)) * (t * t * t);

        /* Compute all gradient contributions */
        array<float> values;
        values.Resize(cells);
        for (int i = 0; i < cells; ++i)
        {
            /* FIXME: find another way to travel the hypercube that
             * causes fewer bit flips, and compute the return value
             * directly without allocating an array value. */
            vec_t<float, N> v = delta;
            vec_t<int, N> corner = origin;
            for (int bit = 0; bit < N; ++bit)
                if ((1 << bit) & i)
                {
                    ++corner[bit];
                    v[bit] = v[bit] - 1.f;
                }

            values[i] = dot(v, this->get_gradient(corner));
        }

        /* Interpolate all contributions together */
        for (int bit = N; bit--; )
        {
            for (int i = 0; i < (1 << bit); ++i)
                values[i] = (1.f - t[bit]) * values[i]
                                  + t[bit] * values[i + (1 << bit)];
        }

        return values[0];
    }
};

}

