//
//  Lol Engine
//
//  Copyright © 2010—2014 Sam Hocevar <sam@hocevar.net>
//            © 2013—2014 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            © 2013—2014 Guillaume Bittoun <guillaume.bittoun@gmail.com>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#include <lol/math/noise/gradient.h>

namespace lol
{

/*
 * Simplex noise in dimension N
 * ----------------------------
 *
 *  The N-dimensional regular hypercube can be split into N! simplices that
 * all have the main diagonal as a shared edge.
 *  - number of simplices: N!
 *  - number of vertices per simplex: N+1
 *  - number of edges: N(N+1)/2
 *  - minimum edge length: 1           (hypercube edges, e.g. [1,0,0,…,0])
 *  - maximum edge length: sqrt(N)     (hypercube diagonal, i.e. [1,1,1,…,1])
 *
 *  We skew the simplicial grid along the main diagonal by a factor f =
 * sqrt(N+1), which means the diagonal of the initial parallelotope has
 * length sqrt(N/(N+1)). The edges of that parallelotope have length
 * sqrt(N/(N+1)), too. A formula for the maximum edge length was found
 * empirically:
 *  - minimum edge length: sqrt(N/(N+1))   (parallelotope edges and diagonal)
 *  - maximum edge length: sqrt(floor((N+1)²/4)/(N+1))
 */

template<int N>
class simplex_noise : public gradient_provider<N>
{
public:
    simplex_noise()
      : gradient_provider<N>()
    {
#if 0
        debugprint();
#endif
    }

    simplex_noise(int seed)
      : gradient_provider<N>(seed)
    {
    }

    /* Evaluate noise at a given point */
    inline float eval(vec_t<float, N> position) const
    {
        // Retrieve the containing hypercube origin and associated decimals
        vec_t<int, N> origin;
        vec_t<float, N> pos;
        get_origin(skew(position), origin, pos);

        return get_noise(origin, pos);
    }

    /* Only for debug purposes: return the gradient vector of the given
     * point’s simplex origin. */
    inline vec_t<float, N> gradient(vec_t<float, N> position) const
    {
        vec_t<int, N> origin;
        vec_t<float, N> pos;
        get_origin(skew(position), origin, pos);

        return get_gradient(origin);
    }

protected:
    inline float get_noise(vec_t<int, N> origin,
                           vec_t<float, N> const & pos) const
    {
        /* For a given position [0…1]^N inside a regular N-hypercube, find
         * the N-simplex which contains that position, and return a path
         * along the hypercube edges from (0,0,…,0) to (1,1,…,1) which
         * uniquely describes that simplex. */
        vec_t<int, N> traversal_order;
        for (int i = 0; i < N; ++i)
            traversal_order[i] = i;

        /* Naïve bubble sort — enough for now since the general complexity
         * of our algorithm is O(N²). Sorting in O(N²) will not hurt more! */
        for (int i = 0; i < N; ++i)
            for (int j = i + 1; j < N; ++j)
                if (pos[traversal_order[i]] < pos[traversal_order[j]])
                    std::swap(traversal_order[i], traversal_order[j]);


        /* Get the position in world coordinates, too */
        vec_t<float, N> world_pos = unskew(pos);

        /* “corner” will traverse the simplex along its edges in world
         * coordinates. */
        vec_t<float, N> world_corner(0.f);
        float result = 0.f, sum = 0.f, special = 0.f;
        UNUSED(sum, special);

        for (int i = 0; i < N + 1; ++i)
        {
#if 1
            /* In “Noise Hardware” (2-17) Perlin uses 0.6 - d².
             *
             * In an errata to “Simplex noise demystified”, Gustavson uses
             * 0.5 - d² instead, saying “else the noise is not continuous at
             * simplex boundaries”.
             * And indeed, the distance between any given simplex vertex and
             * the opposite hyperplane is 1/sqrt(2), so the contribution of
             * that vertex should never be > 0 for points further than
             * 1/sqrt(2). Hence 0.5 - d².
             *
             * I prefer to use 1 - 2d² and compensate for the d⁴ below by
             * dividing the final result by 2⁴, because manipulating values
             * between 0 and 1 is more convenient. */
            float d = 1.0f - 2.f * sqlength(world_pos - world_corner);
#else
            // DEBUG: this is the linear contribution of each vertex
            // in the skewed simplex. Unfortunately it creates artifacts.
            float d = ((i == 0) ? 1.f : pos[traversal_order[i - 1]])
                    - ((i == N) ? 0.f : pos[traversal_order[i]]);
#endif

#if 0
            // DEBUG: identify simplex features:
            //  -4.f: centre (-2.f),
            //  -3.f: r=0.38 sphere of influence (contribution = 1/4)
            //  -2.f: r=0.52 sphere of influence (contribution = 1/24)
            if (d > 0.99f) special = min(special, -4.f);
            if (d > 0.7f && d < 0.72f) special = min(special, -3.f);
            if (d > 0.44f && d < 0.46f) special = min(special, -2.f);
#endif

            if (d > 0)
            {
                // Perlin uses 8d⁴ whereas Gustavson uses d⁴ and a final
                // multiplication factor at the end. Let’s go with
                // Gustavson, it’s a few multiplications less.
                d = d * d * d * d;

                //d = (3.f - 2.f * d) * d * d;
                //d = ((6 * d - 15) * d + 10) * d * d * d;

                result += d * dot(this->get_gradient(origin),
                                  world_pos - world_corner);
                sum += d;
            }

            if (i < N)
            {
                auto axis = vec_t<float, N>::axis(traversal_order[i]);
                world_corner += unskew(axis);
                origin[traversal_order[i]] += 1;
            }
        }

#if 0
        if (special < 0.f)
            return special;
#endif

        return get_scale() * result;
    }

    static inline float get_scale()
    {
        /* FIXME: Gustavson uses the value 70 for dimension 2, 32 for
         * dimension 3, and 27 for dimension 4, and uses non-unit gradients
         * of length sqrt(2), sqrt(2) and sqrt(3), saying “The result is
         * scaled to stay just inside [-1,1]” which honestly is just not
         * true.
         * Experiments show that the scaling factor is remarkably close
         * to 6.7958 for all high dimensions (measured up to 12). */
        return N ==  2 ? 6.2003f
             : N ==  3 ? 6.7297f
             : N ==  4 ? 6.7861f
             : N ==  5 ? 6.7950f
             : N ==  6 ? 6.7958f
             : N ==  7 ? 6.7958f
             : /* 7+ */  6.7958f;
    }

    static inline vec_t<float, N> skew(vec_t<float, N> const &v)
    {
        /* Quoting Perlin in “Hardware Noise” (2-18):
         *   The “skew factor” f should be set to f = sqrt(N+1), so that
         *   the point (1,1,...1) is transformed to the point (f,f,...f). */
        float const sum = dot(v, vec_t<float, N>(1));
        float const f = sqrt(1.f + N);
        return v + vec_t<float, N>(sum * (f - 1) / N);
    }

    static inline vec_t<float, N> unskew(vec_t<float, N> const &v)
    {
        float const sum = dot(v, vec_t<float, N>(1));
        float const f = sqrt(1.f + N);
        return v + vec_t<float, N>(sum * (1 / f - 1) / N);
    }

    /* For a given world position, extract grid coordinates (origin) and
     * the corresponding delta position (pos). */
    inline void get_origin(vec_t<float, N> const & world_position,
                           vec_t<int, N> & origin, vec_t<float, N> & pos) const
    {
        // Finding floor point index
        for (int i = 0; i < N; ++i)
            origin[i] = (int)world_position[i] - (world_position[i] < 0);

        // Extracting decimal part from simplex sample
        pos = world_position - (vec_t<float, N>)origin;
    }

private:
    void debugprint()
    {
        // Print some debug information
        printf("Simplex Noise of Dimension %d\n", N);

        long long int n = 1; for (int i = 1; i <= N; ++i) n *= i;
        printf(" - each hypercube cell has %lld simplices "
               "with %d vertices and %d edges\n", n, N + 1, N * (N + 1) / 2);

        vec_t<float, N> diagonal(1.f);
        printf(" - regular hypercube:\n");
        printf("   · edge length 1\n");
        printf("   · diagonal length %f\n", length(diagonal));
        printf(" - unskewed parallelotope:\n");
        printf("   · edge length %f\n", length(unskew(diagonal)));
        printf("   · diagonal length %f\n", length(unskew(diagonal)));
        printf("   · simplex edge lengths between %f and %f\n",
               sqrt((float)N/(N+1)), sqrt((N+1)*(N+1)/4/(float)(N+1)));

        /* Generate simplex vertices */
        vec_t<float, N> vertices[N + 1];
        vertices[0] = vec_t<float, N>(0.f);
        for (int i = 0; i < N; ++i)
        {
            vertices[i + 1] = vertices[i];
            vertices[i + 1][i] += 1.f;
        }
        for (int i = 0; i < N + 1; ++i)
            vertices[i] = unskew(vertices[i]);

        /* Output information for each vertex */
        for (int i = 0; i < N + 1; ++i)
        {
            printf(" - vertex %d\n", i);

            /* Coordinates for debugging purposes */
#if 0
            printf("   · [");
            for (int k = 0; k < N; ++k)
                printf(" %f", vertices[i][k]);
            printf(" ]\n");
#endif

            /* Analyze edge lengths from that vertex */
#if 0
            float minlength = 1.0f;
            float maxlength = 0.0f;
            for (int j = 0; j < N + 1; ++j)
            {
                if (i == j)
                    continue;

                float l = length(vertices[i] - vertices[j]);
                minlength = min(minlength, l);
                maxlength = max(maxlength, l);
            }
            printf("   · edge lengths between %f and %f\n",
                   minlength, maxlength);
#endif

            /* Experimental calculation of the distance to the opposite
             * hyperplane, by picking random points. Works reasonably
             * well up to dimension 6. After that, we’d need something
             * better such as gradient walk. */
#if 0
            float mindist = 1.0f;
            for (int run = 0; run < 10000000; ++run)
            {
                vec_t<float, N> p(0.f);
                float sum = 0.f;
                for (int j = 0; j < N + 1; ++j)
                {
                    if (i == j)
                        continue;
                    float k = rand(1.f);
                    p += k * vertices[j];
                    sum += k;
                }
                mindist = min(mindist, distance(vertices[i], p / sum));
            }
            printf("   · approx. dist. to opposite hyperplane: %f\n", mindist);
#endif

            /* Find a normal vector to the opposite hyperplane. First, pick
             * any point p(i0) on the hyperplane. We just need i0 != i. Then,
             * build a matrix where each row is p(i0)p(j) for all j != i0.
             * Multiplying this matrix by the normal vectors gives a vector
             * full of zeroes except at position i. So we build a vector
             * full of zeroes except at position i, and multiply it by the
             * matrix inverse. */
#if 1
            int i0 = (i == 0) ? 1 : 0;
            mat_t<float, N, N> m;
            for (int j = 0; j < N; ++j)
            {
                auto v = vertices[j < i0 ? j : j + 1] - vertices[i0];
                for (int k = 0; k < N; ++k)
                    m[k][j] = v[k];
            }
            auto axis = vec_t<float, N>::axis(i < i0 ? i : i - 1);
            auto normal = normalize(inverse(m) * axis);

            /* Find distance from current vertex to the opposite hyperplane.
             * Just use the projection theorem in N dimensions. */
            auto w = vertices[i] - vertices[i0];
            float dist = abs(dot(normal, w));
            printf("   · distance to opposite hyperplane: %f\n", dist);
#endif
        }

        /* Compute some statistics about the noise. TODO: histogram */
#if 0
        vec_t<float, N> input(0.f);
        for (int run = 0; run < 1000000; ++run)
        {
            float t = eval(input);

            input[run % N] = rand(1000.f);
        }
#endif

        /* Try to find max noise value by climbing gradient */
        float minval = 0.f, maxval = 0.f;
        array<vec_t<float, N>> deltas;
        for (int i = 0; i < N; ++i)
        {
            auto v = vec_t<float, N>::axis(i);
            deltas << v << -v;
        }
        for (int run = 0; run < 1000; ++run)
        {
            /* Pick a random vector */
            vec_t<float, N> v;
            for (int i = 0; i < N; ++i)
                v[i] = rand(-100.f, 100.f);
            float t = eval(v);
            float e = 0.1f;
            /* Climb up gradient in all dimensions */
            while (e > 1e-6f)
            {
                int best_delta = -1;
                float best_t2 = t;
                for (int i = 0; i < deltas.count(); ++i)
                {
                    float t2 = eval(v + e * deltas[i]);
                    if (abs(t2) > abs(best_t2))
                    {
                        best_delta = i;
                        best_t2 = t2;
                    }
                }
                if (best_delta == -1)
                    e *= 0.5f;
                else
                {
                    v += e * deltas[best_delta];
                    t = best_t2;
                }
            }
            minval = min(t, minval);
            maxval = max(t, maxval);
        }
        printf(" - noise value min/max: %f %f\n", minval, maxval);
        float newscale = 1.f / max(-minval, maxval);
        if (newscale < 1.f)
             printf(" - could replace scale %f with %f\n",
                    get_scale(), newscale * get_scale());
        else
             printf(" - scale looks OK\n");

        printf("\n");
    }
};

}

