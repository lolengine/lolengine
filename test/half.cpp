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

#include <cmath>

#include "core.h"
#include "lol/unit.h"

/* Ensure isnan() is present even on systems that don't define it, or
 * when -ffast-math is being used. */
#if defined __FAST_MATH__
#   undef isnan
#endif
#if !defined isnan
static inline int isnan(float f)
{
    union { float f; uint32_t x; } u = { f };
    return (u.x << 1) > 0xff000000u;
}
#endif

namespace lol
{

LOLUNIT_FIXTURE(HalfTest)
{
public:
    void setUp() {}
    void tearDown() {}

    LOLUNIT_TEST(test_half_from_float)
    {
        for (size_t i = 0; i < sizeof(pairs) / sizeof(*pairs); i++)
        {
            half a = (half)pairs[i].f;
            uint16_t b = pairs[i].x;
            LOLUNIT_ASSERT_EQUAL(a.bits, b);
        }
    }

    LOLUNIT_TEST(test_half_makeaccurate)
    {
        for (size_t i = 0; i < sizeof(pairs) / sizeof(*pairs); i++)
        {
            half a = half::makeaccurate(pairs[i].f);
            uint16_t b = pairs[i].x;
            LOLUNIT_ASSERT_EQUAL(a.bits, b);
        }
    }

    LOLUNIT_TEST(test_half_makebits)
    {
        for (unsigned int i = 0; i < 0x10000; i++)
        {
            half a = half::makebits(i);
            uint16_t b = i;
            LOLUNIT_ASSERT_EQUAL(a.bits, b);
        }
    }

    LOLUNIT_TEST(test_half_is_nan)
    {
        LOLUNIT_ASSERT(half::makebits(0x7c01).is_nan());
        LOLUNIT_ASSERT(half::makebits(0xfc01).is_nan());
        LOLUNIT_ASSERT(half::makebits(0x7e00).is_nan());
        LOLUNIT_ASSERT(half::makebits(0xfe00).is_nan());

        LOLUNIT_ASSERT(!half::makebits(0x7c00).is_nan());
        LOLUNIT_ASSERT(!half::makebits(0xfc00).is_nan());

        LOLUNIT_ASSERT(!half(0.0f).is_nan());
        LOLUNIT_ASSERT(!half(-0.0f).is_nan());
        LOLUNIT_ASSERT(!half(2.0f).is_nan());
        LOLUNIT_ASSERT(!half(-2.0f).is_nan());
    }

    LOLUNIT_TEST(test_half_is_inf)
    {
        LOLUNIT_ASSERT(half(65536.0f).is_inf());
        LOLUNIT_ASSERT(half(-65536.0f).is_inf());

        LOLUNIT_ASSERT(!half(0.0f).is_inf());
        LOLUNIT_ASSERT(!half(-0.0f).is_inf());
        LOLUNIT_ASSERT(!half(65535.0f).is_inf());
        LOLUNIT_ASSERT(!half(-65535.0f).is_inf());

        LOLUNIT_ASSERT(half::makebits(0x7c00).is_inf());
        LOLUNIT_ASSERT(half::makebits(0xfc00).is_inf());

        LOLUNIT_ASSERT(!half::makebits(0x7e00).is_inf());
        LOLUNIT_ASSERT(!half::makebits(0xfe00).is_inf());
    }

    LOLUNIT_TEST(test_half_is_finite)
    {
        LOLUNIT_ASSERT(half(0.0f).is_finite());
        LOLUNIT_ASSERT(half(-0.0f).is_finite());
        LOLUNIT_ASSERT(half(65535.0f).is_finite());
        LOLUNIT_ASSERT(half(-65535.0f).is_finite());

        LOLUNIT_ASSERT(!half(65536.0f).is_finite());
        LOLUNIT_ASSERT(!half(-65536.0f).is_finite());

        LOLUNIT_ASSERT(!half::makebits(0x7c00).is_finite());
        LOLUNIT_ASSERT(!half::makebits(0xfc00).is_finite());

        LOLUNIT_ASSERT(!half::makebits(0x7e00).is_finite());
        LOLUNIT_ASSERT(!half::makebits(0xfe00).is_finite());
    }

    LOLUNIT_TEST(test_half_is_normal)
    {
        LOLUNIT_ASSERT(half(0.0f).is_normal());
        LOLUNIT_ASSERT(half(-0.0f).is_normal());
        LOLUNIT_ASSERT(half(65535.0f).is_normal());
        LOLUNIT_ASSERT(half(-65535.0f).is_normal());

        LOLUNIT_ASSERT(!half(65536.0f).is_normal());
        LOLUNIT_ASSERT(!half(-65536.0f).is_normal());

        LOLUNIT_ASSERT(!half::makebits(0x7c00).is_normal());
        LOLUNIT_ASSERT(!half::makebits(0xfc00).is_normal());

        LOLUNIT_ASSERT(!half::makebits(0x7e00).is_normal());
        LOLUNIT_ASSERT(!half::makebits(0xfe00).is_normal());
    }

    LOLUNIT_TEST(test_half_classify)
    {
        for (uint32_t i = 0; i < 0x10000; i++)
        {
            half h = half::makebits(i);
            if (h.is_nan())
            {
                LOLUNIT_ASSERT(!h.is_inf());
                LOLUNIT_ASSERT(!h.is_normal());
                LOLUNIT_ASSERT(!h.is_finite());
            }
            else if (h.is_inf())
            {
                LOLUNIT_ASSERT(!h.is_normal());
                LOLUNIT_ASSERT(!h.is_finite());
            }
            else
            {
                LOLUNIT_ASSERT(h.is_finite());
            }
        }
    }

    LOLUNIT_TEST(test_half_to_float)
    {
        for (size_t i = 0; i < sizeof(pairs) / sizeof(*pairs); i++)
        {
            float a = (float)half::makebits(pairs[i].x);
            float b = pairs[i].f;
            LOLUNIT_ASSERT_EQUAL(a, b);
        }

        for (uint32_t i = 0; i < 0x10000; i++)
        {
            half h = half::makebits(i);
            float f = (float)h;
            half g = (half)f;
            if (h.is_nan())
            {
                LOLUNIT_ASSERT(isnan(f));
                LOLUNIT_ASSERT(g.is_nan());
            }
            else
            {
                LOLUNIT_ASSERT(!isnan(f));
                LOLUNIT_ASSERT_EQUAL(g.bits, h.bits);
            }
        }
    }

    LOLUNIT_TEST(test_half_to_int)
    {
        LOLUNIT_ASSERT_EQUAL((int)(half)(0.0f), 0);
        LOLUNIT_ASSERT_EQUAL((int)(half)(-0.0f), 0);
        LOLUNIT_ASSERT_EQUAL((int)(half)(0.9f), 0);
        LOLUNIT_ASSERT_EQUAL((int)(half)(-0.9f), 0);
        LOLUNIT_ASSERT_EQUAL((int)(half)(1.0f), 1);
        LOLUNIT_ASSERT_EQUAL((int)(half)(-1.0f), -1);
        LOLUNIT_ASSERT_EQUAL((int)(half)(1.9f), 1);
        LOLUNIT_ASSERT_EQUAL((int)(half)(-1.9f), -1);
        LOLUNIT_ASSERT_EQUAL((int)(half)(65504.0f), 65504);
        LOLUNIT_ASSERT_EQUAL((int)(half)(-65504.0f), -65504);
    }

    LOLUNIT_TEST(test_float_op_half)
    {
        half zero = 0;
        half one = 1;
        half two = 2;

        float a = zero + one;
        LOLUNIT_ASSERT_EQUAL(1.0f, a);
        a += zero;
        LOLUNIT_ASSERT_EQUAL(1.0f, a);
        a -= zero;
        LOLUNIT_ASSERT_EQUAL(1.0f, a);
        a *= one;
        LOLUNIT_ASSERT_EQUAL(1.0f, a);
        a /= one;
        LOLUNIT_ASSERT_EQUAL(1.0f, a);

        float b = one + zero;
        LOLUNIT_ASSERT_EQUAL(1.0f, b);
        b += one;
        LOLUNIT_ASSERT_EQUAL(2.0f, b);
        b *= two;
        LOLUNIT_ASSERT_EQUAL(4.0f, b);
        b -= two;
        LOLUNIT_ASSERT_EQUAL(2.0f, b);
        b /= two;
        LOLUNIT_ASSERT_EQUAL(1.0f, b);

        float c = one - zero;
        LOLUNIT_ASSERT_EQUAL(1.0f, c);

        float d = two - one;
        LOLUNIT_ASSERT_EQUAL(1.0f, d);

        float e = two + (-one);
        LOLUNIT_ASSERT_EQUAL(1.0f, e);

        float f = (two * two) / (one + one);
        LOLUNIT_ASSERT_EQUAL(2.0f, f);
    }

    LOLUNIT_TEST(test_half_op_float)
    {
        half zero = 0;
        half one = 1;
        half two = 2;
        half four = 4;

        half a = one + 0.0f;
        LOLUNIT_ASSERT_EQUAL(one.bits, a.bits);
        a += 0.0f;
        LOLUNIT_ASSERT_EQUAL(one.bits, a.bits);
        a -= 0.0f;
        LOLUNIT_ASSERT_EQUAL(one.bits, a.bits);
        a *= 1.0f;
        LOLUNIT_ASSERT_EQUAL(one.bits, a.bits);
        a /= 1.0f;
        LOLUNIT_ASSERT_EQUAL(one.bits, a.bits);

        half b = one + 0.0f;
        LOLUNIT_ASSERT_EQUAL(one.bits, b.bits);
        b += 1.0f;
        LOLUNIT_ASSERT_EQUAL(two.bits, b.bits);
        b *= 2.0f;
        LOLUNIT_ASSERT_EQUAL(four.bits, b.bits);
        b -= 2.0f;
        LOLUNIT_ASSERT_EQUAL(two.bits, b.bits);
        b /= 2.0f;
        LOLUNIT_ASSERT_EQUAL(one.bits, b.bits);

        half c = 1.0f - zero;
        LOLUNIT_ASSERT_EQUAL(one.bits, c.bits);

        half d = 2.0f - one;
        LOLUNIT_ASSERT_EQUAL(one.bits, d.bits);

        half e = 2.0f + (-one);
        LOLUNIT_ASSERT_EQUAL(one.bits, e.bits);

        half f = (2.0f * two) / (1.0f + one);
        LOLUNIT_ASSERT_EQUAL(two.bits, f.bits);
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
    { 31.0f / (1 << 14), 0x17c0 }, /* 0x1.fp-10 */
    { 31.0f / (1 << 18), 0x07c0 }, /* 0x1.fp-14, denormal */
    { 31.0f / (1 << 19), 0x03e0 }, /* 0x1.fp-15, denormal */
};

} /* namespace lol */

