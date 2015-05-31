//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(array3d_test)
{
    lolunit_declare_test(array3d_create)
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

    lolunit_declare_test(array3d_init)
    {
        array3d<int> a = { { {  1,  2,  3,  4 },
                             {  5,  6,  7,  8 },
                             {  9,  8,  7,  6 } },
                           { { -1, -2, -3, -4 },
                             { -5, -6, -7, -8 },
                             { -9, -8, -7, -6 } } };

        lolunit_assert_equal(a.size().x, 4);
        lolunit_assert_equal(a.size().y, 3);
        lolunit_assert_equal(a.size().z, 2);

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

