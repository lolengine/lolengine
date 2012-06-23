//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
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

LOLUNIT_FIXTURE(TrigTest)
{
    LOLUNIT_TEST(Sin)
    {
        using std::fabs;

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 1000.0);
#if defined __GNUC__
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
#if defined __GNUC__
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
#if defined __GNUC__
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
#if defined __GNUC__
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
#if defined __GNUC__
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
#if defined __GNUC__
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
#if defined __GNUC__
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
#if defined __GNUC__
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

