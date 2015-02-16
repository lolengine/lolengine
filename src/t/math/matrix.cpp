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
        tri2 = mat2(vec2(1.0f, 0.0f),
                    vec2(7.0f, 2.0f));
        inv2 = mat2(vec2(4.0f, 3.0f),
                    vec2(3.0f, 2.0f));

        tri3 = mat3(vec3(1.0f, 0.0f, 0.0f),
                    vec3(7.0f, 2.0f, 0.0f),
                    vec3(1.0f, 5.0f, 3.0f));
        inv3 = mat3(vec3(2.0f, 3.0f, 5.0f),
                    vec3(3.0f, 2.0f, 3.0f),
                    vec3(9.0f, 5.0f, 7.0f));

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

    lolunit_declare_test(multiplication_4x4)
    {
        mat4 m0(1.f);
        mat4 m1(1.f);
        mat4 m2 = m0 * m1;

        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_doubles_equal(m2[i][j], mat4(1.f)[i][j], 1e-5);
    }

    lolunit_declare_test(inverse_2x2)
    {
        mat2 m(vec2(4, 3),
               vec2(3, 2));

        mat2 m1 = inverse(m);
        for (int j = 0; j < 2; ++j)
        for (int i = 0; i < 2; ++i)
            lolunit_assert_equal(m1[i][j], m1[i][j]);

        mat2 m2 = m1 * m;
        for (int j = 0; j < 2; ++j)
        for (int i = 0; i < 2; ++i)
            lolunit_assert_doubles_equal(m2[i][j], mat2(1.f)[i][j], 1e-5);
    }

#if 0 /* XXX: LU decomposition is currently broken */
    lolunit_declare_test(lu_decomposition_3x3)
    {
        mat3 m(vec3(2, 3, 5),
               vec3(3, 2, 3),
               vec3(9, 5, 7));
        mat3 L, U;
        lu_decomposition(m, L, U);
        mat3 m2 = L * U;

        for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 3; ++i)
        {
            lolunit_assert(!isnan(U[i][j]));
            lolunit_assert(!isnan(L[i][j]));

            if (i < j)
                lolunit_assert_doubles_equal(U[i][j], 0.f, 1e-5);
            if (i == j)
                lolunit_assert_doubles_equal(L[i][j], 1.f, 1e-5);
            if (j < i)
                lolunit_assert_doubles_equal(L[i][j], 0.f, 1e-5);

            lolunit_assert_doubles_equal(m2[i][j], m[i][j], 1e-5);
        }
    }

    lolunit_declare_test(lu_decomposition_4x4_full)
    {
        mat4 m(vec4( 1,  1,  2, -1),
               vec4(-2, -1, -2,  2),
               vec4( 4,  2,  5, -4),
               vec4( 5, -3, -7, -6));
        mat4 L, U;
        lu_decomposition(m, L, U);
        mat4 m2 = L * U;

        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
        {
            lolunit_assert(!isnan(U[i][j]));
            lolunit_assert(!isnan(L[i][j]));

            if (i < j)
                lolunit_assert_doubles_equal(U[i][j], 0.f, 1e-5);
            if (i == j)
                lolunit_assert_doubles_equal(L[i][j], 1.f, 1e-5);
            if (j < i)
                lolunit_assert_doubles_equal(L[i][j], 0.f, 1e-5);

            lolunit_assert_doubles_equal(m2[i][j], m[i][j], 1e-5);
        }
    }

    lolunit_declare_test(lu_decomposition_4x4_sparse)
    {
        mat4 m(vec4(1,  0,  0,  0),
               vec4(0,  0,  1,  0),
               vec4(0, -1,  0,  0),
               vec4(0,  0, -1,  1));
        mat4 L, U;
        lu_decomposition(m, L, U);
        mat4 m2 = L * U;

        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
        {
            lolunit_assert(!isnan(U[i][j]));
            lolunit_assert(!isnan(L[i][j]));

            if (i < j)
                lolunit_assert_doubles_equal(U[i][j], 0.f, 1e-5);
            if (i == j)
                lolunit_assert_doubles_equal(L[i][j], 1.f, 1e-5);
            if (j < i)
                lolunit_assert_doubles_equal(L[i][j], 0.f, 1e-5);

            lolunit_assert_doubles_equal(m2[i][j], m[i][j], 1e-5);
        }
    }

    lolunit_declare_test(l_inverse_3x3)
    {
        mat3 m(vec3(2, 3, 5),
               vec3(3, 2, 3),
               vec3(9, 5, 7));
        mat3 L, U;
        lu_decomposition(m, L, U);
        mat3 m1 = l_inverse(L);
        mat3 m2 = m1 * L;

        for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 3; ++i)
            lolunit_assert_doubles_equal(m2[i][j], mat3(1.f)[i][j], 1e-5);
    }

    lolunit_declare_test(l_inverse_4x4)
    {
        mat4 m(vec4( 1,  1,  2, -1),
               vec4(-2, -1, -2,  2),
               vec4( 4,  2,  5, -4),
               vec4( 5, -3, -7, -6));
        mat4 L, U;
        lu_decomposition(m, L, U);
        mat4 m1 = l_inverse(L);
        mat4 m2 = m1 * L;

        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_doubles_equal(m2[i][j], mat4(1.f)[i][j], 1e-5);
    }

    lolunit_declare_test(u_inverse_3x3)
    {
        mat3 m(vec3(2, 3, 5),
               vec3(3, 2, 3),
               vec3(9, 5, 7));
        mat3 L, U;
        lu_decomposition(m, L, U);
        mat3 m1 = u_inverse(U);
        mat3 m2 = m1 * U;

        for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 3; ++i)
            lolunit_assert_doubles_equal(m2[i][j], mat3(1.f)[i][j], 1e-5);
    }

    lolunit_declare_test(u_inverse_4x4)
    {
        mat4 m(vec4( 1,  1,  2, -1),
               vec4(-2, -1, -2,  2),
               vec4( 4,  2,  5, -4),
               vec4( 5, -3, -7, -6));
        mat4 L, U;
        lu_decomposition(m, L, U);
        mat4 m1 = u_inverse(U);
        mat4 m2 = m1 * U;

        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_doubles_equal(m2[i][j], mat4(1.f)[i][j], 1e-5);
    }
#endif

    lolunit_declare_test(inverse_3x3)
    {
        mat3 m(vec3(2, 3, 5),
               vec3(3, 2, 3),
               vec3(9, 5, 7));

        mat3 m1 = inverse(m);
        for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 3; ++i)
            lolunit_assert_equal(m1[i][j], m1[i][j]);

        mat3 m2 = m1 * m;
        for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 3; ++i)
            lolunit_assert_doubles_equal(m2[i][j], mat3(1.f)[i][j], 1e-5);
    }

    lolunit_declare_test(inverse_4x4_full)
    {
        mat4 m(vec4( 1,  1,  2, -1),
               vec4(-2, -1, -2,  2),
               vec4( 4,  2,  5, -4),
               vec4( 5, -3, -7, -6));

        mat4 m1 = inverse(m);
        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_equal(m1[i][j], m1[i][j]);

        mat4 m2 = m1 * m;
        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_doubles_equal(m2[i][j], mat4(1.f)[i][j], 1e-5);
    }

    lolunit_declare_test(inverse_4x4_sparse)
    {
        mat4 m(vec4(1,  0,  0,  0),
               vec4(0,  0,  1,  0),
               vec4(0, -1,  0,  0),
               vec4(0,  0, -1,  1));
        mat4 m1 = inverse(m);
        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_equal(m1[i][j], m1[i][j]);

        mat4 m2 = m1 * m;
        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_doubles_equal(m2[i][j], mat4(1.f)[i][j], 1e-5);
    }

    lolunit_declare_test(kronecker_product)
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

    mat2 tri2, inv2;
    mat3 tri3, inv3;
    mat4 tri4, inv4;
};

} /* namespace lol */

