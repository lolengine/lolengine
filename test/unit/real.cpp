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

namespace lol
{

LOLUNIT_FIXTURE(RealTest)
{
    LOLUNIT_TEST(Constants)
    {
        double a0 = real::R_0;
        double a1 = real::R_1;
        double a2 = real::R_2;
        double a10 = real::R_10;

        LOLUNIT_ASSERT_EQUAL(a0, 0.0);
        LOLUNIT_ASSERT_EQUAL(a1, 1.0);
        LOLUNIT_ASSERT_EQUAL(a2, 2.0);
        LOLUNIT_ASSERT_EQUAL(a10, 10.0);

        double b1 = log(real::R_E);
        double b2 = log2(real::R_2);
        LOLUNIT_ASSERT_EQUAL(b1, 1.0);
        LOLUNIT_ASSERT_EQUAL(b2, 1.0);

        double c1 = exp(re(real::R_LOG2E));
        double c2 = log(exp2(real::R_LOG2E));
        LOLUNIT_ASSERT_EQUAL(c1, 2.0);
        LOLUNIT_ASSERT_EQUAL(c2, 1.0);

        double d1 = exp(re(real::R_LOG10E));
        LOLUNIT_ASSERT_EQUAL(d1, 10.0);

        double e1 = exp(real::R_LN2);
        LOLUNIT_ASSERT_EQUAL(e1, 2.0);

        double f1 = exp(real::R_LN10);
        LOLUNIT_ASSERT_EQUAL(f1, 10.0);

        double g1 = sin(real::R_PI);
        double g2 = cos(real::R_PI);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(g1, 0.0, 1e-100);
        LOLUNIT_ASSERT_EQUAL(g2, -1.0);

        double h1 = sin(real::R_PI_2);
        double h2 = cos(real::R_PI_2);
        LOLUNIT_ASSERT_EQUAL(h1, 1.0);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(h2, 0.0, 1e-100);

        double i1 = sin(real::R_PI_4) * sin(real::R_PI_4);
        double i2 = cos(real::R_PI_4) * cos(real::R_PI_4);
        LOLUNIT_ASSERT_EQUAL(i1, 0.5);
        LOLUNIT_ASSERT_EQUAL(i2, 0.5);
    }

    LOLUNIT_TEST(FloatToReal)
    {
        float a1 = real(0.0f);
        float a2 = real(-0.0f);
        float a3 = real(1.0f);
        float a4 = real(-1.0f);
        float a5 = real(1.5f);
        float a6 = real(12345678.0f);

        LOLUNIT_ASSERT_EQUAL(a1, 0.0f);
        LOLUNIT_ASSERT_EQUAL(a2, -0.0f);
        LOLUNIT_ASSERT_EQUAL(a3, 1.0f);
        LOLUNIT_ASSERT_EQUAL(a4, -1.0f);
        LOLUNIT_ASSERT_EQUAL(a5, 1.5f);
        LOLUNIT_ASSERT_EQUAL(a6, 12345678.0f);
    }

    LOLUNIT_TEST(DoubleToReal)
    {
        double a1 = real(0.0);
        double a2 = real(-0.0);
        double a3 = real(1.0);
        double a4 = real(-1.0);
        double a5 = real(1.5);
        double a6 = real(1234567876543210.0);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(a1, 0.0, 0.0);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(a2, -0.0, 0.0);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(a3, 1.0, 0.0);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(a4, -1.0, 0.0);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(a5, 1.5, 0.0);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(a6, 1234567876543210.0, 0.0);
    }

    LOLUNIT_TEST(StringToReal)
    {
        float a1 = real("0");
        float a2 = real("1");
        float a3 = real("-1");
        /* 2^-128 * 2^128 */
        float a4 = real("0.0000000000000000000000000000000000000029387358770"
                        "557187699218413430556141945466638919302188037718792"
                        "6569604314863681793212890625")
                 * real("340282366920938463463374607431768211456");

        LOLUNIT_ASSERT_EQUAL(a1, 0.0f);
        LOLUNIT_ASSERT_EQUAL(a2, 1.0f);
        LOLUNIT_ASSERT_EQUAL(a3, -1.0f);
        LOLUNIT_ASSERT_EQUAL(a4, 1.0f);
    }

    LOLUNIT_TEST(UnaryMinus)
    {
        float a1 = - real(1.0f);
        float a2 = - real(-1.0f);
        float a3 = - real(0.0f);
        float a4 = - real(-0.0f);

        LOLUNIT_ASSERT_EQUAL(a1, -1.0f);
        LOLUNIT_ASSERT_EQUAL(a2, 1.0f);
        LOLUNIT_ASSERT_EQUAL(a3, -0.0f);
        LOLUNIT_ASSERT_EQUAL(a4, 0.0f);
    }

    LOLUNIT_TEST(Comparison)
    {
        LOLUNIT_ASSERT(real(1.0f) > real(0.5f));
        LOLUNIT_ASSERT(real(1.0f) >= real(0.5f));
        LOLUNIT_ASSERT(real(1.0f) >= real(1.0f));

        LOLUNIT_ASSERT(real(-1.0f) < real(-0.5f));
        LOLUNIT_ASSERT(real(-1.0f) <= real(-0.5f));
        LOLUNIT_ASSERT(real(-1.0f) <= real(-1.0f));

        LOLUNIT_ASSERT(real(-1.0f) < real(0.5f));
        LOLUNIT_ASSERT(real(-0.5f) < real(1.0f));
        LOLUNIT_ASSERT(real(-1.0f) <= real(0.5f));
        LOLUNIT_ASSERT(real(-0.5f) <= real(1.0f));

        LOLUNIT_ASSERT(real(1.0f) > real(-0.5f));
        LOLUNIT_ASSERT(real(0.5f) > real(-1.0f));
        LOLUNIT_ASSERT(real(1.0f) >= real(-0.5f));
        LOLUNIT_ASSERT(real(0.5f) >= real(-1.0f));
    }

    LOLUNIT_TEST(Addition)
    {
        float a1 = real(1.0f) + real(0.0f);
        float a2 = real(0.0f) + real(1.0f);
        float a3 = real(1.0f) + real(1.0f);
        float a4 = real(-1.0f) + real(-1.0f);
        float a5 = real(1.0f) + real(0.125f);
        double a6 = real(3.13609818956293918)
                  + real(0.00005972154828114);
        float a7 = real(1.0f) + real(-0.125f);
        double a8 = real(0.10000000002) + real(-2.0e-11);

        LOLUNIT_ASSERT_EQUAL(a1, 1.0f);
        LOLUNIT_ASSERT_EQUAL(a2, 1.0f);
        LOLUNIT_ASSERT_EQUAL(a3, 2.0f);
        LOLUNIT_ASSERT_EQUAL(a4, -2.0f);
        LOLUNIT_ASSERT_EQUAL(a5, 1.125f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(a6, 3.1361579, 0.000001);
        LOLUNIT_ASSERT_EQUAL(a7, 0.875f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(a8, 0.1, 1.0e-13);
    }

    LOLUNIT_TEST(Subtraction)
    {
        float a1 = real(1.0f) + real(1e20f) - real(1e20f);

        LOLUNIT_ASSERT_EQUAL(a1, 1.0f);
    }

    LOLUNIT_TEST(Multiplication)
    {
        real x(1.25f);
        real y(1.5f);
        real z(1.99999f);
        real w(-1.5f);

        float m1 = x * x;
        float m2 = y * y;
        float m3 = z * z;
        float m4 = w * w;

        LOLUNIT_ASSERT_EQUAL(m1, 1.25f * 1.25f);
        LOLUNIT_ASSERT_EQUAL(m2, 1.5f * 1.5f);
        LOLUNIT_ASSERT_EQUAL(m3, 1.99999f * 1.99999f);
        LOLUNIT_ASSERT_EQUAL(m4, -1.5f * -1.5f);
    }

    LOLUNIT_TEST(ExactDivision)
    {
        float m1 = real::R_1 / real::R_1;
        float m2 = real::R_2 / real::R_1;
        float m3 = real::R_1 / real::R_2;
        float m4 = real::R_2 / real::R_2;
        float m5 = real::R_1 / -real::R_2;

        LOLUNIT_ASSERT_EQUAL(m1, 1.0f);
        LOLUNIT_ASSERT_EQUAL(m2, 2.0f);
        LOLUNIT_ASSERT_EQUAL(m3, 0.5f);
        LOLUNIT_ASSERT_EQUAL(m4, 1.0f);
        LOLUNIT_ASSERT_EQUAL(m5, -0.5f);
    }

    LOLUNIT_TEST(InexactDivision)
    {
        /* 1 / 3 * 3 should be close to 1... check that it does not differ
         * by more than 2^-k where k is the number of bits in the mantissa. */
        real a = real::R_1 / real::R_3 * real::R_3;
        real b = ldexp(real::R_1 - a, real::BIGITS * real::BIGIT_BITS);

        LOLUNIT_ASSERT_LEQUAL((double)fabs(b), 1.0);
    }

    LOLUNIT_TEST(LoadExp)
    {
        real a1(1.5);
        real a2(-1.5);
        real a3(0.0);

        LOLUNIT_ASSERT_EQUAL((double)ldexp(a1, 7), 192.0);
        LOLUNIT_ASSERT_EQUAL((double)ldexp(a1, -7), 0.01171875);

        LOLUNIT_ASSERT_EQUAL((double)ldexp(a2, 7), -192.0);
        LOLUNIT_ASSERT_EQUAL((double)ldexp(a2, -7), -0.01171875);

        LOLUNIT_ASSERT_EQUAL((double)ldexp(a3, 7), 0.0);
        LOLUNIT_ASSERT_EQUAL((double)ldexp(a3, -7), 0.0);
    }

    LOLUNIT_TEST(Bool)
    {
        real a = 0.0;
        LOLUNIT_ASSERT(!a);

        a = -0.0;
        LOLUNIT_ASSERT(!a);

        a = 1234.0;
        LOLUNIT_ASSERT(a);
        LOLUNIT_ASSERT(!!a);

        a = -1234.0;
        LOLUNIT_ASSERT(a);
        LOLUNIT_ASSERT(!!a);
    }

    LOLUNIT_TEST(AsinAcos)
    {
        double tests[] =
        {
            -1024.0, -1023.0, -513.0, -512.0, -511.0, -1.0, -0.0,
            0.0, 1.0, 511.0, 512.0, 513.0, 1023.0, 1024.0
        };

        for (unsigned int n = 0; n < sizeof(tests) / sizeof(*tests); n++)
        {
            double a = tests[n] / 1024;
            double b = sin(asin((real)a));
            double c = cos(acos((real)a));

            LOLUNIT_SET_CONTEXT(a);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(b, a, 1e-100);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(c, a, 1e-100);
        }
    }

    LOLUNIT_TEST(FloorCeilEtc)
    {
        double tests[] =
        {
            -2.0,  -2.0, -2.0,  -2.0,
            -1.5,  -2.0, -1.0,  -2.0,
            -1.0,  -1.0, -1.0,  -1.0,
            -0.0,  -0.0, -0.0,  -0.0,
             0.0,   0.0,  0.0,   0.0,
             0.25,  0.0,  1.0,   0.0,
             0.375, 0.0,  1.0,   0.0,
             0.5,   0.0,  1.0,   1.0,
             1.0,   1.0,  1.0,   1.0,
             1.5,   1.0,  2.0,   2.0,
             2.0,   2.0,  2.0,   2.0,
             2.5,   2.0,  3.0,   3.0,
             3.0,   3.0,  3.0,   3.0,
            8192.0,     8192.0, 8192.0, 8192.0,
            8192.03125, 8192.0, 8193.0, 8192.0,
            8192.5,     8192.0, 8193.0, 8193.0,
            8193.0,     8193.0, 8193.0, 8193.0,
            549755813888.0,     549755813888.0, 549755813888.0, 549755813888.0,
            549755813888.03125, 549755813888.0, 549755813889.0, 549755813888.0,
            549755813888.5,     549755813888.0, 549755813889.0, 549755813889.0,
            549755813889.0,     549755813889.0, 549755813889.0, 549755813889.0,
        };

        for (unsigned int n = 0; n < sizeof(tests) / sizeof(*tests); n += 4)
        {
            double a0 = floor((real)tests[n]);
            double b0 = tests[n + 1];
            double a1 = ceil((real)tests[n]);
            double b1 = tests[n + 2];
            double a2 = round((real)tests[n]);
            double b2 = tests[n + 3];

            LOLUNIT_ASSERT_EQUAL(b0, a0);
            LOLUNIT_ASSERT_EQUAL(b1, a1);
            LOLUNIT_ASSERT_EQUAL(b2, a2);
        }
    }

    LOLUNIT_TEST(Pow)
    {
        double a1 = pow(-real::R_2, real::R_2);
        double b1 = 4.0;
        LOLUNIT_ASSERT_DOUBLES_EQUAL(a1, b1, 1.0e-13);

        double a2 = pow(-real::R_2, real::R_3);
        double b2 = -8.0;
        LOLUNIT_ASSERT_DOUBLES_EQUAL(a2, b2, 1.0e-13);
    }
};

} /* namespace lol */

