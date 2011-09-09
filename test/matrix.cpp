//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(MatrixTest)
{
public:
    void SetUp()
    {
        identity = mat4(1.0f);
        triangular = mat4(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                          vec4(7.0f, 2.0f, 0.0f, 0.0f),
                          vec4(1.0f, 5.0f, 3.0f, 0.0f),
                          vec4(8.0f, 9.0f, 2.0f, 4.0f));
        invertible = mat4(vec4( 1.0f,  1.0f,  2.0f, -1.0f),
                          vec4(-2.0f, -1.0f, -2.0f,  2.0f),
                          vec4( 4.0f,  2.0f,  5.0f, -4.0f),
                          vec4( 5.0f, -3.0f, -7.0f, -6.0f));
    }

    void TearDown() {}

    LOLUNIT_TEST(test_vec_eq)
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

    LOLUNIT_TEST(test_vec_lt)
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

    LOLUNIT_TEST(test_vec_unary)
    {
        vec2 a(1.0f, 3.0f);
        vec2 b(-1.0f, -3.0f);

        LOLUNIT_ASSERT_EQUAL(a, -b);
        LOLUNIT_ASSERT_EQUAL(-a, b);
    }

    LOLUNIT_TEST(test_vec_cast)
    {
        vec2 a1(1.0f, 3.0f);

        vec3 b(a1);
        vec2 a2(b);
        LOLUNIT_ASSERT_EQUAL(b.x, a1.x);
        LOLUNIT_ASSERT_EQUAL(b.y, a1.y);
        LOLUNIT_ASSERT_EQUAL(b.z, 0.0f);
        LOLUNIT_ASSERT_EQUAL(a2, a1);

        vec4 c(a1);
        vec2 a3(c);
        LOLUNIT_ASSERT_EQUAL(c.x, a1.x);
        LOLUNIT_ASSERT_EQUAL(c.y, a1.y);
        LOLUNIT_ASSERT_EQUAL(c.z, 0.0f);
        LOLUNIT_ASSERT_EQUAL(c.w, 0.0f);
        LOLUNIT_ASSERT_EQUAL(a3, a1);
    }

    LOLUNIT_TEST(test_mat_det)
    {
        float d1 = triangular.det();
        LOLUNIT_ASSERT_EQUAL(d1, 24.0f);
        float d2 = invertible.det();
        LOLUNIT_ASSERT_EQUAL(d2, -1.0f);
    }

    LOLUNIT_TEST(test_mat_mul)
    {
        mat4 m0 = identity;
        mat4 m1 = identity;
        mat4 m2 = m0 * m1;

        LOLUNIT_ASSERT_EQUAL(m2[0][0], 1.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][0], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[2][0], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[3][0], 0.0f);

        LOLUNIT_ASSERT_EQUAL(m2[0][1], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][1], 1.0f);
        LOLUNIT_ASSERT_EQUAL(m2[2][1], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[3][1], 0.0f);

        LOLUNIT_ASSERT_EQUAL(m2[0][2], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][2], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[2][2], 1.0f);
        LOLUNIT_ASSERT_EQUAL(m2[3][2], 0.0f);

        LOLUNIT_ASSERT_EQUAL(m2[0][3], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][3], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[2][3], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[3][3], 1.0f);
    }

    LOLUNIT_TEST(test_mat_inv)
    {
        mat4 m0 = invertible;
        mat4 m1 = m0.invert();

        mat4 m2 = m0 * m1;

        LOLUNIT_ASSERT_EQUAL(m2[0][0], 1.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][0], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[2][0], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[3][0], 0.0f);

        LOLUNIT_ASSERT_EQUAL(m2[0][1], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][1], 1.0f);
        LOLUNIT_ASSERT_EQUAL(m2[2][1], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[3][1], 0.0f);

        LOLUNIT_ASSERT_EQUAL(m2[0][2], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][2], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[2][2], 1.0f);
        LOLUNIT_ASSERT_EQUAL(m2[3][2], 0.0f);

        LOLUNIT_ASSERT_EQUAL(m2[0][3], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][3], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[2][3], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[3][3], 1.0f);
    }

private:
    mat4 triangular, identity, invertible;
};

} /* namespace lol */

