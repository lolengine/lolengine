//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

struct TrackedObj
{
    static int m_ctor, m_dtor;

    TrackedObj() { m_ctor++; }
    TrackedObj(TrackedObj const &) { m_ctor++; }
    ~TrackedObj() { m_dtor++; }
};

int TrackedObj::m_ctor = 0;
int TrackedObj::m_dtor = 0;

lolunit_declare_fixture(ArrayTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(ArrayPush)
    {
        array<int> a;
        a.push(0);
        a.push(1);
        a.push(2);
        a.push(3);

        lolunit_assert_equal(a[0], 0);
        lolunit_assert_equal(a[1], 1);
        lolunit_assert_equal(a[2], 2);
        lolunit_assert_equal(a[3], 3);
    }

    lolunit_declare_test(ArrayInitializer)
    {
        array<int> a({ 2, 4, 6 });

        lolunit_assert_equal(a[0], 2);
        lolunit_assert_equal(a[1], 4);
        lolunit_assert_equal(a[2], 6);

        array<int> b = { 2, 4, 6 };

        lolunit_assert_equal(b[0], 2);
        lolunit_assert_equal(b[1], 4);
        lolunit_assert_equal(b[2], 6);

        array<int, float> c = { { 2, 3.0f },
                                { 4, 5.0f },
                                { 6, 7.0f } };

        lolunit_assert_equal(c[0].m1, 2);
        lolunit_assert_equal(c[0].m2, 3.0f);
        lolunit_assert_equal(c[1].m1, 4);
        lolunit_assert_equal(c[1].m2, 5.0f);
        lolunit_assert_equal(c[2].m1, 6);
        lolunit_assert_equal(c[2].m2, 7.0f);
    }

    lolunit_declare_test(ArrayPushWithShift)
    {
        array<int> a;
        a << 0 << 1 << 2 << 3;

        lolunit_assert_equal(a[0], 0);
        lolunit_assert_equal(a[1], 1);
        lolunit_assert_equal(a[2], 2);
        lolunit_assert_equal(a[3], 3);
    }

    lolunit_declare_test(ArrayCopy)
    {
        array<int> a;
        a << 0 << 1 << 2 << 3;

        array<int> b = a;

        lolunit_assert_equal(b[0], 0);
        lolunit_assert_equal(b[1], 1);
        lolunit_assert_equal(b[2], 2);
        lolunit_assert_equal(b[3], 3);
    }

    lolunit_declare_test(ArrayRemove)
    {
        array<int> a;
        a << 0 << 1 << 2 << 3;
        a.remove(1);

        lolunit_assert_equal(a.count(), 3);
        lolunit_assert_equal(a[0], 0);
        lolunit_assert_equal(a[1], 2);
        lolunit_assert_equal(a[2], 3);

        array<int> b;
        b << 0 << 1 << 2 << 3;
        b.remove(-2);

        lolunit_assert_equal(b.count(), 3);
        lolunit_assert_equal(b[0], 0);
        lolunit_assert_equal(b[1], 1);
        lolunit_assert_equal(b[2], 3);
    }

    lolunit_declare_test(ArrayRemoveSwap)
    {
        array<int> a;
        a << 0 << 1 << 2 << 3;
        a.remove_swap(1);

        lolunit_assert_equal(a.count(), 3);
        lolunit_assert_equal(a[0], 0);
        lolunit_assert_equal(a[1], 3);
        lolunit_assert_equal(a[2], 2);

        array<int> b;
        b << 0 << 1 << 2 << 3;
        b.remove(1, 2);

        lolunit_assert_equal(b.count(), 2);
        lolunit_assert_equal(b[0], 0);
        lolunit_assert_equal(b[1], 3);
    }

    lolunit_declare_test(EightElements)
    {
        array<int, long, float, double, unsigned, char, bool, void *> a;
        a.push(1, 2, 3.f, 4.0, 5, 'a', true, 0);

        lolunit_assert_equal(a[0].m1, 1);
        lolunit_assert_equal(a[0].m2, 2);
        lolunit_assert_equal(a[0].m3, 3.f);
        lolunit_assert_equal(a[0].m4, 4.0);
        lolunit_assert_equal(a[0].m5, 5);
        lolunit_assert_equal(a[0].m6, 'a');
        lolunit_assert_equal(a[0].m7, true);
        lolunit_assert_equal(a[0].m8, 0);
    }

    lolunit_declare_test(ArraySwap)
    {
        array<int, int> a;
        a.push(10, 20);
        a.push(30, 40);

        std::swap(a[0], a[1]);

        lolunit_assert_equal(30, a[0].m1);
        lolunit_assert_equal(40, a[0].m2);
        lolunit_assert_equal(10, a[1].m1);
        lolunit_assert_equal(20, a[1].m2);
    }

    lolunit_declare_test(ArrayInsert)
    {
        array<int> a;
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

    lolunit_declare_test(ArrayInsertTuple)
    {
        array<int, float, String> b;
        b.insert(0, 5, 6.f, "lol");
        lolunit_assert_equal(5, b[0].m1);
        lolunit_assert_equal(6.f, b[0].m2);

        b.insert(1, 8, 9.f, "hi there");
        lolunit_assert_equal(5, b[0].m1);
        lolunit_assert_equal(6.f, b[0].m2);
        lolunit_assert_equal(8, b[1].m1);
        lolunit_assert_equal(9.f, b[1].m2);

        b.insert(1, 4, 5.f, "anyone home?");
        lolunit_assert_equal(5, b[0].m1);
        lolunit_assert_equal(6.f, b[0].m2);
        lolunit_assert_equal(4, b[1].m1);
        lolunit_assert_equal(5.f, b[1].m2);
        lolunit_assert_equal(8, b[2].m1);
        lolunit_assert_equal(9.f, b[2].m2);
    }

    lolunit_declare_test(ArrayConcat)
    {
        array<int> a, b;
        a << 0 << 1;
        b << 2 << 3;

        array<int> c = a + b;
        lolunit_assert_equal(c[0], 0);
        lolunit_assert_equal(c[1], 1);
        lolunit_assert_equal(c[2], 2);
        lolunit_assert_equal(c[3], 3);
    }

    lolunit_declare_test(ArrayAppend)
    {
        array<int> a, b;
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

    lolunit_declare_test(ElementCtorDtor)
    {
        /* Ensure array elements get created and destroyed the proper
         * number of times. */
        TrackedObj::m_ctor = 0;
        TrackedObj::m_dtor = 0;
        {
            array<TrackedObj> a;

            a.push(TrackedObj());
        }
        lolunit_assert_equal(TrackedObj::m_ctor, TrackedObj::m_dtor);

        TrackedObj::m_ctor = 0;
        TrackedObj::m_dtor = 0;
        {
            array<TrackedObj> a;

            a.resize(2);
            a.resize(4);
            a.resize(1);
        }
        lolunit_assert_equal(TrackedObj::m_ctor, TrackedObj::m_dtor);
    }
};

} /* namespace lol */

