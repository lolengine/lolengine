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

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(SimplexInterpolatorTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(Interp2D9PointsTest)
    {
        simplex_interpolator<2> interpolator;

        interpolator.GetSamples() = arraynd<2, float>({{1, 1, 2}, {1, 2, 2}, {2, 2, 2}});

        lolunit_assert_equal(interpolator.Interp(vec_t<float, 2>{0, 0}), 1);
        lolunit_assert_equal(interpolator.Interp(vec_t<float, 2>{1, 0}), 1);
        lolunit_assert_doubles_equal(interpolator.Interp(vec_t<float, 2>{(float)cos(3.14159/3), (float)sin(3.14159/3)}), 1, 1e-5f);

        lolunit_assert(interpolator.Interp(vec_t<float, 2>{0, 0.5}) > 1);
        lolunit_assert(interpolator.Interp(vec_t<float, 2>{0, 0.5}) < 2);
    }
};

}
