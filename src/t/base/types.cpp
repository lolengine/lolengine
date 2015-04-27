//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2014 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(BuildTest)
{
    lolunit_declare_test(TypeSizeHalf)
    {
        lolunit_assert_equal(sizeof(half), 2);

#if LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
        lolunit_assert_equal(sizeof(f16vec2), 4);
        lolunit_assert_equal(sizeof(f16vec3), 6);
        lolunit_assert_equal(sizeof(f16vec4), 8);

        lolunit_assert_equal(sizeof(f16mat2), 8);
        lolunit_assert_equal(sizeof(f16mat3), 18);
        lolunit_assert_equal(sizeof(f16mat4), 32);
#endif
    }

    lolunit_declare_test(TypeSizeFloat)
    {
        lolunit_assert_equal(sizeof(float), 4);

        lolunit_assert_equal(sizeof(vec2), 8);
        lolunit_assert_equal(sizeof(vec3), 12);
        lolunit_assert_equal(sizeof(vec4), 16);

        lolunit_assert_equal(sizeof(mat2), 16);
        lolunit_assert_equal(sizeof(mat3), 36);
        lolunit_assert_equal(sizeof(mat4), 64);
    }

    lolunit_declare_test(TypeSizeDouble)
    {
        lolunit_assert_equal(sizeof(double), 8);

        lolunit_assert_equal(sizeof(dvec2), 16);
        lolunit_assert_equal(sizeof(dvec3), 24);
        lolunit_assert_equal(sizeof(dvec4), 32);

        lolunit_assert_equal(sizeof(dmat2), 32);
        lolunit_assert_equal(sizeof(dmat3), 72);
        lolunit_assert_equal(sizeof(dmat4), 128);
    }

    lolunit_declare_test(TypeSizeInt8)
    {
        lolunit_assert_equal(sizeof(i8vec2), 2);
        lolunit_assert_equal(sizeof(u8vec2), 2);
        lolunit_assert_equal(sizeof(i8vec3), 3);
        lolunit_assert_equal(sizeof(u8vec3), 3);
        lolunit_assert_equal(sizeof(i8vec4), 4);
        lolunit_assert_equal(sizeof(u8vec4), 4);
    }

    lolunit_declare_test(TypeSizeInt16)
    {
        lolunit_assert_equal(sizeof(i16vec2), 4);
        lolunit_assert_equal(sizeof(u16vec2), 4);
        lolunit_assert_equal(sizeof(i16vec3), 6);
        lolunit_assert_equal(sizeof(u16vec3), 6);
        lolunit_assert_equal(sizeof(i16vec4), 8);
        lolunit_assert_equal(sizeof(u16vec4), 8);
    }

    lolunit_declare_test(TypeSizeInt32)
    {
        lolunit_assert_equal(sizeof(ivec2), 8);
        lolunit_assert_equal(sizeof(uvec2), 8);
        lolunit_assert_equal(sizeof(ivec3), 12);
        lolunit_assert_equal(sizeof(uvec3), 12);
        lolunit_assert_equal(sizeof(ivec4), 16);
        lolunit_assert_equal(sizeof(uvec4), 16);
    }

    lolunit_declare_test(TypeSizeInt64)
    {
        lolunit_assert_equal(sizeof(i64vec2), 16);
        lolunit_assert_equal(sizeof(u64vec2), 16);
        lolunit_assert_equal(sizeof(i64vec3), 24);
        lolunit_assert_equal(sizeof(u64vec3), 24);
        lolunit_assert_equal(sizeof(i64vec4), 32);
        lolunit_assert_equal(sizeof(u64vec4), 32);
    }

#if !defined LOL_BUILD_DEBUG
    lolunit_declare_test(FastMath)
    {
        double x, y;

        y = x = 1.0 + rand(0.1f, 0.2f);
        y += 4503599627370496.0;
        /* The compiler should optimise this away */
        y -= 4503599627370496.0;

        lolunit_assert_equal(x, y);
    }
#endif
};

} /* namespace lol */

