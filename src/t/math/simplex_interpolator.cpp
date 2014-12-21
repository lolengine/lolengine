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

#include <lol/engine-internal.h>

#include <lol/math/simplex_interpolator.h>

#include <lolunit.h>

namespace lol
{

template<int N>
class test_interpolator : public simplex_interpolator<N>
{
public:

    test_interpolator() :
        simplex_interpolator<N>()
    {
    }

    void DumpMatrix()
    {
        std::cout << std::endl;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                std::cout << this->m_base[i][j] << ", ";
            }
            std::cout << ";";
        }

        std::cout << std::endl;

        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                std::cout << this->m_base_inverse[i][j] << ", ";
            }
            std::cout << ";";
        }

        std::cout << std::endl;
    }

    void DumpMatrix(float result[N][N])
    {
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                result[i][j] = this->m_base[i][j];
            }
        }
    }

    void DumpCheckInverse(float result[N][N])
    {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                result[i][j] = 0;

        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                for (int k = 0; k < N; ++k)
                {
                    result[i][j] += this->m_base[i][k] * this->m_base_inverse[k][j];
                }
            }
        }
    }

    vec_t<int, N> GetIndexOrder(vec_t<float, N> const & decimal_point)
    {
        return simplex_interpolator<N>::GetIndexOrder(decimal_point);
    }
};

lolunit_declare_fixture(SimplexInterpolatorTest)
{
    void SetUp() {}

    void TearDown() {}

    template<int N>
    void check_base_matrix()
    {
        test_interpolator<N> s;

        float result[N][N];
        s.DumpCheckInverse(result);

        // Check base matrix inverse
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                if (i == j)
                    lolunit_assert_doubles_equal(1, result[i][j], 1e-6);
                else
                    lolunit_assert_doubles_equal(0, result[i][j], 1e-6);
            }
        }

        s.DumpMatrix(result);

        // Check base vectors’ norms
        for (int i = 0; i < N; ++i)
        {
            float norm = 0;

            for (int j = 0; j < N; ++j)
            {
                norm += result[i][j] * result[i][j];
            }

            lolunit_assert_doubles_equal(1, norm, 1e-6);
        }

        // Check result of sum of base vectors => must have norm = 1
        float vec_result[N];
        for (int i = 0; i < N; ++i)
        {
            vec_result[i] = 0;
            for (int j = 0; j < N; ++j)
            {
                vec_result[i] += result[i][j];
            }
        }

        float norm = 0;
        for (int i = 0 ; i < N ; ++i)
        {
            norm += vec_result[i] * vec_result[i];
        }
        lolunit_assert_doubles_equal(1, norm, 1e-6);
    }

    lolunit_declare_test(CoordinateMatrixTest)
    {
        check_base_matrix<1>();
        check_base_matrix<2>();
        check_base_matrix<3>();
        check_base_matrix<4>();
        check_base_matrix<5>();
        check_base_matrix<6>();
        check_base_matrix<7>();
        check_base_matrix<8>();
        check_base_matrix<9>();
        check_base_matrix<10>();
    }

    template<int N>
    void check_index_ordering()
    {
        static int gen = 12345678;
        test_interpolator<N> s;

        vec_t<float, N> vec_ref;
        for (int i = 0 ; i < N ; ++i)
            vec_ref[i] = (gen = gen * gen + gen);

        vec_t<int, N> result = s.GetIndexOrder(vec_ref);

        for (int i = 1 ; i < N ; ++i)
            lolunit_assert(vec_ref[result[i]] < vec_ref[result[i-1]]);
    }

    lolunit_declare_test(IndexOrderTest)
    {
        for (int i = 1 ; i < 10 ; ++i)
            check_index_ordering<1>();
        for (int i = 1 ; i < 10 ; ++i)
            check_index_ordering<2>();
        for (int i = 1 ; i < 10 ; ++i)
            check_index_ordering<3>();
        for (int i = 1 ; i < 10 ; ++i)
            check_index_ordering<4>();
        for (int i = 1 ; i < 10 ; ++i)
            check_index_ordering<5>();
        for (int i = 1 ; i < 10 ; ++i)
            check_index_ordering<6>();
        for (int i = 1 ; i < 10 ; ++i)
            check_index_ordering<7>();
        for (int i = 1 ; i < 10 ; ++i)
            check_index_ordering<8>();
        for (int i = 1 ; i < 10 ; ++i)
            check_index_ordering<9>();
        for (int i = 1 ; i < 10 ; ++i)
            check_index_ordering<10>();
    }

    lolunit_declare_test(CheckSampleOrder2)
    {
        static int gen = 12345678;

        arraynd<2, vec_t<float, 2> > gradients(vec_t<ptrdiff_t, 2>({10, 10}));
        for (int i = 0 ; i < 10 ; ++i)
        {
            for (int j = 0 ; j < 10 ; ++j)
            {
                float x1 = (gen = gen * gen + gen) % 255;
                float x2 = (gen = gen * gen + gen) % 255;

                vec_t<float, 2> v = {x1, x2};

                gradients[i][j] = v;
            }
        }

        simplex_interpolator<2> s;
        s.SetGradients(gradients);

        std::cout << std::endl;
        for (int i = -64 ; i < 64 ; ++i)
        {
            for (int j = -64 ; j < 64 ; ++j)
            {
                std::cout << s.Interp(vec_t<float, 2>({i * 0.1f, j * 0.1f})) << ", ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

#if 0
    lolunit_declare_test(FloatGridPoints2D1x1)
    {
        simplex_interpolator<2> s({{1.f}});
        float val;

        val = s.Interp(GridPoint2D(-1, -1));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(0, -1));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(1, -1));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(-1, 0));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(0, 0));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(1, 0));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(-1, 1));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(0, 1));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(1, 1));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
    }

    lolunit_declare_test(VectorGridPoints2D1x1)
    {
        simplex_interpolator<2, vec3> s({{vec3(1.f, 2.f, 3.f)}});

        vec3 val = s.Interp(GridPoint2D(-1, 0));
        lolunit_assert_doubles_equal(1.f, val.x, 1e-5f);
        lolunit_assert_doubles_equal(2.f, val.y, 1e-5f);
        lolunit_assert_doubles_equal(3.f, val.z, 1e-5f);
    }

    lolunit_declare_test(GridPoints2D2x2)
    {
        simplex_interpolator<2> s({{1.f, 1.f}, {1.f, 2.f}});
        float val;

        val = s.Interp(GridPoint2D(0, 0));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(1, 0));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(0, 1));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(1, 1));
        lolunit_assert_doubles_equal(2.f, val, 1e-5f);
    }

    lolunit_declare_test(MoreGridPoints2D2x2)
    {
        simplex_interpolator<2> s({{1.f, 1.f}, {1.f, 2.f}});
        float val;

        val = s.Interp(GridPoint2D(-1, -1));
        lolunit_assert_doubles_equal(2.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(0, -1));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(1, -1));
        lolunit_assert_doubles_equal(2.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(2, -1));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(2, 0));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(2, 1));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(2, 2));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(1, 2));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(0, 2));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(-1, 2));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(-1, 1));
        lolunit_assert_doubles_equal(2.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(-1, 0));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
    }

    lolunit_declare_test(GridPoints2D3x3)
    {
        simplex_interpolator<2> s({{1, 1, 2}, {1, 2, 2}, {2, 2, 2}});
        float val;

        val = s.Interp(GridPoint2D(0, 0));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(1, 0));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(0, 1));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);
        val = s.Interp(GridPoint2D(1, 1));
        lolunit_assert_doubles_equal(2.f, val, 1e-5f);
    }

    lolunit_declare_test(OtherPoints2D3x3)
    {
        simplex_interpolator<2> s({{1, 1, 2}, {1, 2, 2}, {2, 2, 2}});
        float val;

        /* 1/1 triangle edge */
        val = s.Interp(vec2(1.f, lol::sin(F_PI / 3)));
        lolunit_assert_doubles_equal(1.5f, val, 1e-5f);

        /* 1/1/1 triangle interior */
        val = s.Interp(vec2(0.5f, 0.5f));
        lolunit_assert_doubles_equal(1.f, val, 1e-5f);

        /* 1/1/2 triangle interior */
        val = s.Interp(vec2(0.f, 0.5f));
        lolunit_assert(val < 2.f);
        lolunit_assert(val > 1.f);

        /* Another 1/1/2 triangle interior */
        val = s.Interp(vec2(1.f, 0.5f));
        lolunit_assert(val < 2.f);
        lolunit_assert(val > 1.f);
    }

private:
    vec2 GridPoint2D(int i, int j)
    {
        static vec2 vi(1.f, 0.f);
        static vec2 vj(lol::cos(F_PI / 3), lol::sin(F_PI / 3));

        return (float)i * vi + (float)j * vj;
    }
#endif
};

}
