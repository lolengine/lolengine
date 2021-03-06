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
#include <lol/../easymesh/easyarray.h>

namespace lol
{

struct tracked_object
{
    static int m_ctor, m_dtor;

    tracked_object() { m_ctor++; }
    tracked_object(tracked_object const &) { m_ctor++; }
    ~tracked_object() { m_dtor++; }
};

int tracked_object::m_ctor = 0;
int tracked_object::m_dtor = 0;

lolunit_declare_fixture(array_test)
{
    lolunit_declare_test(array_push)
    {
        easy_array<int> a;
        a.push(0);
        a.push(1);
        a.push(2);
        a.push(3);

        lolunit_assert_equal(a[0], 0);
        lolunit_assert_equal(a[1], 1);
        lolunit_assert_equal(a[2], 2);
        lolunit_assert_equal(a[3], 3);
    }

    lolunit_declare_test(array_initializer)
    {
        easy_array<int> a({ 2, 4, 6 });

        lolunit_assert_equal(a[0], 2);
        lolunit_assert_equal(a[1], 4);
        lolunit_assert_equal(a[2], 6);

        easy_array<int> b = { 2, 4, 6 };

        lolunit_assert_equal(b[0], 2);
        lolunit_assert_equal(b[1], 4);
        lolunit_assert_equal(b[2], 6);

        easy_array<int, float> c = { { 2, 3.0f },
                                     { 4, 5.0f },
                                     { 6, 7.0f } };

        lolunit_assert_equal(std::get<0>(c[0]), 2);
        lolunit_assert_equal(std::get<1>(c[0]), 3.0f);
        lolunit_assert_equal(std::get<0>(c[1]), 4);
        lolunit_assert_equal(std::get<1>(c[1]), 5.0f);
        lolunit_assert_equal(std::get<0>(c[2]), 6);
        lolunit_assert_equal(std::get<1>(c[2]), 7.0f);
    }

    lolunit_declare_test(array_push_with_shift)
    {
        easy_array<int> a;
        a << 0 << 1 << 2 << 3;

        lolunit_assert_equal(a[0], 0);
        lolunit_assert_equal(a[1], 1);
        lolunit_assert_equal(a[2], 2);
        lolunit_assert_equal(a[3], 3);
    }

    lolunit_declare_test(array_copy)
    {
        easy_array<int> a;
        a << 0 << 1 << 2 << 3;

        easy_array<int> b = a;

        lolunit_assert_equal(b[0], 0);
        lolunit_assert_equal(b[1], 1);
        lolunit_assert_equal(b[2], 2);
        lolunit_assert_equal(b[3], 3);
    }

    lolunit_declare_test(array_remove)
    {
        easy_array<int> a;
        a << 0 << 1 << 2 << 3;
        a.remove(1);

        lolunit_assert_equal(a.count(), 3);
        lolunit_assert_equal(a[0], 0);
        lolunit_assert_equal(a[1], 2);
        lolunit_assert_equal(a[2], 3);

        easy_array<int> b;
        b << 0 << 1 << 2 << 3;
        b.remove(-2);

        lolunit_assert_equal(b.count(), 3);
        lolunit_assert_equal(b[0], 0);
        lolunit_assert_equal(b[1], 1);
        lolunit_assert_equal(b[2], 3);
    }

    lolunit_declare_test(array_remove_swap)
    {
        easy_array<int> a;
        a << 0 << 1 << 2 << 3;
        a.remove_swap(1);

        lolunit_assert_equal(a.count(), 3);
        lolunit_assert_equal(a[0], 0);
        lolunit_assert_equal(a[1], 3);
        lolunit_assert_equal(a[2], 2);

        easy_array<int> b;
        b << 0 << 1 << 2 << 3;
        b.remove(1, 2);

        lolunit_assert_equal(b.count(), 2);
        lolunit_assert_equal(b[0], 0);
        lolunit_assert_equal(b[1], 3);
    }

    lolunit_declare_test(eight_element_arrays)
    {
        easy_array<int, long, float, double, unsigned, char, bool, void *> a;
        a.push(1, 2, 3.f, 4.0, 5, 'a', true, 0);

        lolunit_assert_equal(std::get<0>(a[0]), 1);
        lolunit_assert_equal(std::get<1>(a[0]), 2);
        lolunit_assert_equal(std::get<2>(a[0]), 3.f);
        lolunit_assert_equal(std::get<3>(a[0]), 4.0);
        lolunit_assert_equal(std::get<4>(a[0]), 5);
        lolunit_assert_equal(std::get<5>(a[0]), 'a');
        lolunit_assert_equal(std::get<6>(a[0]), true);
        lolunit_assert_equal(std::get<7>(a[0]), 0);
    }

    lolunit_declare_test(array_swap)
    {
        easy_array<int, int> a;
        a.push(10, 20);
        a.push(30, 40);

        a.swap(0, 1);

        lolunit_assert_equal(30, std::get<0>(a[0]));
        lolunit_assert_equal(40, std::get<1>(a[0]));
        lolunit_assert_equal(10, std::get<0>(a[1]));
        lolunit_assert_equal(20, std::get<1>(a[1]));
    }

    lolunit_declare_test(array_insert)
    {
        easy_array<int> a;
        a << 1 << 2;

        a.insert(5, 0);
        lolunit_assert_equal(5, a[0]);
        lolunit_assert_equal(1, a[1]);
        lolunit_assert_equal(2, a[2]);

        a.insert(6, 3);
        lolunit_assert_equal(5, a[0]);
        lolunit_assert_equal(1, a[1]);
        lolunit_assert_equal(2, a[2]);
        lolunit_assert_equal(6, a[3]);

        a.insert(7, 2);
        lolunit_assert_equal(5, a[0]);
        lolunit_assert_equal(1, a[1]);
        lolunit_assert_equal(7, a[2]);
        lolunit_assert_equal(2, a[3]);
        lolunit_assert_equal(6, a[4]);
    }

    lolunit_declare_test(array_insert_tuple)
    {
        easy_array<int, float, std::string> b;
        b.insert(0, 5, 6.f, "lol");
        lolunit_assert_equal(5,   std::get<0>(b[0]));
        lolunit_assert_equal(6.f, std::get<1>(b[0]));

        b.insert(1, 8, 9.f, "hi there");
        lolunit_assert_equal(5,   std::get<0>(b[0]));
        lolunit_assert_equal(6.f, std::get<1>(b[0]));
        lolunit_assert_equal(8,   std::get<0>(b[1]));
        lolunit_assert_equal(9.f, std::get<1>(b[1]));

        b.insert(1, 4, 5.f, "anyone home?");
        lolunit_assert_equal(5,   std::get<0>(b[0]));
        lolunit_assert_equal(6.f, std::get<1>(b[0]));
        lolunit_assert_equal(4,   std::get<0>(b[1]));
        lolunit_assert_equal(5.f, std::get<1>(b[1]));
        lolunit_assert_equal(8,   std::get<0>(b[2]));
        lolunit_assert_equal(9.f, std::get<1>(b[2]));
    }

    lolunit_declare_test(array_concat)
    {
        easy_array<int> a, b;
        a << 0 << 1;
        b << 2 << 3;

        easy_array<int> c = a + b;
        lolunit_assert_equal(c[0], 0);
        lolunit_assert_equal(c[1], 1);
        lolunit_assert_equal(c[2], 2);
        lolunit_assert_equal(c[3], 3);
    }

    lolunit_declare_test(array_append)
    {
        easy_array<int> a, b;
        a << 0 << 1;
        b << 2 << 3;

        a += b;
        lolunit_assert_equal(a[0], 0);
        lolunit_assert_equal(a[1], 1);
        lolunit_assert_equal(a[2], 2);
        lolunit_assert_equal(a[3], 3);

        b += b;
        lolunit_assert_equal(b[0], 2);
        lolunit_assert_equal(b[1], 3);
        lolunit_assert_equal(b[2], 2);
        lolunit_assert_equal(b[3], 3);
    }

    lolunit_declare_test(element_ctor_dtor)
    {
        /* Ensure array elements get created and destroyed the proper
         * number of times. */
        tracked_object::m_ctor = 0;
        tracked_object::m_dtor = 0;
        {
            easy_array<tracked_object> a;

            a.push(tracked_object());
        }
        lolunit_assert_equal(tracked_object::m_ctor, tracked_object::m_dtor);

        tracked_object::m_ctor = 0;
        tracked_object::m_dtor = 0;
        {
            easy_array<tracked_object> a;

            a.resize(2);
            a.resize(4);
            a.resize(1);
        }
        lolunit_assert_equal(tracked_object::m_ctor, tracked_object::m_dtor);
    }
};

} /* namespace lol */

