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

#include <lol/engine-internal.h>
#include <lol/base/lolunit.h>

#include <cmath>

namespace lol
{

static_assert(sizeof(f16vec2) == 4, "sizeof(f16vec2) == 4");
static_assert(sizeof(f16vec3) == 6, "sizeof(f16vec3) == 6");
static_assert(sizeof(f16vec4) == 8, "sizeof(f16vec4) == 8");

static_assert(sizeof(f16mat2) == 8, "sizeof(f16mat2) == 8");
static_assert(sizeof(f16mat3) == 18, "sizeof(f16mat3) == 18");
static_assert(sizeof(f16mat4) == 32, "sizeof(f16mat4) == 32");

static_assert(sizeof(f16cmplx) == 4, "sizeof(f16cmplx) == 4");
static_assert(sizeof(f16quat) == 8, "sizeof(f16quat) == 8");

lolunit_declare_fixture(half_test)
{
    // Largest normal number is 65504 (2¹⁵*(1+1023/1024))
    static float constexpr largest_normal = 65504.f;

    lolunit_declare_test(float_to_half)
    {
        for (auto &pair : pairs)
        {
            lolunit_set_context(pair.x);

            half a = (half)pair.f;
            uint16_t b = pair.x;
            lolunit_assert_equal(a.bits(), b);
        }
    }

    lolunit_declare_test(bits_to_half)
    {
        for (unsigned int i = 0; i < 0x10000; i++)
        {
            lolunit_set_context(i);

            half a = half::frombits(i);
            uint16_t b = i;
            lolunit_assert_equal(a.bits(), b);
        }
    }

    lolunit_declare_test(equal)
    {
        for (auto &pair : pairs)
        {
             lolunit_set_context(pair.x);

             half a = half::frombits(pair.x);
             half b = half::frombits(pair.x);
             lolunit_assert_equal(a, b);
        }
    }

    lolunit_declare_test(different)
    {
        for (auto &p1 : pairs)
        for (auto &p2 : pairs)
        {
            if (p1.f == p2.f)
                continue;

            lolunit_set_context(lol::format("%04x %04x", p1.x, p2.x));

            half a = half::frombits(p1.x);
            half b = half::frombits(p2.x);
            lolunit_assert_different(a, b);
        }
    }

    lolunit_declare_test(half_is_nan)
    {
        lolunit_assert(half::frombits(0x7c01).is_nan());
        lolunit_assert(half::frombits(0xfc01).is_nan());
        lolunit_assert(half::frombits(0x7e00).is_nan());
        lolunit_assert(half::frombits(0xfe00).is_nan());

        lolunit_assert(!half::frombits(0x7c00).is_nan());
        lolunit_assert(!half::frombits(0xfc00).is_nan());

        lolunit_assert(!half(0.0f).is_nan());
        lolunit_assert(!half(-0.0f).is_nan());
        lolunit_assert(!half(2.0f).is_nan());
        lolunit_assert(!half(-2.0f).is_nan());
    }

    lolunit_declare_test(half_is_inf)
    {
        lolunit_assert(half(65536.0f).is_inf());
        lolunit_assert(half(-65536.0f).is_inf());

        lolunit_assert(!half(0.0f).is_inf());
        lolunit_assert(!half(-0.0f).is_inf());
        lolunit_assert(!half(largest_normal).is_inf());
        lolunit_assert(!half(-largest_normal).is_inf());

        lolunit_assert(half::frombits(0x7c00).is_inf());
        lolunit_assert(half::frombits(0xfc00).is_inf());

        lolunit_assert(!half::frombits(0x7e00).is_inf());
        lolunit_assert(!half::frombits(0xfe00).is_inf());
    }

    lolunit_declare_test(half_is_finite)
    {
        lolunit_assert(half(0.0f).is_finite());
        lolunit_assert(half(-0.0f).is_finite());
        lolunit_assert(half(largest_normal).is_finite());
        lolunit_assert(half(-largest_normal).is_finite());

        lolunit_assert(!half(65536.0f).is_finite());
        lolunit_assert(!half(-65536.0f).is_finite());

        lolunit_assert(!half::frombits(0x7c00).is_finite());
        lolunit_assert(!half::frombits(0xfc00).is_finite());

        lolunit_assert(!half::frombits(0x7e00).is_finite());
        lolunit_assert(!half::frombits(0xfe00).is_finite());
    }

    lolunit_declare_test(half_is_normal)
    {
        lolunit_assert(half(0.0f).is_normal());
        lolunit_assert(half(-0.0f).is_normal());
        lolunit_assert(half(largest_normal).is_normal());
        lolunit_assert(half(-largest_normal).is_normal());

        lolunit_assert(!half(65536.0f).is_normal());
        lolunit_assert(!half(-65536.0f).is_normal());

        lolunit_assert(!half::frombits(0x7c00).is_normal());
        lolunit_assert(!half::frombits(0xfc00).is_normal());

        lolunit_assert(!half::frombits(0x7e00).is_normal());
        lolunit_assert(!half::frombits(0xfe00).is_normal());
    }

    lolunit_declare_test(half_classify)
    {
        for (uint32_t i = 0; i < 0x10000; i++)
        {
            lolunit_set_context(i);
            half h = half::frombits(i);
            if (h.is_nan())
            {
                lolunit_assert(!h.is_inf());
                lolunit_assert(!h.is_normal());
                lolunit_assert(!h.is_finite());
            }
            else if (h.is_inf())
            {
                lolunit_assert(!h.is_normal());
                lolunit_assert(!h.is_finite());
            }
            else
            {
                lolunit_assert(h.is_finite());
            }
        }
    }

    lolunit_declare_test(half_to_float)
    {
        for (size_t i = 0; i < sizeof(pairs) / sizeof(*pairs); i++)
        {
            float a = (float)half::frombits(pairs[i].x);
            float b = pairs[i].f;
            lolunit_set_context(i);
            lolunit_assert_equal(a, b);
        }

        for (uint32_t i = 0; i < 0x10000; i++)
        {
            half h = half::frombits(i);
            if (h.is_nan())
                continue;

            float f = (float)h;
            half g = (half)f;
            lolunit_set_context(i);
            lolunit_assert_equal(g, h);
        }
    }

    lolunit_declare_test(half_to_int)
    {
        lolunit_assert_equal((int)(half)(0.0f), 0);
        lolunit_assert_equal((int)(half)(-0.0f), 0);
        lolunit_assert_equal((int)(half)(0.9f), 0);
        lolunit_assert_equal((int)(half)(-0.9f), 0);
        lolunit_assert_equal((int)(half)(1.0f), 1);
        lolunit_assert_equal((int)(half)(-1.0f), -1);
        lolunit_assert_equal((int)(half)(1.9f), 1);
        lolunit_assert_equal((int)(half)(-1.9f), -1);
        lolunit_assert_equal((int)(half)(65504.0f), 65504);
        lolunit_assert_equal((int)(half)(-65504.0f), -65504);
    }

    lolunit_declare_test(float_op_half)
    {
        half const zero(0);
        half const one(1);
        half const two(2);

        float a = zero + one;
        lolunit_assert_equal(1.0f, a);
        a += zero;
        lolunit_assert_equal(1.0f, a);
        a -= zero;
        lolunit_assert_equal(1.0f, a);
        a *= one;
        lolunit_assert_equal(1.0f, a);
        a /= one;
        lolunit_assert_equal(1.0f, a);

        float b = one + zero;
        lolunit_assert_equal(1.0f, b);
        b += one;
        lolunit_assert_equal(2.0f, b);
        b *= two;
        lolunit_assert_equal(4.0f, b);
        b -= two;
        lolunit_assert_equal(2.0f, b);
        b /= two;
        lolunit_assert_equal(1.0f, b);

        float c = one - zero;
        lolunit_assert_equal(1.0f, c);

        float d = two - one;
        lolunit_assert_equal(1.0f, d);

        float e = two + (-one);
        lolunit_assert_equal(1.0f, e);

        float f = (two * two) / (one + one);
        lolunit_assert_equal(2.0f, f);
    }

    lolunit_declare_test(half_op_float)
    {
        half const zero(0);
        half const one(1);
        half const two(2);
        half const four(4);

        half a(one + 0.0f);
        lolunit_assert_equal(one, a);
        a += 0.0f;
        lolunit_assert_equal(one, a);
        a -= 0.0f;
        lolunit_assert_equal(one, a);
        a *= 1.0f;
        lolunit_assert_equal(one, a);
        a /= 1.0f;
        lolunit_assert_equal(one, a);

        half b(one + 0.0f);
        lolunit_assert_equal(one, b);
        b += 1.0f;
        lolunit_assert_equal(two, b);
        b *= 2.0f;
        lolunit_assert_equal(four, b);
        b -= 2.0f;
        lolunit_assert_equal(two, b);
        b /= 2.0f;
        lolunit_assert_equal(one, b);

        half c(1.0f - zero);
        lolunit_assert_equal(one, c);

        half d(2.0f - one);
        lolunit_assert_equal(one, d);

        half e(2.0f + (-one));
        lolunit_assert_equal(one, e);

        half f((2.0f * two) / (1.0f + one));
        lolunit_assert_equal(two, f);
    }

    struct test_pair { float f; uint16_t x; };

    static test_pair const pairs[11];
};

half_test::test_pair const half_test::pairs[] =
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
    { 31.0f / (1 << 18), 0x07c0 }, /* 0x1.fp-14, normal float, denormal half */
    { 31.0f / (1 << 19), 0x03e0 }, /* 0x1.fp-15, normal float, denormal half */
};

} /* namespace lol */

