//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/unit_test>
#include <lol/narray> // lol::narray
#include <cstring>    // memset

namespace lol
{

lolunit_declare_fixture(narray_test)
{
    lolunit_declare_test(narray_2d_accessors)
    {
        narray<int, 2> a;
        a.resize(vec_t<int, 2>(2, 2));

        // Non-const accessors
        a(0, 0) = 1;
        a(0, 1) = 2;
        a(1, 0) = 3;
        a(1, 1) = 4;
        lolunit_assert_equal(a(0, 0), 1);
        lolunit_assert_equal(a(0, 1), 2);
        lolunit_assert_equal(a(1, 0), 3);
        lolunit_assert_equal(a(1, 1), 4);

        // Const accessors
        narray<int, 2> const &b = a;
        lolunit_assert_equal(b(0, 0), 1);
        lolunit_assert_equal(b(0, 1), 2);
        lolunit_assert_equal(b(1, 0), 3);
        lolunit_assert_equal(b(1, 1), 4);
    }

    lolunit_declare_test(narray_create)
    {
        narray<int, 10> a;
        narray<float, 20> b;
        narray<uint8_t, 30> c;
        narray<double, 30> d;

#if 0
        narray<int, 3> e = { { {1, 2}, {3, 4} }, { {5, 6}, {7, 8} } };

        lolunit_assert_equal(e.sizes()(0), 2);
        lolunit_assert_equal(e.sizes()(1), 2);
        lolunit_assert_equal(e.sizes()(2), 2);

        lolunit_assert_equal(e(0, 0, 0), 1);
        lolunit_assert_equal(e(1, 0, 0), 2);
        lolunit_assert_equal(e(0, 1, 0), 3);
        lolunit_assert_equal(e(1, 1, 0), 4);
        lolunit_assert_equal(e(0, 0, 1), 5);
        lolunit_assert_equal(e(1, 0, 1), 6);
        lolunit_assert_equal(e(0, 1, 1), 7);
        lolunit_assert_equal(e(1, 1, 1), 8);

        narray<int, 3> f = { { {1, 2, 3, 4}, {5, 6, 7} }, { {8, 9}, {10} } };

        lolunit_assert_equal(f.sizes()(0), 4);
        lolunit_assert_equal(f.sizes()(1), 2);
        lolunit_assert_equal(f.sizes()(2), 2);

        lolunit_assert_equal(f(0, 0, 0), 1);
        lolunit_assert_equal(f(1, 0, 0), 2);
        lolunit_assert_equal(f(2, 0, 0), 3);
        lolunit_assert_equal(f(3, 0, 0), 4);
        lolunit_assert_equal(f(0, 1, 0), 5);
        lolunit_assert_equal(f(1, 1, 0), 6);
        lolunit_assert_equal(f(2, 1, 0), 7);
        lolunit_assert_equal(f(3, 1, 0), 0);
        lolunit_assert_equal(f(0, 0, 1), 8);
        lolunit_assert_equal(f(1, 0, 1), 9);
        lolunit_assert_equal(f(2, 0, 1), 0);
        lolunit_assert_equal(f(3, 0, 1), 0);
        lolunit_assert_equal(f(0, 1, 1), 10);
        lolunit_assert_equal(f(1, 1, 1), 0);
        lolunit_assert_equal(f(2, 1, 1), 0);
        lolunit_assert_equal(f(3, 1, 1), 0);
#endif
    }

    lolunit_declare_test(narray_init)
    {
        int const NDIM = 8;
        vec_t<int, NDIM> size(5);

        narray<uint8_t, NDIM> a(size);
        memset(a.data(), 0, a.bytes());
        lolunit_assert_equal(a(2, 3, 2, 0, 1, 4, 0, 1), 0x00);

        vec_t<int, NDIM> v = { 2, 3, 2, 0, 1, 4, 0, 1 };
        lolunit_assert_equal(a(v), 0x00);

        a(2, 3, 2, 0, 1, 4, 0, 1) = 0xcd;
        lolunit_assert_equal(a(2, 3, 2, 0, 1, 4, 0, 1), 0xcd);
        lolunit_assert_equal(a(v), 0xcd);

        narray<uint8_t, NDIM> const &b = a;
        lolunit_assert_equal(b(2, 3, 2, 0, 1, 4, 0, 1), 0xcd);
        lolunit_assert_equal(b(v), 0xcd);
    }
};

} // namespace lol

