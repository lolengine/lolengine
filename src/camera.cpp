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

#include <cstring>
#include <cstdlib>

#include "core.h"

#if defined _WIN32 || defined _XBOX
#   define strcasecmp _stricmp
#   undef near
#   undef far
#endif

namespace lol
{

Camera::Camera()
{
    m_gamegroup = GAMEGROUP_BEFORE;
    m_drawgroup = DRAWGROUP_CAMERA;

    ivec2 screen_size = (g_renderer)?(Video::GetSize()):(ivec2(0));
    m_fov = 45.f;
    m_near = -1000.f;
    m_far = 1000.f;
    m_screen_size = (float)screen_size.x;
    m_screen_ratio = (float)screen_size.x / (float)screen_size.y;
    m_is_shifted = false;

    /* Create a default perspective */
    SetProjection(mat4::perspective(45.f, 800.f, 600.f, -1000.f, 1000.f));
    SetView(mat4::lookat(vec3(0.f, 50.f, 50.f),
                         vec3(0.f),
                         vec3(0.f, 1.f, 0.f)));
}

Camera::~Camera()
{
}

//-----------------------------------------------------------------------------
//View functions
//--
void Camera::SetView(mat4 const &view)
{
    m_view_matrix = view;
    m_position = inverse(view)[3].xyz;
}

void Camera::SetView(vec3 eye, vec3 target, vec3 up)
{
    m_view_matrix = mat4::lookat(eye, target, up);
    m_position = eye;
    m_target_distance = length(target - m_position);
}

void Camera::SetView(vec3 pos, vec3 rot)
{
    SetView(pos, quat::fromeuler_xyz(rot));
}

void Camera::SetView(vec3 pos, quat rot)
{
    m_view_matrix = mat4::lookat(pos,
                                 pos + rot.transform(vec3(0.f, 0.f, -1.f)),
                                 rot.transform(vec3(0.f, 1.f, 0.f)));
    m_position = pos;
    m_target_distance = .0f;
}

mat4 Camera::GetView()
{
    return m_view_matrix;
}

//-----------------------------------------------------------------------------
//Projections functions
//--
void Camera::SetProjection(mat4 const &proj)
{
    m_proj_matrix = proj;
}

void Camera::SetProjection(float fov, float near, float far)
{
    SetProjection(fov, near, far, m_screen_size, m_screen_ratio);
}

void Camera::SetProjection(float fov, float near, float far, float screen_size, float screen_ratio)
{
    m_fov = fov;
    m_near = near;
    m_far = far;
    m_screen_size = screen_size;
    m_screen_ratio = screen_ratio;

    if (m_fov > .0f)
    {
        if (m_is_shifted)
            SetProjection(mat4::shifted_perspective(m_fov, screen_size, screen_ratio, m_far - m_near));
        else
            SetProjection(mat4::perspective(m_fov, screen_size, screen_size * screen_ratio, m_near, m_far));
    }
    else
        SetProjection(mat4::ortho(screen_size, screen_size * screen_ratio, m_near, m_far));
}

mat4 Camera::GetProjection()
{
    return m_proj_matrix;
}

//-----------------------------------------------------------------------------
//Projections manipulation functions
//--
void Camera::SetFov(float fov)
{
    SetProjection(fov, m_near, m_far, m_screen_size, m_screen_ratio);
}

void Camera::SetScreenInfos(float screen_size)
{
    SetScreenInfos(screen_size, m_screen_ratio);
}

void Camera::SetScreenInfos(float screen_size, float screen_ratio)
{
    SetProjection(m_fov, m_near, m_far, screen_size, screen_ratio);
}

void Camera::SetDrawInfos(float far)
{
    SetDrawInfos(m_near, far);
}

void Camera::SetDrawInfos(float near, float far)
{
    SetProjection(m_fov, near, far, m_screen_size, m_screen_ratio);
}

void Camera::UseShift(bool should_shift)
{
    m_is_shifted = should_shift;
    SetProjection(m_fov, m_near, m_far, m_screen_size, m_screen_ratio);
}


//-----------------------------------------------------------------------------
//camera manipulation Functions
//--
void Camera::SetPosition(vec3 pos)
{
    if (m_target_distance > .0f)
    {
        vec4 up = m_view_matrix * vec4(0.f, 1.f, 0.f, 0.f);
        vec4 target = m_view_matrix * vec4(0.f, 0.f, -m_target_distance, 0.f);
        SetView(m_position, m_position + target.xyz, up.xyz);
    }
    else
        m_view_matrix = m_view_matrix * mat4::translate(pos - m_position);
    m_position = pos;
}

void Camera::SetTarget(vec3 target)
{
    vec4 up = inverse(m_view_matrix) * vec4(0.f, 1.f, 0.f, 0.f);
    SetView(m_position, target, up.xyz);
}

void Camera::SetUp(vec3 up)
{
    vec4 target = inverse(m_view_matrix) * vec4(0.f, 0.f, -max(m_target_distance, 1.f), 0.f);
    SetView(m_position, m_position + target.xyz, up);
}

void Camera::SetRotation(vec3 rot)
{
    SetRotation(quat::fromeuler_xyz(rot));
}

void Camera::SetRotation(quat rot)
{
    SetView(m_position, rot);
}

//--
vec3 Camera::GetPosition()
{
    return m_position;
}

vec3 Camera::GetTarget()
{
    return m_position + (inverse(m_view_matrix) * vec4(0.f, 0.f, -max(m_target_distance, 1.f), 0.f)).xyz;
}

vec3 Camera::GetUp()
{
    return (inverse(m_view_matrix) * vec4(0.f, 1.f, 0.f, 0.f)).xyz;
}

vec3 Camera::GetRotationEuler()
{
    return vec3::toeuler(GetRotation());
}

quat Camera::GetRotation()
{
    return quat(inverse(m_view_matrix));
}

// Calculate the frustum height at a given distance from the camera.
float Camera::GetFrustumHeightAtDistance(float distance, float fov)
{
    return 2.f * distance * lol::tan(fov * .5f * (F_PI / 180.f));
}

// Calculate the FOV needed to get a given frustum height at a given distance.
float Camera::GetFOVForHeightAndDistance(float distance, float height)
{
    return 2.f * lol::atan(height * .5f / distance) * (180.f / F_PI);
}

void Camera::TickGame(float seconds)
{
    WorldEntity::TickGame(seconds);
}

void Camera::TickDraw(float seconds)
{
    WorldEntity::TickDraw(seconds);
}

} /* namespace lol */

