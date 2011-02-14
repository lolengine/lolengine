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

class MatrixTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(MatrixTest);
    CPPUNIT_TEST(test_mat_mul);
    CPPUNIT_TEST_SUITE_END();

public:
    MatrixTest() : CppUnit::TestCase("Matrix Test") {}

    void setUp() {}

    void tearDown() {}

    void test_mat_mul()
    {
        float4 v0(1.0f, 0.0f, 0.0f, 0.0f);
        float4 v1(0.0f, 1.0f, 0.0f, 0.0f);
        float4 v2(0.0f, 0.0f, 1.0f, 0.0f);
        float4 v3(0.0f, 0.0f, 0.0f, 1.0f);
        float4x4 m0(v0, v1, v2, v3);
        float4x4 m1(v0, v1, v2, v3);
        float4x4 m2 = m0 * m1;

        CPPUNIT_ASSERT(m2[0][0] == 1.0f);
        CPPUNIT_ASSERT(m2[1][1] == 1.0f);
        CPPUNIT_ASSERT(m2[2][2] == 1.0f);
        CPPUNIT_ASSERT(m2[3][3] == 1.0f);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(MatrixTest);

