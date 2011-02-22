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

class MatrixTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(MatrixTest);
    CPPUNIT_TEST(test_mat_det);
    CPPUNIT_TEST(test_mat_mul);
    CPPUNIT_TEST(test_mat_inv);
    CPPUNIT_TEST_SUITE_END();

public:
    MatrixTest() : CppUnit::TestCase("Matrix Test") {}

    void setUp()
    {
        identity = mat4(1.0f);
        triangular = mat4(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                          vec4(7.0f, 2.0f, 0.0f, 0.0f),
                          vec4(1.0f, 5.0f, 3.0f, 0.0f),
                          vec4(8.0f, 9.0f, 2.0f, 4.0f));
        invertible = mat4(vec4( 1.0f,  1.0f,  2.0f, -1.0f),
                          vec4(-2.0f, -1.0f, -2.0f,  2.0f),
                          vec4( 4.0f,  2.0f,  5.0f, -4.0f),
                          vec4( 5.0f, -3.0f, -7.0f, -6.0f));
    }

    void tearDown() {}

    void test_mat_det()
    {
        float d1 = triangular.det();
        CPPUNIT_ASSERT(d1 == 24.0f);
        float d2 = invertible.det();
        CPPUNIT_ASSERT(d2 == -1.0f);
    }

    void test_mat_mul()
    {
        mat4 m0 = identity;
        mat4 m1 = identity;
        mat4 m2 = m0 * m1;

        CPPUNIT_ASSERT(m2[0][0] == 1.0f);
        CPPUNIT_ASSERT(m2[1][0] == 0.0f);
        CPPUNIT_ASSERT(m2[2][0] == 0.0f);
        CPPUNIT_ASSERT(m2[3][0] == 0.0f);

        CPPUNIT_ASSERT(m2[0][1] == 0.0f);
        CPPUNIT_ASSERT(m2[1][1] == 1.0f);
        CPPUNIT_ASSERT(m2[2][1] == 0.0f);
        CPPUNIT_ASSERT(m2[3][1] == 0.0f);

        CPPUNIT_ASSERT(m2[0][2] == 0.0f);
        CPPUNIT_ASSERT(m2[1][2] == 0.0f);
        CPPUNIT_ASSERT(m2[2][2] == 1.0f);
        CPPUNIT_ASSERT(m2[3][2] == 0.0f);

        CPPUNIT_ASSERT(m2[0][3] == 0.0f);
        CPPUNIT_ASSERT(m2[1][3] == 0.0f);
        CPPUNIT_ASSERT(m2[2][3] == 0.0f);
        CPPUNIT_ASSERT(m2[3][3] == 1.0f);
    }

    void test_mat_inv()
    {
        mat4 m0 = invertible;
        mat4 m1 = m0.invert();

        mat4 m2 = m0 * m1;

        CPPUNIT_ASSERT(m2[0][0] == 1.0f);
        CPPUNIT_ASSERT(m2[1][0] == 0.0f);
        CPPUNIT_ASSERT(m2[2][0] == 0.0f);
        CPPUNIT_ASSERT(m2[3][0] == 0.0f);

        CPPUNIT_ASSERT(m2[0][1] == 0.0f);
        CPPUNIT_ASSERT(m2[1][1] == 1.0f);
        CPPUNIT_ASSERT(m2[2][1] == 0.0f);
        CPPUNIT_ASSERT(m2[3][1] == 0.0f);

        CPPUNIT_ASSERT(m2[0][2] == 0.0f);
        CPPUNIT_ASSERT(m2[1][2] == 0.0f);
        CPPUNIT_ASSERT(m2[2][2] == 1.0f);
        CPPUNIT_ASSERT(m2[3][2] == 0.0f);

        CPPUNIT_ASSERT(m2[0][3] == 0.0f);
        CPPUNIT_ASSERT(m2[1][3] == 0.0f);
        CPPUNIT_ASSERT(m2[2][3] == 0.0f);
        CPPUNIT_ASSERT(m2[3][3] == 1.0f);
    }

private:
    mat4 triangular, identity, invertible;
};

CPPUNIT_TEST_SUITE_REGISTRATION(MatrixTest);

} /* namespace lol */

