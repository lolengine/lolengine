//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(MatrixTest)
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

    lolunit_declare_test(Determinant)
    {
        float d1, d2;

        d1 = determinant(tri2);
        lolunit_assert_doubles_equal(d1, 2.0f, 1e-5);
        d2 = determinant(inv2);
        lolunit_assert_doubles_equal(d2, -1.0f, 1e-5);

        d1 = determinant(tri3);
        lolunit_assert_doubles_equal(d1, 6.0f, 1e-5);
        d2 = determinant(inv3);
        lolunit_assert_doubles_equal(d2, 1.0f, 1e-5);

        d1 = determinant(tri4);
        lolunit_assert_doubles_equal(d1, 24.0f, 1e-5);
        d2 = determinant(inv4);
        lolunit_assert_doubles_equal(d2, -1.0f, 1e-5);
    }

    lolunit_declare_test(Multiplication)
    {
        mat4 m0 = id4;
        mat4 m1 = id4;
        mat4 m2 = m0 * m1;

        lolunit_assert_equal(m2[0][0], 1.0f);
        lolunit_assert_equal(m2[1][0], 0.0f);
        lolunit_assert_equal(m2[2][0], 0.0f);
        lolunit_assert_equal(m2[3][0], 0.0f);

        lolunit_assert_equal(m2[0][1], 0.0f);
        lolunit_assert_equal(m2[1][1], 1.0f);
        lolunit_assert_equal(m2[2][1], 0.0f);
        lolunit_assert_equal(m2[3][1], 0.0f);

        lolunit_assert_equal(m2[0][2], 0.0f);
        lolunit_assert_equal(m2[1][2], 0.0f);
        lolunit_assert_equal(m2[2][2], 1.0f);
        lolunit_assert_equal(m2[3][2], 0.0f);

        lolunit_assert_equal(m2[0][3], 0.0f);
        lolunit_assert_equal(m2[1][3], 0.0f);
        lolunit_assert_equal(m2[2][3], 0.0f);
        lolunit_assert_equal(m2[3][3], 1.0f);
    }

    lolunit_declare_test(Inverse2x2)
    {
        mat2 m0 = inv2;
        mat2 m1 = inverse(m0);

        mat2 m2 = m0 * m1;

        lolunit_assert_equal(m2[0][0], 1.0f);
        lolunit_assert_equal(m2[1][0], 0.0f);

        lolunit_assert_equal(m2[0][1], 0.0f);
        lolunit_assert_equal(m2[1][1], 1.0f);
    }

    lolunit_declare_test(LUDecomposition3x3)
    {
        mat3 m0 = inv3;

        mat3 L, U;

        lu_decomposition(inv3, L, U);

        mat3 result = L * U;

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (i > j)
                    lolunit_assert_equal(L[i][j], 0);
                else if (i < j)
                    lolunit_assert_equal(U[i][j], 0);
                else
                    lolunit_assert_equal(L[i][j], 1);

                lolunit_assert_equal(result[i][j], inv3[i][j]);
            }
        }
    }

    lolunit_declare_test(LUDecomposition4x4)
    {
        mat4 m0 = inv4;

        mat4 L, U;

        lu_decomposition(inv4, L, U);

        mat4 result = L * U;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (i > j)
                    lolunit_assert_equal(L[i][j], 0);
                else if (i < j)
                    lolunit_assert_equal(U[i][j], 0);
                else
                    lolunit_assert_equal(L[i][j], 1);

                lolunit_assert_equal(result[i][j], inv4[i][j]);
            }
        }
    }

    lolunit_declare_test(LInverse3x3)
    {
        mat3 m0 = inv3;
        mat3 L, U;
        lu_decomposition(inv3, L, U);
        mat3 l_inv = l_inverse(L);

        mat3 identity = l_inv * L;

        for (int i = 0 ; i < 3 ; ++i)
            for (int j = 0 ; j < 3 ; ++j)
                lolunit_assert_doubles_equal(identity[i][j], i == j ? 1 : 0, 1e-5);
    }

    lolunit_declare_test(LInverse4x4)
    {
        mat4 m0 = inv4;
        mat4 L, U;
        lu_decomposition(inv4, L, U);
        mat4 l_inv = l_inverse(L);

        mat4 identity = l_inv * L;

        for (int i = 0 ; i < 4 ; ++i)
            for (int j = 0 ; j < 4 ; ++j)
                lolunit_assert_doubles_equal(identity[i][j], i == j ? 1 : 0, 1e-5);
    }

    lolunit_declare_test(UInverse3x3)
    {
        mat3 m0 = inv3;
        mat3 L, U;
        lu_decomposition(inv3, L, U);
        mat3 u_inv = u_inverse(U);

        mat3 identity = u_inv * U;

        for (int i = 0 ; i < 3 ; ++i)
            for (int j = 0 ; j < 3 ; ++j)
                lolunit_assert_doubles_equal(identity[i][j], i == j ? 1 : 0, 1e-5);
    }

    lolunit_declare_test(UInverse4x4)
    {
        mat4 m0 = inv4;
        mat4 L, U;
        lu_decomposition(inv4, L, U);
        mat4 u_inv = u_inverse(U);

        mat4 identity = u_inv * U;

        for (int i = 0 ; i < 4 ; ++i)
            for (int j = 0 ; j < 4 ; ++j)
                lolunit_assert_doubles_equal(identity[i][j], i == j ? 1 : 0, 1e-5);
    }

    lolunit_declare_test(Inverse3x3)
    {
        mat3 m0 = inv3;
        mat3 m1 = inverse(m0);

        mat3 m2 = m0 * m1;

        lolunit_assert_doubles_equal(m2[0][0], 1.0f, 1e-4);
        lolunit_assert_doubles_equal(m2[1][0], 0.0f, 1e-4);
        lolunit_assert_doubles_equal(m2[2][0], 0.0f, 1e-4);

        lolunit_assert_doubles_equal(m2[0][1], 0.0f, 1e-4);
        lolunit_assert_doubles_equal(m2[1][1], 1.0f, 1e-4);
        lolunit_assert_doubles_equal(m2[2][1], 0.0f, 1e-4);

        lolunit_assert_doubles_equal(m2[0][2], 0.0f, 1e-4);
        lolunit_assert_doubles_equal(m2[1][2], 0.0f, 1e-4);
        lolunit_assert_doubles_equal(m2[2][2], 1.0f, 1e-4);
    }

    lolunit_declare_test(Inverse4x4)
    {
        mat4 m0 = inv4;
        mat4 m1 = inverse(m0);

        mat4 m2 = m0 * m1;

        lolunit_assert_equal(m2[0][0], 1.0f);
        lolunit_assert_equal(m2[1][0], 0.0f);
        lolunit_assert_equal(m2[2][0], 0.0f);
        lolunit_assert_equal(m2[3][0], 0.0f);

        lolunit_assert_equal(m2[0][1], 0.0f);
        lolunit_assert_equal(m2[1][1], 1.0f);
        lolunit_assert_equal(m2[2][1], 0.0f);
        lolunit_assert_equal(m2[3][1], 0.0f);

        lolunit_assert_equal(m2[0][2], 0.0f);
        lolunit_assert_equal(m2[1][2], 0.0f);
        lolunit_assert_equal(m2[2][2], 1.0f);
        lolunit_assert_equal(m2[3][2], 0.0f);

        lolunit_assert_equal(m2[0][3], 0.0f);
        lolunit_assert_equal(m2[1][3], 0.0f);
        lolunit_assert_equal(m2[2][3], 0.0f);
        lolunit_assert_equal(m2[3][3], 1.0f);
    }

    lolunit_declare_test(Kronecker)
    {
        int const COLS1 = 2, ROWS1 = 3;
        int const COLS2 = 5, ROWS2 = 7;

        mat_t<int, COLS1, ROWS1> a;
        mat_t<int, COLS2, ROWS2> b;

        for (int i = 0; i < COLS1; ++i)
            for (int j = 0; j < ROWS1; ++j)
                a[i][j] = (i + 11) * (j + 13);

        for (int i = 0; i < COLS2; ++i)
            for (int j = 0; j < ROWS2; ++j)
                b[i][j] = (i + 17) * (j + 19);

        mat_t<int, COLS1 * COLS2, ROWS1 * ROWS2> m = outer(a, b);

        for (int i1 = 0; i1 < COLS1; ++i1)
        for (int j1 = 0; j1 < ROWS1; ++j1)
        for (int i2 = 0; i2 < COLS2; ++i2)
        for (int j2 = 0; j2 < ROWS2; ++j2)
        {
            int expected = a[i1][j1] * b[i2][j2];
            int actual = m[i1 * COLS2 + i2][j1 * ROWS2 + j2];

            lolunit_assert_equal(actual, expected);
        }
    }

    mat2 tri2, id2, inv2;
    mat3 tri3, id3, inv3;
    mat4 tri4, id4, inv4;
};

} /* namespace lol */

