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

#include "core.h"

namespace lol
{

class HalfTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(HalfTest);
    CPPUNIT_TEST(test_half_makebits);
    CPPUNIT_TEST(test_half_makeslow);
    CPPUNIT_TEST(test_half_makefast);
    CPPUNIT_TEST(test_half_to_float);
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
            CPPUNIT_ASSERT_EQUAL(a.bits(), b);
        }
    }

    void test_half_makeslow()
    {
        for (size_t i = 0; i < 8; i++)
        {
            half a = half::makeslow(array1[i].f);
            uint16_t b = array1[i].x;
            CPPUNIT_ASSERT_EQUAL(a.bits(), b);
        }
    }

    void test_half_makefast()
    {
        for (size_t i = 0; i < 8; i++)
        {
            half a = half::makefast(array1[i].f);
            uint16_t b = array1[i].x;
            CPPUNIT_ASSERT_EQUAL(a.bits(), b);
        }
    }

    void test_half_to_float()
    {
#if 0
        for (unsigned int i = 0; i < 0x10000; i++)
        {
            half a = half::makebits(i);
            half b = half::makefast((float)a);
            //CPPUNIT_ASSERT_EQUAL(a.bits(), b.bits());
        }
    }
#endif
    }

private:
    struct TestPair { float f; uint16_t x; };

    static TestPair const array1[];
};

HalfTest::TestPair const HalfTest::array1[] =
{
    { 0.0f,     0x0000 },
    { -0.0f,    0x8000 },
    { 1.0f,     0x3c00 },
    { -1.0f,    0xbc00 },
    { 2.0f,     0x4000 },
    { 0.5f,     0x3800 },
    { 0.125f,   0x3000 },
    { 15.9375f, 0x4bf8 },
};

CPPUNIT_TEST_SUITE_REGISTRATION(HalfTest);

} /* namespace lol */

