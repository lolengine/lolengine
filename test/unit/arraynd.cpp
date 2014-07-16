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

LOLUNIT_FIXTURE(ArrayNDTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(Array2D)
    {
        arraynd<2, int> a;
        a.SetSize(vec_t<size_t, 2>(1, 2));

        a[0][0] = 1;
        a[0][1] = 2;
        LOLUNIT_ASSERT_EQUAL(a[0][0], 1);
        LOLUNIT_ASSERT_EQUAL(a[0][1], 2);

        arraynd<2, int> const &b = a;
        LOLUNIT_ASSERT_EQUAL(b[0][0], 1);
        LOLUNIT_ASSERT_EQUAL(b[0][1], 2);
    }

    LOLUNIT_TEST(ArrayNDCreate)
    {
        arraynd<10, int> a;
        arraynd<20, float> b;
        arraynd<30, uint8_t> c;
        arraynd<40, double> d;
    }

    LOLUNIT_TEST(ArrayNDInit)
    {
        int const NDIM = 8;
        vec_t<size_t, NDIM> size;
        for (int i = 0; i < NDIM; ++i)
            size[i] = 5;

        arraynd<NDIM, uint8_t> a(size);
        memset(a.Data(), 0, a.Bytes());
        LOLUNIT_ASSERT_EQUAL(a[2][3][2][0][1][4][0][1], 0x00);

        vec_t<int, NDIM> v = { 2, 3, 2, 0, 1, 4, 0, 1 };
        LOLUNIT_ASSERT_EQUAL(a[v], 0x00);

        a[2][3][2][0][1][4][0][1] = 0xcd;
        LOLUNIT_ASSERT_EQUAL(a[2][3][2][0][1][4][0][1], 0xcd);
        LOLUNIT_ASSERT_EQUAL(a[v], 0xcd);

        arraynd<NDIM, uint8_t> const &b = a;
        LOLUNIT_ASSERT_EQUAL(b[2][3][2][0][1][4][0][1], 0xcd);
        LOLUNIT_ASSERT_EQUAL(b[v], 0xcd);
    }
};

} /* namespace lol */

