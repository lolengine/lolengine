//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(vector_test)
{
    lolunit_declare_test(vector_equality)
    {
        vec2 a2(1.0f, 2.0f);
        vec2 b2(0.0f, 2.0f);
        vec2 c2(1.0f, 0.0f);

        lolunit_assert_equal(a2, a2);
        lolunit_refute_different(a2, a2);

        lolunit_assert_different(a2, b2);
        lolunit_refute_equal(a2, b2);
        lolunit_assert_different(a2, c2);
        lolunit_refute_equal(a2, c2);

        vec3 a3(1.0f, 2.0f, 3.0f);
        vec3 b3(0.0f, 2.0f, 3.0f);
        vec3 c3(1.0f, 0.0f, 3.0f);
        vec3 d3(1.0f, 2.0f, 0.0f);

        lolunit_assert_equal(a3, a3);
        lolunit_refute_different(a3, a3);

        lolunit_assert_different(a3, b3);
        lolunit_refute_equal(a3, b3);
        lolunit_assert_different(a3, c3);
        lolunit_refute_equal(a3, c3);
        lolunit_assert_different(a3, d3);
        lolunit_refute_equal(a3, d3);

        vec4 a4(1.0f, 2.0f, 3.0f, 4.0f);
        vec4 b4(0.0f, 2.0f, 3.0f, 4.0f);
        vec4 c4(1.0f, 0.0f, 3.0f, 4.0f);
        vec4 d4(1.0f, 2.0f, 0.0f, 4.0f);
        vec4 e4(1.0f, 2.0f, 3.0f, 0.0f);

        lolunit_assert_equal(a4, a4);
        lolunit_refute_different(a4, a4);

        lolunit_assert_different(a4, b4);
        lolunit_refute_equal(a4, b4);
        lolunit_assert_different(a4, c4);
        lolunit_refute_equal(a4, c4);
        lolunit_assert_different(a4, d4);
        lolunit_refute_equal(a4, d4);
        lolunit_assert_different(a4, e4);
        lolunit_refute_equal(a4, e4);
    }

    lolunit_declare_test(vector_inequality)
    {
        vec2 a2(1.0f, 3.0f);
        vec2 b2(0.0f, 0.0f);
        vec2 c2(1.0f, 1.0f);
        vec2 d2(2.0f, 2.0f);
        vec2 e2(3.0f, 3.0f);
        vec2 f2(4.0f, 4.0f);

        lolunit_assert_lequal(a2, a2);
        lolunit_refute_less(a2, a2);

        lolunit_refute_lequal(a2, b2);
        lolunit_refute_less(a2, b2);
        lolunit_refute_lequal(a2, c2);
        lolunit_refute_less(a2, c2);
        lolunit_refute_lequal(a2, d2);
        lolunit_refute_less(a2, d2);
        lolunit_assert_lequal(a2, e2);
        lolunit_refute_less(a2, e2);
        lolunit_assert_lequal(a2, f2);
        lolunit_assert_less(a2, f2);
    }

    lolunit_declare_test(vector_init)
    {
        vec2 a { 1.f, 2.f };
        lolunit_assert_equal(1.f, a.x);
        lolunit_assert_equal(2.f, a.y);

        vec3 b { 1.f, 2.f, 3.f };
        lolunit_assert_equal(1.f, b.x);
        lolunit_assert_equal(2.f, b.y);
        lolunit_assert_equal(3.f, b.z);

        vec4 c { 1.f, 2.f, 3.f, 4.f };
        lolunit_assert_equal(1.f, c.x);
        lolunit_assert_equal(2.f, c.y);
        lolunit_assert_equal(3.f, c.z);
        lolunit_assert_equal(4.f, c.w);

        vec_t<float, 10> d { 1.f, 2.f, 3.f, 4.f, 5.f };
        lolunit_assert_equal(1.f, d[0]);
        lolunit_assert_equal(2.f, d[1]);
        lolunit_assert_equal(3.f, d[2]);
        lolunit_assert_equal(4.f, d[3]);
        lolunit_assert_equal(5.f, d[4]);
        lolunit_assert_equal(0.f, d[5]);
        lolunit_assert_equal(0.f, d[6]);
        lolunit_assert_equal(0.f, d[7]);
        lolunit_assert_equal(0.f, d[8]);
        lolunit_assert_equal(0.f, d[9]);
    }

    lolunit_declare_test(vector_swizzle)
    {
        vec3 a(1.0f, 2.0f, 3.0f);
        vec3 b(4.0f, 5.0f, 6.0f);
        vec3 c;

        c = a;
        c.x = b.y;
        lolunit_assert_equal(c.x, 5.0f);
        lolunit_assert_equal(c.y, 2.0f);
        lolunit_assert_equal(c.z, 3.0f);

        c = a.zyx;
        lolunit_assert_equal(c.x, 3.0f);
        lolunit_assert_equal(c.y, 2.0f);
        lolunit_assert_equal(c.z, 1.0f);

#if 0 /* Visual Studio doesn't support these yet. */
        c = a;
        c.xy = b.yz;
        lolunit_assert_equal(c.x, 5.0f);
        lolunit_assert_equal(c.y, 6.0f);
        lolunit_assert_equal(c.z, 3.0f);

        c = a;
        c.xy = b.zz;
        lolunit_assert_equal(c.x, 6.0f);
        lolunit_assert_equal(c.y, 6.0f);
        lolunit_assert_equal(c.z, 3.0f);

        c = a;
        c.xz = b.xy;
        lolunit_assert_equal(c.x, 4.0f);
        lolunit_assert_equal(c.y, 2.0f);
        lolunit_assert_equal(c.z, 5.0f);

        c = a;
        c.xz = b.xz;
        lolunit_assert_equal(c.x, 4.0f);
        lolunit_assert_equal(c.y, 2.0f);
        lolunit_assert_equal(c.z, 6.0f);

        c = a;
        c.xz = c.zy = b.yx;
        lolunit_assert_equal(c.x, 5.0f);
        lolunit_assert_equal(c.y, 4.0f);
        lolunit_assert_equal(c.z, 4.0f);
#endif
    }

    lolunit_declare_test(vector_swizzle_mul)
    {
        ivec3 a(1, 2, 3);

        ivec3 b = a * 2;
        lolunit_assert_equal(b.x, 2);
        lolunit_assert_equal(b.y, 4);
        lolunit_assert_equal(b.z, 6);

        ivec3 c = (ivec3)a.zyx * 2;
        lolunit_assert_equal(c.x, 6);
        lolunit_assert_equal(c.y, 4);
        lolunit_assert_equal(c.z, 2);

        ivec3 d = 2 * (ivec3)a.zyx;
        lolunit_assert_equal(d.x, 6);
        lolunit_assert_equal(d.y, 4);
        lolunit_assert_equal(d.z, 2);

        ivec3 e = a.zyx * 2;
        lolunit_assert_equal(e.x, 6);
        lolunit_assert_equal(e.y, 4);
        lolunit_assert_equal(e.z, 2);

        ivec3 f = 2 * a.zyx;
        lolunit_assert_equal(f.x, 6);
        lolunit_assert_equal(f.y, 4);
        lolunit_assert_equal(f.z, 2);
    }

    lolunit_declare_test(vector_unary_minus)
    {
        vec2 a(1.0f, 3.0f);
        vec2 b(-1.0f, -3.0f);

        lolunit_assert_equal(a, -b);
        lolunit_assert_equal(-a, b);
    }

    lolunit_declare_test(cast_vector)
    {
        vec2 a1(1.0f, 3.0f);

        vec3 b(a1, 0.0f);
        vec2 a2(b.xy);
        lolunit_assert_equal(b.x, a1.x);
        lolunit_assert_equal(b.y, a1.y);
        lolunit_assert_equal(b.z, 0.0f);
        lolunit_assert_equal(a2, a1);

        vec4 c(a1, 0.0f, 0.0f);
        vec2 a3(c.xy);
        lolunit_assert_equal(c.x, a1.x);
        lolunit_assert_equal(c.y, a1.y);
        lolunit_assert_equal(c.z, 0.0f);
        lolunit_assert_equal(c.w, 0.0f);
        lolunit_assert_equal(a3, a1);
    }

    lolunit_declare_test(vector_orthogonal)
    {
        vec3 a(1.f, 0.f, 0.f);
        vec3 b(0.f, 1.f, 0.f);
        vec3 c(0.f, 0.f, 1.f);

        lolunit_assert_doubles_equal(dot(orthogonal(a), a), 0.f, 1e-6f);
        lolunit_assert_doubles_equal(dot(orthogonal(b), b), 0.f, 1e-6f);
        lolunit_assert_doubles_equal(dot(orthogonal(c), c), 0.f, 1e-6f);

        /* The length of the orthogonal vector should be at least
         * sqrt(2)/2 times the length of the original vector. */
        lolunit_assert_greater(length(orthogonal(a)), 0.7f);
        lolunit_assert_greater(length(orthogonal(b)), 0.7f);
        lolunit_assert_greater(length(orthogonal(c)), 0.7f);

        lolunit_assert_doubles_equal(dot(orthonormal(a), a), 0.f, 1e-6f);
        lolunit_assert_doubles_equal(dot(orthonormal(b), b), 0.f, 1e-6f);
        lolunit_assert_doubles_equal(dot(orthonormal(c), c), 0.f, 1e-6f);

        /* The length of the orthonormal vector should be 1. */
        lolunit_assert_doubles_equal(length(orthonormal(a)), 1.f, 1e-6f);
        lolunit_assert_doubles_equal(length(orthonormal(b)), 1.f, 1e-6f);
        lolunit_assert_doubles_equal(length(orthonormal(c)), 1.f, 1e-6f);
    }

    lolunit_declare_test(large_vectors)
    {
        vec_t<int, 50> v0(0);
        vec_t<int, 50> v1(1);
        vec_t<int, 50> v2(2);
        vec_t<int, 50> v3(3);

        auto va = v0 + v3;
        auto vb = v1 + v2;
        lolunit_assert(va == vb);
    }

#if !LOL_FEATURE_VISUAL_STUDIO_THAT_FUCKING_PIECE_OF_SHIT_COMPILER
    lolunit_declare_test(vector_iterator)
    {
        vec4 v4(1.125f, 1.25f, 1.375f, 1.25f);

        for (auto x : v4)
        {
            lolunit_assert_greater(x, 1.0f);
            lolunit_assert_less(x, 1.5f);
        }

        for (auto &x : v4)
        {
            lolunit_assert_greater(x, 1.0f);
            lolunit_assert_less(x, 1.5f);
        }

        for (auto x : v4.zywx)
        {
            lolunit_assert_greater(x, 1.0f);
            lolunit_assert_less(x, 1.5f);
        }

        vec4 const &v4c = v4;
        for (auto x : v4c)
        {
            lolunit_assert_greater(x, 1.0f);
            lolunit_assert_less(x, 1.5f);
        }

        for (auto &x : v4c)
        {
            lolunit_assert_greater(x, 1.0f);
            lolunit_assert_less(x, 1.5f);
        }
    }
#endif
};

} /* namespace lol */

