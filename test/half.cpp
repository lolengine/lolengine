//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>

#include <cmath>

#include "core.h"

/* This will not work with aggressive optimisation, but a reasonable
 * assumption is that such environments do have a proper isnan(). */
#if !defined isnan
#   define isnan(x) (!((x) == (x)))
#endif

namespace lol
{

class HalfTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(HalfTest);
    CPPUNIT_TEST(test_half_makebits);
    CPPUNIT_TEST(test_half_makeslow);
    CPPUNIT_TEST(test_half_makefast);
    CPPUNIT_TEST(test_half_is_nan);
    CPPUNIT_TEST(test_half_is_inf);
    CPPUNIT_TEST(test_half_is_finite);
    CPPUNIT_TEST(test_half_is_normal);
    CPPUNIT_TEST(test_half_classify);
    CPPUNIT_TEST(test_half_to_float);
    CPPUNIT_TEST(test_half_to_int);
    CPPUNIT_TEST(test_float_op_half);
    CPPUNIT_TEST(test_half_op_float);
    CPPUNIT_TEST_SUITE_END();

public:
    HalfTest() : CppUnit::TestCase("Matrix Test") {}

    void setUp()
    {
    }

    void tearDown() {}

    void test_half_makebits()
    {
        for (unsigned int i = 0; i < 0x10000; i++)
        {
            half a = half::makebits(i);
            uint16_t b = i;
            CPPUNIT_ASSERT_EQUAL(a.bits, b);
        }
    }

    void test_half_makeslow()
    {
        for (size_t i = 0; i < sizeof(pairs) / sizeof(*pairs); i++)
        {
            half a = half::makeslow(pairs[i].f);
            uint16_t b = pairs[i].x;
            CPPUNIT_ASSERT_EQUAL(a.bits, b);
        }
    }

    void test_half_makefast()
    {
        for (size_t i = 0; i < sizeof(pairs) / sizeof(*pairs); i++)
        {
            half a = half::makefast(pairs[i].f);
            uint16_t b = pairs[i].x;
            CPPUNIT_ASSERT_EQUAL(a.bits, b);
        }
    }

    void test_half_is_nan()
    {
        CPPUNIT_ASSERT(half::makebits(0x7c01).is_nan());
        CPPUNIT_ASSERT(half::makebits(0xfc01).is_nan());
        CPPUNIT_ASSERT(half::makebits(0x7e00).is_nan());
        CPPUNIT_ASSERT(half::makebits(0xfe00).is_nan());

        CPPUNIT_ASSERT(!half::makebits(0x7c00).is_nan());
        CPPUNIT_ASSERT(!half::makebits(0xfc00).is_nan());

        CPPUNIT_ASSERT(!half(0.0f).is_nan());
        CPPUNIT_ASSERT(!half(-0.0f).is_nan());
        CPPUNIT_ASSERT(!half(2.0f).is_nan());
        CPPUNIT_ASSERT(!half(-2.0f).is_nan());
    }

    void test_half_is_inf()
    {
        CPPUNIT_ASSERT(half(65536.0f).is_inf());
        CPPUNIT_ASSERT(half(-65536.0f).is_inf());

        CPPUNIT_ASSERT(!half(0.0f).is_inf());
        CPPUNIT_ASSERT(!half(-0.0f).is_inf());
        CPPUNIT_ASSERT(!half(65535.0f).is_inf());
        CPPUNIT_ASSERT(!half(-65535.0f).is_inf());

        CPPUNIT_ASSERT(half::makebits(0x7c00).is_inf());
        CPPUNIT_ASSERT(half::makebits(0xfc00).is_inf());

        CPPUNIT_ASSERT(!half::makebits(0x7e00).is_inf());
        CPPUNIT_ASSERT(!half::makebits(0xfe00).is_inf());
    }

    void test_half_is_finite()
    {
        CPPUNIT_ASSERT(half(0.0f).is_finite());
        CPPUNIT_ASSERT(half(-0.0f).is_finite());
        CPPUNIT_ASSERT(half(65535.0f).is_finite());
        CPPUNIT_ASSERT(half(-65535.0f).is_finite());

        CPPUNIT_ASSERT(!half(65536.0f).is_finite());
        CPPUNIT_ASSERT(!half(-65536.0f).is_finite());

        CPPUNIT_ASSERT(!half::makebits(0x7c00).is_finite());
        CPPUNIT_ASSERT(!half::makebits(0xfc00).is_finite());

        CPPUNIT_ASSERT(!half::makebits(0x7e00).is_finite());
        CPPUNIT_ASSERT(!half::makebits(0xfe00).is_finite());
    }

    void test_half_is_normal()
    {
        CPPUNIT_ASSERT(half(0.0f).is_normal());
        CPPUNIT_ASSERT(half(-0.0f).is_normal());
        CPPUNIT_ASSERT(half(65535.0f).is_normal());
        CPPUNIT_ASSERT(half(-65535.0f).is_normal());

        CPPUNIT_ASSERT(!half(65536.0f).is_normal());
        CPPUNIT_ASSERT(!half(-65536.0f).is_normal());

        CPPUNIT_ASSERT(!half::makebits(0x7c00).is_normal());
        CPPUNIT_ASSERT(!half::makebits(0xfc00).is_normal());

        CPPUNIT_ASSERT(!half::makebits(0x7e00).is_normal());
        CPPUNIT_ASSERT(!half::makebits(0xfe00).is_normal());
    }

    void test_half_classify()
    {
        for (uint32_t i = 0; i < 0x10000; i++)
        {
            half h = half::makebits(i);
            if (h.is_nan())
            {
                CPPUNIT_ASSERT(!h.is_inf());
                CPPUNIT_ASSERT(!h.is_normal());
                CPPUNIT_ASSERT(!h.is_finite());
            }
            else if (h.is_inf())
            {
                CPPUNIT_ASSERT(!h.is_normal());
                CPPUNIT_ASSERT(!h.is_finite());
            }
            else
            {
                CPPUNIT_ASSERT(h.is_finite());
            }
        }
    }

    void test_half_to_float()
    {
        for (size_t i = 0; i < sizeof(pairs) / sizeof(*pairs); i++)
        {
            float a = (float)half::makebits(pairs[i].x);
            float b = pairs[i].f;
            CPPUNIT_ASSERT_EQUAL(a, b);
        }

        for (uint32_t i = 0; i < 0x10000; i++)
        {
            half h = half::makebits(i);
            float f = (float)h;
            half g = (half)f;
            if (h.is_nan())
            {
                CPPUNIT_ASSERT(isnan(f));
                CPPUNIT_ASSERT(g.is_nan());
            }
            else
            {
                CPPUNIT_ASSERT(!isnan(f));
                CPPUNIT_ASSERT_EQUAL(g.bits, h.bits);
            }
        }
    }

    void test_half_to_int()
    {
        CPPUNIT_ASSERT_EQUAL((int)(half)(0.0f), 0);
        CPPUNIT_ASSERT_EQUAL((int)(half)(-0.0f), 0);
        CPPUNIT_ASSERT_EQUAL((int)(half)(0.9f), 0);
        CPPUNIT_ASSERT_EQUAL((int)(half)(-0.9f), 0);
        CPPUNIT_ASSERT_EQUAL((int)(half)(1.0f), 1);
        CPPUNIT_ASSERT_EQUAL((int)(half)(-1.0f), -1);
        CPPUNIT_ASSERT_EQUAL((int)(half)(1.9f), 1);
        CPPUNIT_ASSERT_EQUAL((int)(half)(-1.9f), -1);
        CPPUNIT_ASSERT_EQUAL((int)(half)(65504.0f), 65504);
        CPPUNIT_ASSERT_EQUAL((int)(half)(-65504.0f), -65504);
    }

    void test_float_op_half()
    {
        half zero = 0;
        half one = 1;
        half two = 2;

        float a = zero + one;
        CPPUNIT_ASSERT_EQUAL(1.0f, a);
        a += zero;
        CPPUNIT_ASSERT_EQUAL(1.0f, a);
        a -= zero;
        CPPUNIT_ASSERT_EQUAL(1.0f, a);
        a *= one;
        CPPUNIT_ASSERT_EQUAL(1.0f, a);
        a /= one;
        CPPUNIT_ASSERT_EQUAL(1.0f, a);

        float b = one + zero;
        CPPUNIT_ASSERT_EQUAL(1.0f, b);
        b += one;
        CPPUNIT_ASSERT_EQUAL(2.0f, b);
        b *= two;
        CPPUNIT_ASSERT_EQUAL(4.0f, b);
        b -= two;
        CPPUNIT_ASSERT_EQUAL(2.0f, b);
        b /= two;
        CPPUNIT_ASSERT_EQUAL(1.0f, b);

        float c = one - zero;
        CPPUNIT_ASSERT_EQUAL(1.0f, c);

        float d = two - one;
        CPPUNIT_ASSERT_EQUAL(1.0f, d);

        float e = two + (-one);
        CPPUNIT_ASSERT_EQUAL(1.0f, e);

        float f = (two * two) / (one + one);
        CPPUNIT_ASSERT_EQUAL(2.0f, f);
    }

    void test_half_op_float()
    {
        half zero = 0;
        half one = 1;
        half two = 2;
        half four = 4;

        half a = one + 0.0f;
        CPPUNIT_ASSERT_EQUAL(one.bits, a.bits);
        a += 0.0f;
        CPPUNIT_ASSERT_EQUAL(one.bits, a.bits);
        a -= 0.0f;
        CPPUNIT_ASSERT_EQUAL(one.bits, a.bits);
        a *= 1.0f;
        CPPUNIT_ASSERT_EQUAL(one.bits, a.bits);
        a /= 1.0f;
        CPPUNIT_ASSERT_EQUAL(one.bits, a.bits);

        half b = one + 0.0f;
        CPPUNIT_ASSERT_EQUAL(one.bits, b.bits);
        b += 1.0f;
        CPPUNIT_ASSERT_EQUAL(two.bits, b.bits);
        b *= 2.0f;
        CPPUNIT_ASSERT_EQUAL(four.bits, b.bits);
        b -= 2.0f;
        CPPUNIT_ASSERT_EQUAL(two.bits, b.bits);
        b /= 2.0f;
        CPPUNIT_ASSERT_EQUAL(one.bits, b.bits);

        half c = 1.0f - zero;
        CPPUNIT_ASSERT_EQUAL(one.bits, c.bits);

        half d = 2.0f - one;
        CPPUNIT_ASSERT_EQUAL(one.bits, d.bits);

        half e = 2.0f + (-one);
        CPPUNIT_ASSERT_EQUAL(one.bits, e.bits);

        half f = (2.0f * two) / (1.0f + one);
        CPPUNIT_ASSERT_EQUAL(two.bits, f.bits);
    }

private:
    struct TestPair { float f; uint16_t x; };

    static TestPair const pairs[11];
};

HalfTest::TestPair const HalfTest::pairs[] =
{
    /* All these values have exact half representations */
    { 0.0f,      0x0000 },
    { -0.0f,     0x8000 }, /* negative zero */
    { 1.0f,      0x3c00 },
    { -1.0f,     0xbc00 },
    { 2.0f,      0x4000 },
    { 0.5f,      0x3800 },
    { 0.125f,    0x3000 },
    { 15.9375f,  0x4bf8 },
    { 0x1.fp-10, 0x17c0 },
    { 0x1.fp-14, 0x07c0 }, /* denormal */
    { 0x1.fp-15, 0x03e0 }, /* denormal */
};

CPPUNIT_TEST_SUITE_REGISTRATION(HalfTest);

} /* namespace lol */

