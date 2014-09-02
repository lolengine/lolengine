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

lolunit_declare_fixture(Array3DTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(Array3DCreate)
    {
        array3d<int> a(ivec3(10, 10, 10));

        a[0][0][0] = 2;
        a[9][0][0] = 4;
        a[0][9][9] = 6;
        a[9][9][9] = 8;

        lolunit_assert_equal(a[0][0][0], 2);
        lolunit_assert_equal(a[9][0][0], 4);
        lolunit_assert_equal(a[0][9][9], 6);
        lolunit_assert_equal(a[9][9][9], 8);

        array3d<int> const &b = a;

        lolunit_assert_equal(b[0][0][0], 2);
        lolunit_assert_equal(b[9][0][0], 4);
        lolunit_assert_equal(b[0][9][9], 6);
        lolunit_assert_equal(b[9][9][9], 8);
    }

    lolunit_declare_test(Array3DInit)
    {
        array3d<int> a = { { {  1,  2,  3,  4 },
                             {  5,  6,  7,  8 },
                             {  9,  8,  7,  6 } },
                           { { -1, -2, -3, -4 },
                             { -5, -6, -7, -8 },
                             { -9, -8, -7, -6 } } };

        lolunit_assert_equal(a.GetSize().x, 4);
        lolunit_assert_equal(a.GetSize().y, 3);
        lolunit_assert_equal(a.GetSize().z, 2);

        lolunit_assert_equal(a[0][0][0], 1);
        lolunit_assert_equal(a[1][0][0], 2);
        lolunit_assert_equal(a[2][0][0], 3);
        lolunit_assert_equal(a[3][0][0], 4);

        lolunit_assert_equal(a[0][1][0], 5);
        lolunit_assert_equal(a[1][1][0], 6);
        lolunit_assert_equal(a[2][1][0], 7);
        lolunit_assert_equal(a[3][1][0], 8);

        lolunit_assert_equal(a[0][2][0], 9);
        lolunit_assert_equal(a[1][2][0], 8);
        lolunit_assert_equal(a[2][2][0], 7);
        lolunit_assert_equal(a[3][2][0], 6);

        lolunit_assert_equal(a[0][0][1], -1);
        lolunit_assert_equal(a[1][0][1], -2);
        lolunit_assert_equal(a[2][0][1], -3);
        lolunit_assert_equal(a[3][0][1], -4);

        lolunit_assert_equal(a[0][1][1], -5);
        lolunit_assert_equal(a[1][1][1], -6);
        lolunit_assert_equal(a[2][1][1], -7);
        lolunit_assert_equal(a[3][1][1], -8);

        lolunit_assert_equal(a[0][2][1], -9);
        lolunit_assert_equal(a[1][2][1], -8);
        lolunit_assert_equal(a[2][2][1], -7);
        lolunit_assert_equal(a[3][2][1], -6);
    }
};

} /* namespace lol */

