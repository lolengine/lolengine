//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//            © 2013—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//            © 2013—2015 Guillaume Bittoun <guillaume.bittoun@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#include <../legacy/lol/math/noise/gradient.h>

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
        /* Compute the containing hypercube origin */
        vec_t<int, N> origin;
        for (int i = 0; i < N; ++i)
            origin[i] = (int)position[i] - (position[i] < 0);

        vec_t<float, N> delta = position - (vec_t<float, N>)origin;

        /* Apply a smooth step to delta and store it in “t”. */
        vec_t<float, N> t = delta;
        t = ((6.f * t - vec_t<float, N>(15.f))
                  * t + vec_t<float, N>(10.f)) * (t * t * t);
        /* DEBUG: original Perlin noise polynomial */
        //t = (vec_t<float, N>(3.f) - 2.f * t) * t * t;

        /* Premultiply and predivide (1-t)/t and t/(1-t) into “u” and “v”. */
        vec_t<float, N> u, v;
        float multiplier = 1.f;
        for (int bit = 0; bit < N; ++bit)
        {
            /* Avoid divisions by zero near the hypercube boundaries. */
            float f = clamp(t[bit], 0.001f, 0.999f);

            multiplier *= (1.f - f);
            u[bit] = (1.f - f) / f;
            v[bit] = f / (1.f - f);
        }

        float ret = 0.f;

        /* Compute all gradient contributions, for each of the 2^N corners
         * of the hypercube. */
        for (int i = 0; ; ++i)
        {
            /* Accumulate Perlin noise */
            ret += multiplier * dot(delta, this->get_gradient(origin));

            /* Avoid buffer overflow below in origin[bit] */
            if (i + 1 == (1 << N))
                break;

            /* Don’t use the binary pattern for “i” but use its Gray code
             * “j” instead, so we know we only have one component to alter
             * in “origin” and in “delta”. We know which bit was flipped by
             * looking at “k”, the Gray code for the next value of “i”. */
            int j = i ^ (i >> 1);
            int k = (i + 1) ^ ((i + 1) >> 1);

            int bit = 0;
            while ((j ^ k) > (1 << bit))
                ++bit;

            origin[bit] += j > k ? -1 : 1;
            delta[bit] += j > k ? 1.f : -1.f;
            multiplier *= (j > k ? u : v)[bit];
        }

        return sqrt(2.f) * ret;
    }
};

}

