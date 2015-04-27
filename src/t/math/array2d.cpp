//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2014 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(Array2DTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(Array2DCreate)
    {
        array2d<int> a(ivec2(10, 10));

        a[0][0] = 2;
        a[9][0] = 4;
        a[0][9] = 6;
        a[9][9] = 8;

        lolunit_assert_equal(a[0][0], 2);
        lolunit_assert_equal(a[9][0], 4);
        lolunit_assert_equal(a[0][9], 6);
        lolunit_assert_equal(a[9][9], 8);

        array2d<int> const &b = a;

        lolunit_assert_equal(b[0][0], 2);
        lolunit_assert_equal(b[9][0], 4);
        lolunit_assert_equal(b[0][9], 6);
        lolunit_assert_equal(b[9][9], 8);
    }

    lolunit_declare_test(Array2DInit)
    {
        array2d<int> a = { { 1, 2, 3, 4 },
                           { 5, 6, 7, 8 },
                           { 9, 8, 7, 6 } };

        lolunit_assert_equal(a.size().x, 4);
        lolunit_assert_equal(a.size().y, 3);

        lolunit_assert_equal(a[0][0], 1);
        lolunit_assert_equal(a[1][0], 2);
        lolunit_assert_equal(a[2][0], 3);
        lolunit_assert_equal(a[3][0], 4);

        lolunit_assert_equal(a[0][1], 5);
        lolunit_assert_equal(a[1][1], 6);
        lolunit_assert_equal(a[2][1], 7);
        lolunit_assert_equal(a[3][1], 8);

        lolunit_assert_equal(a[0][2], 9);
        lolunit_assert_equal(a[1][2], 8);
        lolunit_assert_equal(a[2][2], 7);
        lolunit_assert_equal(a[3][2], 6);
    }
};

} /* namespace lol */

