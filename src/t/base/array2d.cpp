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
#include <lol/narray> // lol::array2d

namespace lol
{

lolunit_declare_fixture(array2d_test)
{
    lolunit_declare_test(array2d_create)
    {
        array2d<int> a(10, 10);

        a(0, 0) = 2;
        a(9, 0) = 4;
        a(0, 9) = 6;
        a(9, 9) = 8;

        lolunit_assert_equal(a(0, 0), 2);
        lolunit_assert_equal(a(9, 0), 4);
        lolunit_assert_equal(a(0, 9), 6);
        lolunit_assert_equal(a(9, 9), 8);

        array2d<int> const &b = a;

        lolunit_assert_equal(b(0, 0), 2);
        lolunit_assert_equal(b(9, 0), 4);
        lolunit_assert_equal(b(0, 9), 6);
        lolunit_assert_equal(b(9, 9), 8);
    }

    lolunit_declare_test(array2d_layout)
    {
        array2d<int> a(3, 2);

        // Check that the first element points to the array data
        lolunit_assert_equal(&a(0, 0), a.data());

        // Check that the layout is row-major
        for (int i = 0; i < 6; ++i)
            a.data()[i] = i;

        lolunit_assert_equal(a(0, 0), 0);
        lolunit_assert_equal(a(1, 0), 1);
        lolunit_assert_equal(a(2, 0), 2);
        lolunit_assert_equal(a(0, 1), 3);
        lolunit_assert_equal(a(1, 1), 4);
        lolunit_assert_equal(a(2, 1), 5);
    }

    lolunit_declare_test(array2d_resize)
    {
        array2d<int> a;
        array2d<int> b(11, 12);
        array2d<int> c(ivec2(11, 12));

        a.resize(11, 12);
        b.resize(ivec2(10, 10));
        c.clear();

        lolunit_assert_equal(a.size(), 11 * 12);
        lolunit_assert_equal(a.bytes(), 11 * 12 * sizeof(int));
        lolunit_assert_equal(a.sizes()[0], 11);
        lolunit_assert_equal(a.sizes()[1], 12);
    }

    lolunit_declare_test(array2d_data)
    {
        array2d<int> a(3, 5);
        auto const &d = a;

        lolunit_assert_equal(a.data(), d.data());

        a[0] = 12;
        lolunit_assert_equal(a[0], d[0]);
        lolunit_assert_equal(a(0, 0), d(0, 0));
        lolunit_assert_equal(a(ivec2(0, 0)), d(ivec2(0, 0)));
    }

    lolunit_declare_test(array2d_views)
    {
        array2d<int> a(5, 3);
        array2d_view<int> b(a);

        lolunit_assert_equal(a.data(), b.data());

        a(3, 1) = 42;
        lolunit_assert_equal(b(3, 1), 42);

        b(4, 2) = 12;
        lolunit_assert_equal(a(4, 2), 12);
    }

 #if 0
    lolunit_declare_test(array2d_init)
    {
        array2d<int> a = { { 1, 2, 3, 4 },
                           { 5, 6, 7, 8 },
                           { 9, 8, 7, 6 } };

        lolunit_assert_equal(a.sizes().x, 4);
        lolunit_assert_equal(a.sizes().y, 3);

        lolunit_assert_equal(a(0, 0), 1);
        lolunit_assert_equal(a(1, 0), 2);
        lolunit_assert_equal(a(2, 0), 3);
        lolunit_assert_equal(a(3, 0), 4);

        lolunit_assert_equal(a(0, 1), 5);
        lolunit_assert_equal(a(1, 1), 6);
        lolunit_assert_equal(a(2, 1), 7);
        lolunit_assert_equal(a(3, 1), 8);

        lolunit_assert_equal(a(0, 2), 9);
        lolunit_assert_equal(a(1, 2), 8);
        lolunit_assert_equal(a(2, 2), 7);
        lolunit_assert_equal(a(3, 2), 6);
    }
#endif
};

} // namespace lol

