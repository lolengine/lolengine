//
//  Lol Engine — Unit tests for the SQT transform class
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

static sqt const test_sqt_1(1.5f,
                            quat::rotate(1.1f,
                                         normalize(vec3(1.f, 2.f, 3.f))),
                            vec3(1.f, -1.f, 0.5f));

static vec4 test_vec4s[] =
{
    vec4(1.f, 0.f, 0.f, 1.f),
    vec4(0.f, 1.f, 0.f, 1.f),
    vec4(0.f, 0.f, 1.f, 1.f),
    vec4(1.f, 2.f, 3.f, 1.f),
    vec4(-1.f, 50.f, 12.f, 1.f),
    vec4(20.f, -10.f, 0.f, 1.f),
    vec4(-20.f, 10.f, 8.f, 1.f),
};

lolunit_declare_fixture(sqt_test)
{
    lolunit_declare_test(transform_vec3_vec4)
    {
        /* We check that transforming a vec3 and a vec4 with w==1
         * yield the same results. */
        for (vec4 v0 : test_vec4s)
        {
            vec4 v1 = test_sqt_1.transform(v0);
            vec3 v2 = test_sqt_1.transform(v0.xyz);

            lolunit_assert_doubles_equal(v1.x, v2.x, length(v0.xyz) * 1e-5f);
            lolunit_assert_doubles_equal(v1.y, v2.y, length(v0.xyz) * 1e-5f);
            lolunit_assert_doubles_equal(v1.z, v2.z, length(v0.xyz) * 1e-5f);
            lolunit_assert_doubles_equal(v1.w, 1.0f, 1e-5f);
        }
    }

    lolunit_declare_test(sqt_vs_matrix_vec4)
    {
        /* We check that transforming a vec4 with an SQT and a
         * vec4 with a product of 4×4 matrices yield the same
         * results. */
        mat4 m = mat4::translate(test_sqt_1.t)
               * mat4(test_sqt_1.q)
               * mat4::scale(test_sqt_1.s);

        for (vec4 v0 : test_vec4s)
        {
            vec4 v1 = test_sqt_1 * v0;
            vec4 v2 = m * v0;

            lolunit_assert_doubles_equal(v1.x, v2.x, length(v0.xyz) * 1e-5f);
            lolunit_assert_doubles_equal(v1.y, v2.y, length(v0.xyz) * 1e-5f);
            lolunit_assert_doubles_equal(v1.z, v2.z, length(v0.xyz) * 1e-5f);
            lolunit_assert_doubles_equal(v1.w, v2.w, 1e-5f);
        }
    }

    lolunit_declare_test(sqt_inverse)
    {
        for (vec4 v0 : test_vec4s)
        {
            vec4 v1 = inverse(test_sqt_1) * (test_sqt_1 * v0);

            lolunit_assert_doubles_equal(v0.x, v1.x, length(v0.xyz) * 1e-5f);
            lolunit_assert_doubles_equal(v0.y, v1.y, length(v0.xyz) * 1e-5f);
            lolunit_assert_doubles_equal(v0.z, v1.z, length(v0.xyz) * 1e-5f);
            lolunit_assert_doubles_equal(v0.w, v1.w, 1e-5f);
        }
    }

    lolunit_declare_test(sqt_composition_vec4)
    {
        /* We check that transforming a vec4 with an SQT and a
         * second SQT yields the same result as transforming a
         * vec4 with the product of the two SQTs. */
        for (vec4 v0 : test_vec4s)
        {
            vec4 v1 = (test_sqt_1 * test_sqt_1) * v0;
            vec4 v2 = test_sqt_1 * (test_sqt_1 * v0);

            lolunit_assert_doubles_equal(v1.x, v2.x, length(v0.xyz) * 1e-5f);
            lolunit_assert_doubles_equal(v1.y, v2.y, length(v0.xyz) * 1e-5f);
            lolunit_assert_doubles_equal(v1.z, v2.z, length(v0.xyz) * 1e-5f);
            lolunit_assert_doubles_equal(v1.w, v2.w, 1e-5f);
        }
    }
};

} /* namespace lol */

