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

namespace lol
{

class TrigTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(TrigTest);
    CPPUNIT_TEST(test_sin);
    CPPUNIT_TEST_SUITE_END();

public:
    TrigTest() : CppUnit::TestCase("Trigonometry Test") {}

    void setUp() {}

    void tearDown() {}

    void test_sin()
    {
        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 1000.0);
            double a = __builtin_sin(f);
            double b = lol_sin(f);
            CPPUNIT_ASSERT(fabs(a - b) <= fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 100000.0);
            double a = __builtin_sin(f);
            double b = lol_sin(f);
            CPPUNIT_ASSERT(fabs(a - b) <= fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 1000.0);
            double a = __builtin_cos(f);
            double b = lol_cos(f);
            CPPUNIT_ASSERT(fabs(a - b) <= fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 100000.0);
            double a = __builtin_cos(f);
            double b = lol_cos(f);
            CPPUNIT_ASSERT(fabs(a - b) <= fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 1000.0);
            double a1 = __builtin_sin(f);
            double a2 = __builtin_cos(f);
            double b1, b2;
            lol_sincos(f, &b1, &b2);
            CPPUNIT_ASSERT(fabs(a1 - b1) <= fabs(f) * 1e-11);
            CPPUNIT_ASSERT(fabs(a2 - b2) <= fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 100000.0);
            double a1 = __builtin_sin(f);
            double a2 = __builtin_cos(f);
            double b1, b2;
            lol_sincos(f, &b1, &b2);
            CPPUNIT_ASSERT(fabs(a1 - b1) <= fabs(f) * 1e-11);
            CPPUNIT_ASSERT(fabs(a2 - b2) <= fabs(f) * 1e-11);
        }

        for (int i = -100000; i < 100000; i++)
        {
            double f = (double)i * (1.0 / 10000.0);
            double a = __builtin_tan(f);
            double b = lol_tan(f);
            if (fabs(a) > 1e4)
               CPPUNIT_ASSERT(fabs(a - b) <= fabs(a) * fabs(a) * 1e-11);
            else if (fabs(a) > 1.0)
               CPPUNIT_ASSERT(fabs(a - b) <= fabs(a) * 1e-11);
            else
               CPPUNIT_ASSERT(fabs(a - b) <= fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 100000.0);
            double a = __builtin_tan(f);
            double b = lol_tan(f);
            if (fabs(a) > 1e4)
               CPPUNIT_ASSERT(fabs(a - b) <= fabs(a) * fabs(a) * 1e-11);
            else if (fabs(a) > 1.0)
               CPPUNIT_ASSERT(fabs(a - b) <= fabs(a) * 1e-11);
            else
               CPPUNIT_ASSERT(fabs(a - b) <= fabs(f) * 1e-11);
        }
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TrigTest);

} /* namespace lol */

