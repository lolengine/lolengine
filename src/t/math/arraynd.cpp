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

#include <iostream>

namespace lol
{

lolunit_declare_fixture(ArrayNDTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(Array2D)
    {
        arraynd<2, int> a;
        a.SetSize(vec_t<int, 2>(2, 2));

        /* Non-const accessors */
        a[0][0] = 1;
        a[0][1] = 2;
        a[1][0] = 3;
        a[1][1] = 4;
        lolunit_assert_equal(a[0][0], 1);
        lolunit_assert_equal(a[0][1], 2);
        lolunit_assert_equal(a[1][0], 3);
        lolunit_assert_equal(a[1][1], 4);

        /* Const accessors */
        arraynd<2, int> const &b = a;
        lolunit_assert_equal(b[0][0], 1);
        lolunit_assert_equal(b[0][1], 2);
        lolunit_assert_equal(b[1][0], 3);
        lolunit_assert_equal(b[1][1], 4);
    }

    lolunit_declare_test(ArrayNDCreate)
    {
        arraynd<10, int> a;
        arraynd<20, float> b;
        arraynd<30, uint8_t> c;
        arraynd<40, double> d;

        arraynd<3, int> e = { { {1, 2}, {3, 4} }, { {5, 6}, {7, 8} } };

        lolunit_assert_equal(e.GetSize()[0], 2);
        lolunit_assert_equal(e.GetSize()[1], 2);
        lolunit_assert_equal(e.GetSize()[2], 2);

        lolunit_assert_equal(e[0][0][0], 1);
        lolunit_assert_equal(e[1][0][0], 2);
        lolunit_assert_equal(e[0][1][0], 3);
        lolunit_assert_equal(e[1][1][0], 4);
        lolunit_assert_equal(e[0][0][1], 5);
        lolunit_assert_equal(e[1][0][1], 6);
        lolunit_assert_equal(e[0][1][1], 7);
        lolunit_assert_equal(e[1][1][1], 8);

        arraynd<3, int> f = { { {1, 2, 3, 4}, {5, 6, 7} }, { {8, 9}, {10} } };

        lolunit_assert_equal(f.GetSize()[0], 4);
        lolunit_assert_equal(f.GetSize()[1], 2);
        lolunit_assert_equal(f.GetSize()[2], 2);

        lolunit_assert_equal(f[0][0][0], 1);
        lolunit_assert_equal(f[1][0][0], 2);
        lolunit_assert_equal(f[2][0][0], 3);
        lolunit_assert_equal(f[3][0][0], 4);
        lolunit_assert_equal(f[0][1][0], 5);
        lolunit_assert_equal(f[1][1][0], 6);
        lolunit_assert_equal(f[2][1][0], 7);
        lolunit_assert_equal(f[3][1][0], 0);
        lolunit_assert_equal(f[0][0][1], 8);
        lolunit_assert_equal(f[1][0][1], 9);
        lolunit_assert_equal(f[2][0][1], 0);
        lolunit_assert_equal(f[3][0][1], 0);
        lolunit_assert_equal(f[0][1][1], 10);
        lolunit_assert_equal(f[1][1][1], 0);
        lolunit_assert_equal(f[2][1][1], 0);
        lolunit_assert_equal(f[3][1][1], 0);
    }

    lolunit_declare_test(ArrayNDInit)
    {
        int const NDIM = 8;
        vec_t<int, NDIM> size;
        for (int i = 0; i < NDIM; ++i)
            size[i] = 5;

        arraynd<NDIM, uint8_t> a(size);
        memset(a.Data(), 0, a.Bytes());
        lolunit_assert_equal(a[2][3][2][0][1][4][0][1], 0x00);

        vec_t<int, NDIM> v = { 2, 3, 2, 0, 1, 4, 0, 1 };
        lolunit_assert_equal(a[v], 0x00);

        a[2][3][2][0][1][4][0][1] = 0xcd;
        lolunit_assert_equal(a[2][3][2][0][1][4][0][1], 0xcd);
        lolunit_assert_equal(a[v], 0xcd);

        arraynd<NDIM, uint8_t> const &b = a;
        lolunit_assert_equal(b[2][3][2][0][1][4][0][1], 0xcd);
        lolunit_assert_equal(b[v], 0xcd);
    }
};

} /* namespace lol */

