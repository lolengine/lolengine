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
            float f = (float)i * (1.0f / 1000.0f);
            double a = __builtin_sin(f);
            double b = lol_sin(f);
            CPPUNIT_ASSERT(fabs(a - b) <= fabs(f) * 1e-10f);
        }

        for (int i = -10000; i < 10000; i++)
        {
            float f = (float)i * (1.0f / 100000.0f);
            double a = __builtin_sin(f);
            double b = lol_sin(f);
            CPPUNIT_ASSERT(fabs(a - b) <= fabs(f) * 1e-10f);
        }
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TrigTest);

} /* namespace lol */

