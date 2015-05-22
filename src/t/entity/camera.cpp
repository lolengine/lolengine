//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2014 Sam Hocevar <sam@hocevar.net>
//            © 2013 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
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

lolunit_declare_fixture(CameraTest)
{
    Camera  tc;
    vec3    eye;
    vec3    target;
    vec3    up;
    mat4    m_lookat;
    quat    q_lookat;
    vec3    v_lookat;
    float   fov;
    float   screen_size;
    float   screen_ratio;
    float   near;
    float   far;
    bool    is_shifted;

    void SetUp()
    {
        eye = vec3(0.f, 0.f, 50.f);
        target = vec3::zero;
        up = vec3::axis_y;
        m_lookat = mat4::lookat(eye, target, up);
        q_lookat = quat(mat3(m_lookat));
        v_lookat = vec3::toeuler_zyx(q_lookat);
        fov = 90.f;
        screen_size = 800.f;
        screen_ratio = 1.0f;
        near = 1.f;
        far = 1000.f;
        is_shifted = false;
    }

    void TearDown() {}

#define TEST_VECTOR(v0, v1) \
        lolunit_assert_doubles_equal(v0.x, v1.x, 1.e-5f); \
        lolunit_assert_doubles_equal(v0.y, v1.y, 1.e-5f); \
        lolunit_assert_doubles_equal(v0.z, v1.z, 1.e-5f);

    lolunit_declare_test(SetViewTest)
    {
        tc.SetView(eye, target, up);
        TEST_VECTOR(eye, tc.GetPosition());
        TEST_VECTOR(target, tc.GetTarget());
        TEST_VECTOR(up, tc.GetUp());

        tc.SetView(eye, q_lookat);
        TEST_VECTOR(eye, tc.GetPosition());
        TEST_VECTOR(target, tc.GetTarget());
        TEST_VECTOR(up, tc.GetUp());

        tc.SetView(eye, v_lookat);
        TEST_VECTOR(eye, tc.GetPosition());
        TEST_VECTOR(target, tc.GetTarget());
        TEST_VECTOR(up, tc.GetUp());

        tc.SetView(m_lookat);
        TEST_VECTOR(eye, tc.GetPosition());
        TEST_VECTOR(target, tc.GetTarget());
        TEST_VECTOR(up, tc.GetUp());

        tc.UseTarget(false);
        TEST_VECTOR(vec3(0.f, 0.f, 49.f), tc.GetTarget());
    }

#define TEST_MATRIX(m0, m1) \
        lolunit_assert_doubles_equal(m0[0][0], m1[0][0], 1.e-5f); \
        lolunit_assert_doubles_equal(m0[1][0], m1[1][0], 1.e-5f); \
        lolunit_assert_doubles_equal(m0[2][0], m1[2][0], 1.e-5f); \
        lolunit_assert_doubles_equal(m0[3][0], m1[3][0], 1.e-5f); \
                                                                  \
        lolunit_assert_doubles_equal(m0[0][1], m1[0][1], 1.e-5f); \
        lolunit_assert_doubles_equal(m0[1][1], m1[1][1], 1.e-5f); \
        lolunit_assert_doubles_equal(m0[2][1], m1[2][1], 1.e-5f); \
        lolunit_assert_doubles_equal(m0[3][1], m1[3][1], 1.e-5f); \
                                                                  \
        lolunit_assert_doubles_equal(m0[0][2], m1[0][2], 1.e-5f); \
        lolunit_assert_doubles_equal(m0[1][2], m1[1][2], 1.e-5f); \
        lolunit_assert_doubles_equal(m0[2][2], m1[2][2], 1.e-5f); \
        lolunit_assert_doubles_equal(m0[3][2], m1[3][2], 1.e-5f); \
                                                                  \
        lolunit_assert_doubles_equal(m0[0][3], m1[0][3], 1.e-5f); \
        lolunit_assert_doubles_equal(m0[1][3], m1[1][3], 1.e-5f); \
        lolunit_assert_doubles_equal(m0[2][3], m1[2][3], 1.e-5f); \
        lolunit_assert_doubles_equal(m0[3][3], m1[3][3], 1.e-5f);


    lolunit_declare_test(SetProjectionTest)
    {
        mat4 refmx = mat4::perspective(fov, screen_size, screen_size * screen_ratio, near, far);

        tc.SetProjection(fov, near, far, screen_size, screen_ratio);
        TEST_MATRIX(refmx, tc.GetProjection());
        lolunit_assert_doubles_equal(fov, tc.GetFov(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_size, tc.GetScreenSize(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_ratio, tc.GetScreenRatio(), 1.e-5f);
        lolunit_assert_doubles_equal(near, tc.GetNear(), 1.e-5f);
        lolunit_assert_doubles_equal(far, tc.GetFar(), 1.e-5f);
        lolunit_assert(is_shifted == tc.IsShifted());

        tc.SetProjection(fov, near, far);
        TEST_MATRIX(refmx, tc.GetProjection());
        lolunit_assert_doubles_equal(fov, tc.GetFov(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_size, tc.GetScreenSize(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_ratio, tc.GetScreenRatio(), 1.e-5f);
        lolunit_assert_doubles_equal(near, tc.GetNear(), 1.e-5f);
        lolunit_assert_doubles_equal(far, tc.GetFar(), 1.e-5f);
        lolunit_assert(is_shifted == tc.IsShifted());

        tc.SetProjection(refmx);
        TEST_MATRIX(refmx, tc.GetProjection());

        tc.SetFov(fov);
        TEST_MATRIX(refmx, tc.GetProjection());
        lolunit_assert_doubles_equal(fov, tc.GetFov(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_size, tc.GetScreenSize(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_ratio, tc.GetScreenRatio(), 1.e-5f);
        lolunit_assert_doubles_equal(near, tc.GetNear(), 1.e-5f);
        lolunit_assert_doubles_equal(far, tc.GetFar(), 1.e-5f);
        lolunit_assert(is_shifted == tc.IsShifted());

        tc.SetScreenInfos(screen_size);
        TEST_MATRIX(refmx, tc.GetProjection());
        lolunit_assert_doubles_equal(fov, tc.GetFov(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_size, tc.GetScreenSize(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_ratio, tc.GetScreenRatio(), 1.e-5f);
        lolunit_assert_doubles_equal(near, tc.GetNear(), 1.e-5f);
        lolunit_assert_doubles_equal(far, tc.GetFar(), 1.e-5f);
        lolunit_assert(is_shifted == tc.IsShifted());

        tc.SetScreenInfos(screen_size, screen_ratio);
        TEST_MATRIX(refmx, tc.GetProjection());
        lolunit_assert_doubles_equal(fov, tc.GetFov(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_size, tc.GetScreenSize(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_ratio, tc.GetScreenRatio(), 1.e-5f);
        lolunit_assert_doubles_equal(near, tc.GetNear(), 1.e-5f);
        lolunit_assert_doubles_equal(far, tc.GetFar(), 1.e-5f);
        lolunit_assert(is_shifted == tc.IsShifted());

        tc.SetDrawInfos(far);
        TEST_MATRIX(refmx, tc.GetProjection());
        lolunit_assert_doubles_equal(fov, tc.GetFov(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_size, tc.GetScreenSize(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_ratio, tc.GetScreenRatio(), 1.e-5f);
        lolunit_assert_doubles_equal(near, tc.GetNear(), 1.e-5f);
        lolunit_assert_doubles_equal(far, tc.GetFar(), 1.e-5f);
        lolunit_assert(is_shifted == tc.IsShifted());

        tc.SetDrawInfos(near, far);
        TEST_MATRIX(refmx, tc.GetProjection());
        lolunit_assert_doubles_equal(fov, tc.GetFov(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_size, tc.GetScreenSize(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_ratio, tc.GetScreenRatio(), 1.e-5f);
        lolunit_assert_doubles_equal(near, tc.GetNear(), 1.e-5f);
        lolunit_assert_doubles_equal(far, tc.GetFar(), 1.e-5f);
        lolunit_assert(is_shifted == tc.IsShifted());

        is_shifted = true;
        refmx = mat4::shifted_perspective(fov, screen_size, screen_ratio, near, far);

        tc.UseShift(is_shifted);
        TEST_MATRIX(refmx, tc.GetProjection());
        lolunit_assert_doubles_equal(fov, tc.GetFov(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_size, tc.GetScreenSize(), 1.e-5f);
        lolunit_assert_doubles_equal(screen_ratio, tc.GetScreenRatio(), 1.e-5f);
        lolunit_assert_doubles_equal(near, tc.GetNear(), 1.e-5f);
        lolunit_assert_doubles_equal(far, tc.GetFar(), 1.e-5f);
        lolunit_assert(is_shifted == tc.IsShifted());
    }
};

} /* namespace lol */

