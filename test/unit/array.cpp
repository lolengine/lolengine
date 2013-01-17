//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "lol/unit.h"

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

LOLUNIT_FIXTURE(ArrayTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(ArrayPush)
    {
        Array<int> a;
        a.Push(0);
        a.Push(1);
        a.Push(2);
        a.Push(3);

        LOLUNIT_ASSERT_EQUAL(a[0], 0);
        LOLUNIT_ASSERT_EQUAL(a[1], 1);
        LOLUNIT_ASSERT_EQUAL(a[2], 2);
        LOLUNIT_ASSERT_EQUAL(a[3], 3);
    }

    LOLUNIT_TEST(ArrayPushWithShift)
    {
        Array<int> a;
        a << 0 << 1 << 2 << 3;

        LOLUNIT_ASSERT_EQUAL(a[0], 0);
        LOLUNIT_ASSERT_EQUAL(a[1], 1);
        LOLUNIT_ASSERT_EQUAL(a[2], 2);
        LOLUNIT_ASSERT_EQUAL(a[3], 3);
    }

    LOLUNIT_TEST(ArrayCopy)
    {
        Array<int> a;
        a << 0 << 1 << 2 << 3;

        Array<int> b = a;

        LOLUNIT_ASSERT_EQUAL(b[0], 0);
        LOLUNIT_ASSERT_EQUAL(b[1], 1);
        LOLUNIT_ASSERT_EQUAL(b[2], 2);
        LOLUNIT_ASSERT_EQUAL(b[3], 3);
    }

    LOLUNIT_TEST(ArrayRemove)
    {
        Array<int> a;
        a << 0 << 1 << 2 << 3;
        a.Remove(1);

        LOLUNIT_ASSERT_EQUAL(a[0], 0);
        LOLUNIT_ASSERT_EQUAL(a[1], 2);
        LOLUNIT_ASSERT_EQUAL(a[2], 3);
    }

    LOLUNIT_TEST(EightElements)
    {
        Array<int, long, float, double, unsigned, char, bool, void *> a;
        a.Push(1, 2, 3.f, 4.0, 5, 'a', true, 0);

        LOLUNIT_ASSERT_EQUAL(a[0].m1, 1);
        LOLUNIT_ASSERT_EQUAL(a[0].m2, 2);
        LOLUNIT_ASSERT_EQUAL(a[0].m3, 3.f);
        LOLUNIT_ASSERT_EQUAL(a[0].m4, 4.0);
        LOLUNIT_ASSERT_EQUAL(a[0].m5, 5);
        LOLUNIT_ASSERT_EQUAL(a[0].m6, 'a');
        LOLUNIT_ASSERT_EQUAL(a[0].m7, true);
        LOLUNIT_ASSERT_EQUAL(a[0].m8, 0);
    }

    LOLUNIT_TEST(ArrayConcat)
    {
        Array<int> a, b;
        a << 0 << 1;
        b << 2 << 3;

        Array<int> c = a + b;
        LOLUNIT_ASSERT_EQUAL(c[0], 0);
        LOLUNIT_ASSERT_EQUAL(c[1], 1);
        LOLUNIT_ASSERT_EQUAL(c[2], 2);
        LOLUNIT_ASSERT_EQUAL(c[3], 3);
    }

    LOLUNIT_TEST(ArrayAppend)
    {
        Array<int> a, b;
        a << 0 << 1;
        b << 2 << 3;

        a += b;
        LOLUNIT_ASSERT_EQUAL(a[0], 0);
        LOLUNIT_ASSERT_EQUAL(a[1], 1);
        LOLUNIT_ASSERT_EQUAL(a[2], 2);
        LOLUNIT_ASSERT_EQUAL(a[3], 3);

        b += b;
        LOLUNIT_ASSERT_EQUAL(b[0], 2);
        LOLUNIT_ASSERT_EQUAL(b[1], 3);
        LOLUNIT_ASSERT_EQUAL(b[2], 2);
        LOLUNIT_ASSERT_EQUAL(b[3], 3);
    }

    LOLUNIT_TEST(ElementCtorDtor)
    {
        /* Ensure array elements get created and destroyed the proper
         * number of times. */
        TrackedObj::m_ctor = 0;
        TrackedObj::m_dtor = 0;
        {
            Array<TrackedObj> a;

            a.Push(TrackedObj());
        }
        LOLUNIT_ASSERT_EQUAL(TrackedObj::m_ctor, TrackedObj::m_dtor);

        TrackedObj::m_ctor = 0;
        TrackedObj::m_dtor = 0;
        {
            Array<TrackedObj> a;

            a.Resize(2);
            a.Resize(4);
            a.Resize(1);
        }
        LOLUNIT_ASSERT_EQUAL(TrackedObj::m_ctor, TrackedObj::m_dtor);
    }
};

} /* namespace lol */

