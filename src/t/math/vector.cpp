//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/main.h>
#include <lolunit.h>

namespace lol
{

LOLUNIT_FIXTURE(VectorTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(VectorEquality)
    {
        vec2 a2(1.0f, 2.0f);
        vec2 b2(0.0f, 2.0f);
        vec2 c2(1.0f, 0.0f);

        LOLUNIT_ASSERT_EQUAL(a2, a2);
        LOLUNIT_ASSERT_NOT_DIFFERENT(a2, a2);

        LOLUNIT_ASSERT_DIFFERENT(a2, b2);
        LOLUNIT_ASSERT_NOT_EQUAL(a2, b2);
        LOLUNIT_ASSERT_DIFFERENT(a2, c2);
        LOLUNIT_ASSERT_NOT_EQUAL(a2, c2);

        vec3 a3(1.0f, 2.0f, 3.0f);
        vec3 b3(0.0f, 2.0f, 3.0f);
        vec3 c3(1.0f, 0.0f, 3.0f);
        vec3 d3(1.0f, 2.0f, 0.0f);

        LOLUNIT_ASSERT_EQUAL(a3, a3);
        LOLUNIT_ASSERT_NOT_DIFFERENT(a3, a3);

        LOLUNIT_ASSERT_DIFFERENT(a3, b3);
        LOLUNIT_ASSERT_NOT_EQUAL(a3, b3);
        LOLUNIT_ASSERT_DIFFERENT(a3, c3);
        LOLUNIT_ASSERT_NOT_EQUAL(a3, c3);
        LOLUNIT_ASSERT_DIFFERENT(a3, d3);
        LOLUNIT_ASSERT_NOT_EQUAL(a3, d3);

        vec4 a4(1.0f, 2.0f, 3.0f, 4.0f);
        vec4 b4(0.0f, 2.0f, 3.0f, 4.0f);
        vec4 c4(1.0f, 0.0f, 3.0f, 4.0f);
        vec4 d4(1.0f, 2.0f, 0.0f, 4.0f);
        vec4 e4(1.0f, 2.0f, 3.0f, 0.0f);

        LOLUNIT_ASSERT_EQUAL(a4, a4);
        LOLUNIT_ASSERT_NOT_DIFFERENT(a4, a4);

        LOLUNIT_ASSERT_DIFFERENT(a4, b4);
        LOLUNIT_ASSERT_NOT_EQUAL(a4, b4);
        LOLUNIT_ASSERT_DIFFERENT(a4, c4);
        LOLUNIT_ASSERT_NOT_EQUAL(a4, c4);
        LOLUNIT_ASSERT_DIFFERENT(a4, d4);
        LOLUNIT_ASSERT_NOT_EQUAL(a4, d4);
        LOLUNIT_ASSERT_DIFFERENT(a4, e4);
        LOLUNIT_ASSERT_NOT_EQUAL(a4, e4);
    }

    LOLUNIT_TEST(VectorInequality)
    {
        vec2 a2(1.0f, 3.0f);
        vec2 b2(0.0f, 0.0f);
        vec2 c2(1.0f, 1.0f);
        vec2 d2(2.0f, 2.0f);
        vec2 e2(3.0f, 3.0f);
        vec2 f2(4.0f, 4.0f);

        LOLUNIT_ASSERT_LEQUAL(a2, a2);
        LOLUNIT_ASSERT_NOT_LESS(a2, a2);

        LOLUNIT_ASSERT_NOT_LEQUAL(a2, b2);
        LOLUNIT_ASSERT_NOT_LESS(a2, b2);
        LOLUNIT_ASSERT_NOT_LEQUAL(a2, c2);
        LOLUNIT_ASSERT_NOT_LESS(a2, c2);
        LOLUNIT_ASSERT_NOT_LEQUAL(a2, d2);
        LOLUNIT_ASSERT_NOT_LESS(a2, d2);
        LOLUNIT_ASSERT_LEQUAL(a2, e2);
        LOLUNIT_ASSERT_NOT_LESS(a2, e2);
        LOLUNIT_ASSERT_LEQUAL(a2, f2);
        LOLUNIT_ASSERT_LESS(a2, f2);
    }

    LOLUNIT_TEST(VectorInit)
    {
        vec2 a { 1.f, 2.f };
        LOLUNIT_ASSERT_EQUAL(1.f, a.x);
        LOLUNIT_ASSERT_EQUAL(2.f, a.y);

        vec3 b { 1.f, 2.f, 3.f };
        LOLUNIT_ASSERT_EQUAL(1.f, b.x);
        LOLUNIT_ASSERT_EQUAL(2.f, b.y);
        LOLUNIT_ASSERT_EQUAL(3.f, b.z);

        vec4 c { 1.f, 2.f, 3.f, 4.f };
        LOLUNIT_ASSERT_EQUAL(1.f, c.x);
        LOLUNIT_ASSERT_EQUAL(2.f, c.y);
        LOLUNIT_ASSERT_EQUAL(3.f, c.z);
        LOLUNIT_ASSERT_EQUAL(4.f, c.w);

        vec_t<float, 10> d { 1.f, 2.f, 3.f, 4.f, 5.f };
        LOLUNIT_ASSERT_EQUAL(1.f, d[0]);
        LOLUNIT_ASSERT_EQUAL(2.f, d[1]);
        LOLUNIT_ASSERT_EQUAL(3.f, d[2]);
        LOLUNIT_ASSERT_EQUAL(4.f, d[3]);
        LOLUNIT_ASSERT_EQUAL(5.f, d[4]);
        LOLUNIT_ASSERT_EQUAL(0.f, d[5]);
        LOLUNIT_ASSERT_EQUAL(0.f, d[6]);
        LOLUNIT_ASSERT_EQUAL(0.f, d[7]);
        LOLUNIT_ASSERT_EQUAL(0.f, d[8]);
        LOLUNIT_ASSERT_EQUAL(0.f, d[9]);
    }

    LOLUNIT_TEST(VectorSwizzle)
    {
        vec3 a(1.0f, 2.0f, 3.0f);
        vec3 b(4.0f, 5.0f, 6.0f);
        vec3 c;

        c = a;
        c.x = b.y;
        LOLUNIT_ASSERT_EQUAL(c.x, 5.0f);
        LOLUNIT_ASSERT_EQUAL(c.y, 2.0f);
        LOLUNIT_ASSERT_EQUAL(c.z, 3.0f);

        c = a.zyx;
        LOLUNIT_ASSERT_EQUAL(c.x, 3.0f);
        LOLUNIT_ASSERT_EQUAL(c.y, 2.0f);
        LOLUNIT_ASSERT_EQUAL(c.z, 1.0f);

#if 0 /* Visual Studio doesn't support these yet. */
        c = a;
        c.xy = b.yz;
        LOLUNIT_ASSERT_EQUAL(c.x, 5.0f);
        LOLUNIT_ASSERT_EQUAL(c.y, 6.0f);
        LOLUNIT_ASSERT_EQUAL(c.z, 3.0f);

        c = a;
        c.xy = b.zz;
        LOLUNIT_ASSERT_EQUAL(c.x, 6.0f);
        LOLUNIT_ASSERT_EQUAL(c.y, 6.0f);
        LOLUNIT_ASSERT_EQUAL(c.z, 3.0f);

        c = a;
        c.xz = b.xy;
        LOLUNIT_ASSERT_EQUAL(c.x, 4.0f);
        LOLUNIT_ASSERT_EQUAL(c.y, 2.0f);
        LOLUNIT_ASSERT_EQUAL(c.z, 5.0f);

        c = a;
        c.xz = b.xz;
        LOLUNIT_ASSERT_EQUAL(c.x, 4.0f);
        LOLUNIT_ASSERT_EQUAL(c.y, 2.0f);
        LOLUNIT_ASSERT_EQUAL(c.z, 6.0f);

        c = a;
        c.xz = c.zy = b.yx;
        LOLUNIT_ASSERT_EQUAL(c.x, 5.0f);
        LOLUNIT_ASSERT_EQUAL(c.y, 4.0f);
        LOLUNIT_ASSERT_EQUAL(c.z, 4.0f);
#endif
    }

    LOLUNIT_TEST(VectorSwizzleMul)
    {
        ivec3 a(1, 2, 3);

        ivec3 b = a * 2;
        LOLUNIT_ASSERT_EQUAL(b.x, 2);
        LOLUNIT_ASSERT_EQUAL(b.y, 4);
        LOLUNIT_ASSERT_EQUAL(b.z, 6);

        ivec3 c = (ivec3)a.zyx * 2;
        LOLUNIT_ASSERT_EQUAL(c.x, 6);
        LOLUNIT_ASSERT_EQUAL(c.y, 4);
        LOLUNIT_ASSERT_EQUAL(c.z, 2);

        ivec3 d = 2 * (ivec3)a.zyx;
        LOLUNIT_ASSERT_EQUAL(d.x, 6);
        LOLUNIT_ASSERT_EQUAL(d.y, 4);
        LOLUNIT_ASSERT_EQUAL(d.z, 2);

        ivec3 e = a.zyx * 2;
        LOLUNIT_ASSERT_EQUAL(e.x, 6);
        LOLUNIT_ASSERT_EQUAL(e.y, 4);
        LOLUNIT_ASSERT_EQUAL(e.z, 2);

        ivec3 f = 2 * a.zyx;
        LOLUNIT_ASSERT_EQUAL(f.x, 6);
        LOLUNIT_ASSERT_EQUAL(f.y, 4);
        LOLUNIT_ASSERT_EQUAL(f.z, 2);
    }

    LOLUNIT_TEST(VectorUnaryMinus)
    {
        vec2 a(1.0f, 3.0f);
        vec2 b(-1.0f, -3.0f);

        LOLUNIT_ASSERT_EQUAL(a, -b);
        LOLUNIT_ASSERT_EQUAL(-a, b);
    }

    LOLUNIT_TEST(CastVector)
    {
        vec2 a1(1.0f, 3.0f);

        vec3 b(a1, 0.0f);
        vec2 a2(b.xy);
        LOLUNIT_ASSERT_EQUAL(b.x, a1.x);
        LOLUNIT_ASSERT_EQUAL(b.y, a1.y);
        LOLUNIT_ASSERT_EQUAL(b.z, 0.0f);
        LOLUNIT_ASSERT_EQUAL(a2, a1);

        vec4 c(a1, 0.0f, 0.0f);
        vec2 a3(c.xy);
        LOLUNIT_ASSERT_EQUAL(c.x, a1.x);
        LOLUNIT_ASSERT_EQUAL(c.y, a1.y);
        LOLUNIT_ASSERT_EQUAL(c.z, 0.0f);
        LOLUNIT_ASSERT_EQUAL(c.w, 0.0f);
        LOLUNIT_ASSERT_EQUAL(a3, a1);
    }

    LOLUNIT_TEST(Orthogonal)
    {
        vec3 a(1.f, 0.f, 0.f);
        vec3 b(0.f, 1.f, 0.f);
        vec3 c(0.f, 0.f, 1.f);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(dot(orthogonal(a), a), 0.f, 1e-6f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(dot(orthogonal(b), b), 0.f, 1e-6f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(dot(orthogonal(c), c), 0.f, 1e-6f);

        /* The length of the orthogonal vector should be at least
         * sqrt(2)/2 times the length of the original vector. */
        LOLUNIT_ASSERT_GREATER(length(orthogonal(a)), 0.7f);
        LOLUNIT_ASSERT_GREATER(length(orthogonal(b)), 0.7f);
        LOLUNIT_ASSERT_GREATER(length(orthogonal(c)), 0.7f);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(dot(orthonormal(a), a), 0.f, 1e-6f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(dot(orthonormal(b), b), 0.f, 1e-6f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(dot(orthonormal(c), c), 0.f, 1e-6f);

        /* The length of the orthonormal vector should be 1. */
        LOLUNIT_ASSERT_DOUBLES_EQUAL(length(orthonormal(a)), 1.f, 1e-6f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(length(orthonormal(b)), 1.f, 1e-6f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(length(orthonormal(c)), 1.f, 1e-6f);
    }

    LOLUNIT_TEST(LargeVectors)
    {
        vec_t<int, 50> v0(0);
        vec_t<int, 50> v1(1);
        vec_t<int, 50> v2(2);
        vec_t<int, 50> v3(3);

        auto va = v0 + v3;
        auto vb = v1 + v2;
        LOLUNIT_ASSERT(va == vb);
    }

#if !LOL_FEATURE_VISUAL_STUDIO_THAT_FUCKING_PIECE_OF_SHIT_COMPILER
    LOLUNIT_TEST(VectorIterator)
    {
        vec4 v4(1.125f, 1.25f, 1.375f, 1.25f);

        for (auto x : v4)
        {
            LOLUNIT_ASSERT_GREATER(x, 1.0f);
            LOLUNIT_ASSERT_LESS(x, 1.5f);
        }

        for (auto &x : v4)
        {
            LOLUNIT_ASSERT_GREATER(x, 1.0f);
            LOLUNIT_ASSERT_LESS(x, 1.5f);
        }

        for (auto x : v4.zywx)
        {
            LOLUNIT_ASSERT_GREATER(x, 1.0f);
            LOLUNIT_ASSERT_LESS(x, 1.5f);
        }

        vec4 const &v4c = v4;
        for (auto x : v4c)
        {
            LOLUNIT_ASSERT_GREATER(x, 1.0f);
            LOLUNIT_ASSERT_LESS(x, 1.5f);
        }

        for (auto &x : v4c)
        {
            LOLUNIT_ASSERT_GREATER(x, 1.0f);
            LOLUNIT_ASSERT_LESS(x, 1.5f);
        }
    }
#endif
};

} /* namespace lol */

