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
        a.Push(0);
        a.Push(1);
        a.Push(2);
        a.Push(3);

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
        a.Remove(1);

        lolunit_assert_equal(a.Count(), 3);
        lolunit_assert_equal(a[0], 0);
        lolunit_assert_equal(a[1], 2);
        lolunit_assert_equal(a[2], 3);

        array<int> b;
        b << 0 << 1 << 2 << 3;
        b.Remove(-2);

        lolunit_assert_equal(b.Count(), 3);
        lolunit_assert_equal(b[0], 0);
        lolunit_assert_equal(b[1], 1);
        lolunit_assert_equal(b[2], 3);
    }

    lolunit_declare_test(ArrayRemoveSwap)
    {
        array<int> a;
        a << 0 << 1 << 2 << 3;
        a.RemoveSwap(1);

        lolunit_assert_equal(a.Count(), 3);
        lolunit_assert_equal(a[0], 0);
        lolunit_assert_equal(a[1], 3);
        lolunit_assert_equal(a[2], 2);

        array<int> b;
        b << 0 << 1 << 2 << 3;
        b.Remove(1, 2);

        lolunit_assert_equal(b.Count(), 2);
        lolunit_assert_equal(b[0], 0);
        lolunit_assert_equal(b[1], 3);
    }

    lolunit_declare_test(EightElements)
    {
        array<int, long, float, double, unsigned, char, bool, void *> a;
        a.Push(1, 2, 3.f, 4.0, 5, 'a', true, 0);

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
        a.Push(10, 20);
        a.Push(30, 40);
        a.Swap(0, 1);

        lolunit_assert_equal(30, a[0].m1);
        lolunit_assert_equal(40, a[0].m2);
        lolunit_assert_equal(10, a[1].m1);
        lolunit_assert_equal(20, a[1].m2);
    }

    lolunit_declare_test(ArrayInsert)
    {
        array<int> a;
        a << 1 << 2;

        a.Insert(5, 0);
        lolunit_assert_equal(5, a[0]);
        lolunit_assert_equal(1, a[1]);
        lolunit_assert_equal(2, a[2]);

        a.Insert(6, 3);
        lolunit_assert_equal(5, a[0]);
        lolunit_assert_equal(1, a[1]);
        lolunit_assert_equal(2, a[2]);
        lolunit_assert_equal(6, a[3]);

        a.Insert(7, 2);
        lolunit_assert_equal(5, a[0]);
        lolunit_assert_equal(1, a[1]);
        lolunit_assert_equal(7, a[2]);
        lolunit_assert_equal(2, a[3]);
        lolunit_assert_equal(6, a[4]);

        array<int, int, int> b;
        b.Insert(0, 5, 6, 7);
        lolunit_assert_equal(5, b[0].m1);
        lolunit_assert_equal(6, b[0].m2);
        lolunit_assert_equal(7, b[0].m3);

        b.Insert(1, 8, 9, 6);
        lolunit_assert_equal(5, b[0].m1);
        lolunit_assert_equal(6, b[0].m2);
        lolunit_assert_equal(7, b[0].m3);
        lolunit_assert_equal(8, b[1].m1);
        lolunit_assert_equal(9, b[1].m2);
        lolunit_assert_equal(6, b[1].m3);

        b.Insert(1, 4, 5, 3);
        lolunit_assert_equal(5, b[0].m1);
        lolunit_assert_equal(6, b[0].m2);
        lolunit_assert_equal(7, b[0].m3);
        lolunit_assert_equal(4, b[2].m1);
        lolunit_assert_equal(5, b[2].m2);
        lolunit_assert_equal(3, b[2].m3);
        lolunit_assert_equal(8, b[2].m1);
        lolunit_assert_equal(9, b[2].m2);
        lolunit_assert_equal(6, b[2].m3);
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

            a.Push(TrackedObj());
        }
        lolunit_assert_equal(TrackedObj::m_ctor, TrackedObj::m_dtor);

        TrackedObj::m_ctor = 0;
        TrackedObj::m_dtor = 0;
        {
            array<TrackedObj> a;

            a.Resize(2);
            a.Resize(4);
            a.Resize(1);
        }
        lolunit_assert_equal(TrackedObj::m_ctor, TrackedObj::m_dtor);
    }
};

} /* namespace lol */

