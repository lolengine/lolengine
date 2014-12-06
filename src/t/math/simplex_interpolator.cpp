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

template<int N, typename T = float>
class test_interpolator : public simplex_interpolator<N, T>
{
public:

    test_interpolator(arraynd<N, T> const & samples) :
        simplex_interpolator<N, T>(samples)
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
};

lolunit_declare_fixture(SimplexInterpolatorTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(CompoundVariable)
    {
        test_interpolator<2, real> b({{ real(0) }});
        test_interpolator<2, vec2> c({{ vec2(0) }});
    }

    lolunit_declare_test(CoordinateMatrixTest)
    {
        test_interpolator<2> s2({{1.f}});
        s2.DumpMatrix();

        test_interpolator<3> s3(arraynd<3, float>({{{1.f}}}));
        s3.DumpMatrix();
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
