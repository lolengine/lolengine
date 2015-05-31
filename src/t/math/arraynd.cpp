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

lolunit_declare_fixture(arraynd_test)
{
    lolunit_declare_test(array2d_accessors)
    {
        arraynd<2, int> a;
        a.resize(vec_t<int, 2>(2, 2));

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

    lolunit_declare_test(arraynd_create)
    {
        arraynd<10, int> a;
        arraynd<20, float> b;
        arraynd<30, uint8_t> c;
        arraynd<40, double> d;

        arraynd<3, int> e = { { {1, 2}, {3, 4} }, { {5, 6}, {7, 8} } };

        lolunit_assert_equal(e.size()[0], 2);
        lolunit_assert_equal(e.size()[1], 2);
        lolunit_assert_equal(e.size()[2], 2);

        lolunit_assert_equal(e[0][0][0], 1);
        lolunit_assert_equal(e[1][0][0], 2);
        lolunit_assert_equal(e[0][1][0], 3);
        lolunit_assert_equal(e[1][1][0], 4);
        lolunit_assert_equal(e[0][0][1], 5);
        lolunit_assert_equal(e[1][0][1], 6);
        lolunit_assert_equal(e[0][1][1], 7);
        lolunit_assert_equal(e[1][1][1], 8);

        arraynd<3, int> f = { { {1, 2, 3, 4}, {5, 6, 7} }, { {8, 9}, {10} } };

        lolunit_assert_equal(f.size()[0], 4);
        lolunit_assert_equal(f.size()[1], 2);
        lolunit_assert_equal(f.size()[2], 2);

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

    lolunit_declare_test(arraynd_init)
    {
        int const NDIM = 8;
        vec_t<int, NDIM> size;
        for (int i = 0; i < NDIM; ++i)
            size[i] = 5;

        arraynd<NDIM, uint8_t> a(size);
        memset(a.data(), 0, a.bytes());
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

