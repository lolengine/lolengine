//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>
#include <lol/base/lolunit.h>

namespace lol
{

// Get the L matrix of an LU decomposition
template<typename T, int N>
static mat_t<T, N, N> l_extract(mat_t<T, N, N> const & lu)
{
    auto l = lu;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j <= i; ++j)
            l[i][j] = T(j == i ? 1 : 0);
    return l;
}

// Get the U matrix of an LU decomposition
template<typename T, int N>
static mat_t<T, N, N> u_extract(mat_t<T, N, N> const & lu)
{
    auto u = lu;
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            u[i][j] = T(0);
    return u;
}

lolunit_declare_fixture(matrix_test)
{
    void setup()
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

    lolunit_declare_test(matrix_determinant)
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
            lolunit_assert_doubles_equal(m2[i][j], float(i == j), 1e-5);
    }

    lolunit_declare_test(lu_decomposition_3x3)
    {
        mat3 m(vec3(2, 3, 5),
               vec3(3, 2, 3),
               vec3(9, 5, 7));
        auto lup = lu_decomposition(m);
        auto lu = std::get<0>(lup);
        auto p = std::get<1>(lup);
        auto m2 = l_extract(lu) * u_extract(lu);

        for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 3; ++i)
            lolunit_assert_doubles_equal(m2[i][j], m[i][p[j]], 1e-5);
    }

    lolunit_declare_test(lu_decomposition_4x4_full)
    {
        mat4 m(vec4( 1,  1,  2, -1),
               vec4(-2, -1, -2,  2),
               vec4( 4,  2,  5, -4),
               vec4( 5, -3, -7, -6));
        auto lup = lu_decomposition(m);
        auto lu = std::get<0>(lup);
        auto p = std::get<1>(lup);
        auto m2 = l_extract(lu) * u_extract(lu);

        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_doubles_equal(m2[i][j], m[i][p[j]], 1e-5);
    }

    lolunit_declare_test(lu_decomposition_4x4_sparse)
    {
        mat4 m(vec4(1,  0,  0,  0),
               vec4(0,  0,  1,  0),
               vec4(0, -1,  0,  0),
               vec4(0,  0, -1,  1));
        auto lup = lu_decomposition(m);
        auto lu = std::get<0>(lup);
        auto p = std::get<1>(lup);
        auto m2 = l_extract(lu) * u_extract(lu);

        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_doubles_equal(m2[i][j], m[i][p[j]], 1e-5);
    }

    lolunit_declare_test(l_inverse_3x3)
    {
        mat3 m(vec3(2, 3, 5),
               vec3(3, 2, 3),
               vec3(9, 5, 7));
        auto lu = std::get<0>(lu_decomposition(m));
        auto m2 = l_extract(lu) * l_inverse(lu);

        for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 3; ++i)
            lolunit_assert_doubles_equal(m2[i][j], float(i == j), 1e-5);
    }

    lolunit_declare_test(l_inverse_4x4)
    {
        mat4 m(vec4( 1,  1,  2, -1),
               vec4(-2, -1, -2,  2),
               vec4( 4,  2,  5, -4),
               vec4( 5, -3, -7, -6));
        auto lu = std::get<0>(lu_decomposition(m));
        auto m2 = l_extract(lu) * l_inverse(lu);

        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_doubles_equal(m2[i][j], float(i == j), 1e-5);
    }

    lolunit_declare_test(u_inverse_3x3)
    {
        mat3 m(vec3(2, 3, 5),
               vec3(3, 2, 3),
               vec3(9, 5, 7));
        auto lu = std::get<0>(lu_decomposition(m));
        auto m2 = u_extract(lu) * u_inverse(lu);

        for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 3; ++i)
            lolunit_assert_doubles_equal(m2[i][j], float(i == j), 1e-5);
    }

    lolunit_declare_test(u_inverse_4x4)
    {
        mat4 m(vec4( 1,  1,  2, -1),
               vec4(-2, -1, -2,  2),
               vec4( 4,  2,  5, -4),
               vec4( 5, -3, -7, -6));
        auto lu = std::get<0>(lu_decomposition(m));
        auto m2 = u_extract(lu) * u_inverse(lu);

        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_doubles_equal(m2[i][j], float(i == j), 1e-5);
    }

    lolunit_declare_test(inverse_2x2)
    {
        mat2 m(vec2(4, 3),
               vec2(3, 2));

        // Invert matrix and check that the results are finite
        mat2 m1 = inverse(m);
        for (int j = 0; j < 2; ++j)
        for (int i = 0; i < 2; ++i)
        {
            lolunit_assert_less(m1[i][j], FLT_MAX);
            lolunit_assert_greater(m1[i][j], -FLT_MAX);
        }

        // Multiply with original matrix and check that we get identity
        mat2 m2 = m1 * m;
        for (int j = 0; j < 2; ++j)
        for (int i = 0; i < 2; ++i)
            lolunit_assert_doubles_equal(m2[i][j], float(i == j), 1e-5);
    }

    lolunit_declare_test(inverse_3x3)
    {
        mat3 m(vec3(2, 3, 5),
               vec3(3, 2, 3),
               vec3(9, 5, 7));

        // Invert matrix and check that the results are finite
        mat3 m1 = inverse(m);
        for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 3; ++i)
        {
            lolunit_assert_less(m1[i][j], FLT_MAX);
            lolunit_assert_greater(m1[i][j], -FLT_MAX);
        }

        // Multiply with original matrix and check that we get identity
        mat3 m2 = m1 * m;
        for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 3; ++i)
            lolunit_assert_doubles_equal(m2[i][j], float(i == j), 1e-5);
    }

    lolunit_declare_test(inverse_4x4_full)
    {
        mat4 m(vec4( 1,  1,  2, -1),
               vec4(-2, -1, -2,  2),
               vec4( 4,  2,  5, -4),
               vec4( 5, -3, -7, -6));

        // Invert matrix and check that the results are finite
        mat4 m1 = inverse(m);
        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
        {
            lolunit_assert_less(m1[i][j], FLT_MAX);
            lolunit_assert_greater(m1[i][j], -FLT_MAX);
        }

        // Multiply with original matrix and check that we get identity
        mat4 m2 = m1 * m;
        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_doubles_equal(m2[i][j], float(i == j), 1e-5);
    }

    lolunit_declare_test(inverse_4x4_sparse)
    {
        mat4 m(vec4(1,  0,  0,  0),
               vec4(0,  0,  1,  0),
               vec4(0, -1,  0,  0),
               vec4(0,  0, -1,  1));

        // Invert matrix and check that the results are finite
        mat4 m1 = inverse(m);
        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
        {
            lolunit_assert_less(m1[i][j], FLT_MAX);
            lolunit_assert_greater(m1[i][j], -FLT_MAX);
        }

        // Multiply with original matrix and check that we get identity
        mat4 m2 = m1 * m;
        for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 4; ++i)
            lolunit_assert_doubles_equal(m2[i][j], float(i == j), 1e-5);
    }

    lolunit_declare_test(inverse_9x9)
    {
        // Use double because float is not accurate enough for 9×9 matrices
        lol::mat_t<double, 9, 9> m;

        // Generate 1000 random-valued matrices
        for (int n = 0; n < 1000; ++n)
        {
            for (int j = 0; j < m.count; ++j)
            for (int i = 0; i < m.count; ++i)
                m[i][j] = lol::rand(-1.f, 1.f);

            // Invert matrix and check that the result is correct
            auto m2 = inverse(m) * m;
            for (int j = 0; j < m.count; ++j)
            for (int i = 0; i < m.count; ++i)
                lolunit_assert_doubles_equal(m2[i][j], double(i == j), 1e-10);
        }
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

} // namespace lol

