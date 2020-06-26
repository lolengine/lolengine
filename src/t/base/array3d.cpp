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
#include <lol/narray> // lol::array3d

namespace lol
{

lolunit_declare_fixture(array3d_test)
{
    lolunit_declare_test(array3d_create)
    {
        array3d<int> a(10, 10, 10);

        a(0, 0, 0) = 2;
        a(9, 0, 0) = 4;
        a(0, 9, 9) = 6;
        a(9, 9, 9) = 8;

        lolunit_assert_equal(a(0, 0, 0), 2);
        lolunit_assert_equal(a(9, 0, 0), 4);
        lolunit_assert_equal(a(0, 9, 9), 6);
        lolunit_assert_equal(a(9, 9, 9), 8);

        array3d<int> const &b = a;

        lolunit_assert_equal(b(0, 0, 0), 2);
        lolunit_assert_equal(b(9, 0, 0), 4);
        lolunit_assert_equal(b(0, 9, 9), 6);
        lolunit_assert_equal(b(9, 9, 9), 8);
    }

    lolunit_declare_test(array3d_layout)
    {
        array3d<int> a(3, 2, 2);

        // Check that the first element points to the array data
        lolunit_assert_equal(&a(0, 0, 0), a.data());

        // Check that the layout is row-major
        for (int i = 0; i < 12; ++i)
            a[i] = i;

        lolunit_assert_equal(a(0, 0, 0), 0);
        lolunit_assert_equal(a(1, 0, 0), 1);
        lolunit_assert_equal(a(2, 0, 0), 2);
        lolunit_assert_equal(a(0, 1, 0), 3);
        lolunit_assert_equal(a(1, 1, 0), 4);
        lolunit_assert_equal(a(2, 1, 0), 5);
        lolunit_assert_equal(a(0, 0, 1), 6);
        lolunit_assert_equal(a(1, 0, 1), 7);
        lolunit_assert_equal(a(2, 0, 1), 8);
        lolunit_assert_equal(a(0, 1, 1), 9);
        lolunit_assert_equal(a(1, 1, 1), 10);
        lolunit_assert_equal(a(2, 1, 1), 11);
    }

    lolunit_declare_test(array3d_resize)
    {
        array3d<int> a;
        array3d<int> b(11, 12, 13);
        array3d<int> c(ivec3(11, 12, 13));

        a.resize(11, 12, 13);
        b.resize(ivec3(10, 10, 10));
        c.clear();

        lolunit_assert_equal(a.size(), 11 * 12 * 13);
        lolunit_assert_equal(a.bytes(), 11 * 12 * 13 * sizeof(int));
        lolunit_assert_equal(a.sizes()[0], 11);
        lolunit_assert_equal(a.sizes()[1], 12);
        lolunit_assert_equal(a.sizes()[2], 13);
    }

    lolunit_declare_test(array3d_data)
    {
        array3d<int> a(3, 4, 5);
        auto const &d = a;

        lolunit_assert_equal(a.data(), d.data());

        a[0] = 12;
        lolunit_assert_equal(a[0], d[0]);
        lolunit_assert_equal(a(0, 0, 0), d(0, 0, 0));
        lolunit_assert_equal(a(ivec3(0, 0, 0)), d(ivec3(0, 0, 0)));
    }

    lolunit_declare_test(array3d_views)
    {
        array3d<int> a(5, 4, 3);
        array3d_view<int> b(a);

        lolunit_assert_equal(a.data(), b.data());

        a(3, 2, 1) = 42;
        lolunit_assert_equal(b(3, 2, 1), 42);

        b(4, 3, 2) = 12;
        lolunit_assert_equal(a(4, 3, 2), 12);
    }

    lolunit_declare_test(array3d_iterator)
    {
        array3d<int> a(2, 2, 2);
        for (int i = 0; i < 8; ++i)
            a[i] = i;

        int n = 0;
        int sum = 0;
        for (auto &i : a)
        {
            ++n;
            sum = sum * 10 + i;
        }

        lolunit_assert_equal(n, 8);
        lolunit_assert_equal(sum, 1234567);
    }

#if 0
    lolunit_declare_test(array3d_init)
    {
        array3d<int> a = { { {  1,  2,  3,  4 },
                             {  5,  6,  7,  8 },
                             {  9,  8,  7,  6 } },
                           { { -1, -2, -3, -4 },
                             { -5, -6, -7, -8 },
                             { -9, -8, -7, -6 } } };

        lolunit_assert_equal(a.sizes().x, 4);
        lolunit_assert_equal(a.sizes().y, 3);
        lolunit_assert_equal(a.sizes().z, 2);

        lolunit_assert_equal(a(0, 0, 0), 1);
        lolunit_assert_equal(a(1, 0, 0), 2);
        lolunit_assert_equal(a(2, 0, 0), 3);
        lolunit_assert_equal(a(3, 0, 0), 4);

        lolunit_assert_equal(a(0, 1, 0), 5);
        lolunit_assert_equal(a(1, 1, 0), 6);
        lolunit_assert_equal(a(2, 1, 0), 7);
        lolunit_assert_equal(a(3, 1, 0), 8);

        lolunit_assert_equal(a(0, 2, 0), 9);
        lolunit_assert_equal(a(1, 2, 0), 8);
        lolunit_assert_equal(a(2, 2, 0), 7);
        lolunit_assert_equal(a(3, 2, 0), 6);

        lolunit_assert_equal(a(0, 0, 1), -1);
        lolunit_assert_equal(a(1, 0, 1), -2);
        lolunit_assert_equal(a(2, 0, 1), -3);
        lolunit_assert_equal(a(3, 0, 1), -4);

        lolunit_assert_equal(a(0, 1, 1), -5);
        lolunit_assert_equal(a(1, 1, 1), -6);
        lolunit_assert_equal(a(2, 1, 1), -7);
        lolunit_assert_equal(a(3, 1, 1), -8);

        lolunit_assert_equal(a(0, 2, 1), -9);
        lolunit_assert_equal(a(1, 2, 1), -8);
        lolunit_assert_equal(a(2, 2, 1), -7);
        lolunit_assert_equal(a(3, 2, 1), -6);
    }
#endif
};

} // namespace lol

