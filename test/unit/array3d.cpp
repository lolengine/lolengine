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
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(Array3DTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(Array3DCreate)
    {
        array3d<int> a(ivec3(10, 10, 10));

        a[0][0][0] = 2;
        a[9][0][0] = 4;
        a[0][9][9] = 6;
        a[9][9][9] = 8;

        LOLUNIT_ASSERT_EQUAL(a[0][0][0], 2);
        LOLUNIT_ASSERT_EQUAL(a[9][0][0], 4);
        LOLUNIT_ASSERT_EQUAL(a[0][9][9], 6);
        LOLUNIT_ASSERT_EQUAL(a[9][9][9], 8);

        array3d<int> const &b = a;

        LOLUNIT_ASSERT_EQUAL(b[0][0][0], 2);
        LOLUNIT_ASSERT_EQUAL(b[9][0][0], 4);
        LOLUNIT_ASSERT_EQUAL(b[0][9][9], 6);
        LOLUNIT_ASSERT_EQUAL(b[9][9][9], 8);
    }

    LOLUNIT_TEST(Array3DInit)
    {
        array3d<int> a = { { {  1,  2,  3,  4 },
                             {  5,  6,  7,  8 },
                             {  9,  8,  7,  6 } },
                           { { -1, -2, -3, -4 },
                             { -5, -6, -7, -8 },
                             { -9, -8, -7, -6 } } };

        LOLUNIT_ASSERT_EQUAL(a.GetSize().x, 4);
        LOLUNIT_ASSERT_EQUAL(a.GetSize().y, 3);
        LOLUNIT_ASSERT_EQUAL(a.GetSize().z, 2);

        LOLUNIT_ASSERT_EQUAL(a[0][0][0], 1);
        LOLUNIT_ASSERT_EQUAL(a[1][0][0], 2);
        LOLUNIT_ASSERT_EQUAL(a[2][0][0], 3);
        LOLUNIT_ASSERT_EQUAL(a[3][0][0], 4);

        LOLUNIT_ASSERT_EQUAL(a[0][1][0], 5);
        LOLUNIT_ASSERT_EQUAL(a[1][1][0], 6);
        LOLUNIT_ASSERT_EQUAL(a[2][1][0], 7);
        LOLUNIT_ASSERT_EQUAL(a[3][1][0], 8);

        LOLUNIT_ASSERT_EQUAL(a[0][2][0], 9);
        LOLUNIT_ASSERT_EQUAL(a[1][2][0], 8);
        LOLUNIT_ASSERT_EQUAL(a[2][2][0], 7);
        LOLUNIT_ASSERT_EQUAL(a[3][2][0], 6);

        LOLUNIT_ASSERT_EQUAL(a[0][0][1], -1);
        LOLUNIT_ASSERT_EQUAL(a[1][0][1], -2);
        LOLUNIT_ASSERT_EQUAL(a[2][0][1], -3);
        LOLUNIT_ASSERT_EQUAL(a[3][0][1], -4);

        LOLUNIT_ASSERT_EQUAL(a[0][1][1], -5);
        LOLUNIT_ASSERT_EQUAL(a[1][1][1], -6);
        LOLUNIT_ASSERT_EQUAL(a[2][1][1], -7);
        LOLUNIT_ASSERT_EQUAL(a[3][1][1], -8);

        LOLUNIT_ASSERT_EQUAL(a[0][2][1], -9);
        LOLUNIT_ASSERT_EQUAL(a[1][2][1], -8);
        LOLUNIT_ASSERT_EQUAL(a[2][2][1], -7);
        LOLUNIT_ASSERT_EQUAL(a[3][2][1], -6);
    }
};

} /* namespace lol */

