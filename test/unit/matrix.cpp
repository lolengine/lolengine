//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/main.h>
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(MatrixTest)
{
    void SetUp()
    {
        id2 = mat2(1.0f);
        tri2 = mat2(vec2(1.0f, 0.0f),
                    vec2(7.0f, 2.0f));
        inv2 = mat2(vec2(4.0f, 3.0f),
                    vec2(3.0f, 2.0f));

        id3 = mat3(1.0f);
        tri3 = mat3(vec3(1.0f, 0.0f, 0.0f),
                    vec3(7.0f, 2.0f, 0.0f),
                    vec3(1.0f, 5.0f, 3.0f));
        inv3 = mat3(vec3(2.0f, 3.0f, 5.0f),
                    vec3(3.0f, 2.0f, 3.0f),
                    vec3(9.0f, 5.0f, 7.0f));

        id4 = mat4(1.0f);
        tri4 = mat4(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                    vec4(7.0f, 2.0f, 0.0f, 0.0f),
                    vec4(1.0f, 5.0f, 3.0f, 0.0f),
                    vec4(8.0f, 9.0f, 2.0f, 4.0f));
        inv4 = mat4(vec4( 1.0f,  1.0f,  2.0f, -1.0f),
                    vec4(-2.0f, -1.0f, -2.0f,  2.0f),
                    vec4( 4.0f,  2.0f,  5.0f, -4.0f),
                    vec4( 5.0f, -3.0f, -7.0f, -6.0f));
    }

    void TearDown() {}

    LOLUNIT_TEST(Determinant)
    {
        float d1, d2;

        d1 = determinant(tri2);
        LOLUNIT_ASSERT_EQUAL(d1, 2.0f);
        d2 = determinant(inv2);
        LOLUNIT_ASSERT_EQUAL(d2, -1.0f);

        d1 = determinant(tri3);
        LOLUNIT_ASSERT_EQUAL(d1, 6.0f);
        d2 = determinant(inv3);
        LOLUNIT_ASSERT_EQUAL(d2, 1.0f);

        d1 = determinant(tri4);
        LOLUNIT_ASSERT_EQUAL(d1, 24.0f);
        d2 = determinant(inv4);
        LOLUNIT_ASSERT_EQUAL(d2, -1.0f);
    }

    LOLUNIT_TEST(Multiplication)
    {
        mat4 m0 = id4;
        mat4 m1 = id4;
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

    LOLUNIT_TEST(Inverse2x2)
    {
        mat2 m0 = inv2;
        mat2 m1 = inverse(m0);

        mat2 m2 = m0 * m1;

        LOLUNIT_ASSERT_EQUAL(m2[0][0], 1.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][0], 0.0f);

        LOLUNIT_ASSERT_EQUAL(m2[0][1], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][1], 1.0f);
    }

    LOLUNIT_TEST(Inverse3x3)
    {
        mat3 m0 = inv3;
        mat3 m1 = inverse(m0);

        mat3 m2 = m0 * m1;

        LOLUNIT_ASSERT_EQUAL(m2[0][0], 1.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][0], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[2][0], 0.0f);

        LOLUNIT_ASSERT_EQUAL(m2[0][1], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][1], 1.0f);
        LOLUNIT_ASSERT_EQUAL(m2[2][1], 0.0f);

        LOLUNIT_ASSERT_EQUAL(m2[0][2], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[1][2], 0.0f);
        LOLUNIT_ASSERT_EQUAL(m2[2][2], 1.0f);
    }

    LOLUNIT_TEST(Inverse4x4)
    {
        mat4 m0 = inv4;
        mat4 m1 = inverse(m0);

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

    mat2 tri2, id2, inv2;
    mat3 tri3, id3, inv3;
    mat4 tri4, id4, inv4;
};

} /* namespace lol */

