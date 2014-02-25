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

#include <cmath>

#include "core.h"
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(TrigTest)
{
    LOLUNIT_TEST(AngleConversions)
    {
        LOLUNIT_ASSERT_DOUBLES_EQUAL(D_PI, radians(180.0), 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(D_PI_2, radians(90.0), 1e-5);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(F_PI, radians(180.0f), 1e-5f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(F_PI_2, radians(90.0f), 1e-5f);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(180.0, degrees(D_PI), 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(90.0, degrees(D_PI_2), 1e-5);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(180.0f, degrees(F_PI), 1e-5f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(90.0f, degrees(F_PI_2), 1e-5f);
    }

    LOLUNIT_TEST(Sin)
    {
        using std::fabs;

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 1000.0);
#if defined __GNUC__ && !defined __SNC__
            double a = __builtin_sin(f);
#else
            double a = std::sin(f);
#endif
            double b = lol_sin(f);
            LOLUNIT_SET_CONTEXT(f);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 100000.0);
#if defined __GNUC__ && !defined __SNC__
            double a = __builtin_sin(f);
#else
            double a = std::sin(f);
#endif
            double b = lol_sin(f);
            LOLUNIT_SET_CONTEXT(f);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, fabs(f) * 1e-11);
        }
    }

    LOLUNIT_TEST(Cos)
    {
        using std::fabs;

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 1000.0);
#if defined __GNUC__ && !defined __SNC__
            double a = __builtin_cos(f);
#else
            double a = std::cos(f);
#endif
            double b = lol_cos(f);
            LOLUNIT_SET_CONTEXT(f);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 100000.0);
#if defined __GNUC__ && !defined __SNC__
            double a = __builtin_cos(f);
#else
            double a = std::cos(f);
#endif
            double b = lol_cos(f);
            LOLUNIT_SET_CONTEXT(f);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, fabs(f) * 1e-11);
        }
    }

    LOLUNIT_TEST(SinCos)
    {
        using std::fabs;

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 1000.0);
#if defined __GNUC__ && !defined __SNC__
            double a1 = __builtin_sin(f);
            double a2 = __builtin_cos(f);
#else
            double a1 = std::sin(f);
            double a2 = std::cos(f);
#endif
            double b1, b2;
            lol_sincos(f, &b1, &b2);
            LOLUNIT_SET_CONTEXT(f);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(a1, b1, fabs(f) * 1e-11);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(a2, b2, fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 100000.0);
#if defined __GNUC__ && !defined __SNC__
            double a1 = __builtin_sin(f);
            double a2 = __builtin_cos(f);
#else
            double a1 = std::sin(f);
            double a2 = std::cos(f);
#endif
            double b1, b2;
            lol_sincos(f, &b1, &b2);
            LOLUNIT_SET_CONTEXT(f);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(a1, b1, fabs(f) * 1e-11);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(a2, b2, fabs(f) * 1e-11);
        }
    }

    LOLUNIT_TEST(Tan)
    {
        using std::fabs;

        for (int i = -100000; i < 100000; i++)
        {
            double f = (double)i * (1.0 / 10000.0);
#if defined __GNUC__ && !defined __SNC__
            double a = __builtin_tan(f);
#else
            double a = std::tan(f);
#endif
            double b = lol_tan(f);
            LOLUNIT_SET_CONTEXT(f);
            if (fabs(a) > 1e4)
                LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, fabs(a) * fabs(a) * 1e-11);
            else if (fabs(a) > 1.0)
                LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, fabs(a) * 1e-11);
            else
                LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 100000.0);
#if defined __GNUC__ && !defined __SNC__
            double a = __builtin_tan(f);
#else
            double a = std::tan(f);
#endif
            double b = lol_tan(f);
            LOLUNIT_SET_CONTEXT(f);
            if (fabs(a) > 1e4)
                LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, fabs(a) * fabs(a) * 1e-11);
            else if (fabs(a) > 1.0)
                LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, fabs(a) * 1e-11);
            else
                LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, fabs(f) * 1e-11);
        }
    }
};

} /* namespace lol */

