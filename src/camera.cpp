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

    /* Create a default perspective */
    SetProjection(mat4::perspective(45.f, 800.f, 600.f, -1000.f, 1000.f));
    SetView(mat4::lookat(vec3(0.f, 50.f, 50.f),
                         vec3(0.f),
                         vec3(0.f, 1.f, 0.f)));
}

Camera::~Camera()
{
}

void Camera::SetView(mat4 const &view)
{
    m_view_matrix = view;
    m_position = inverse(view)[3].xyz;
}

void Camera::SetView(vec3 eye, vec3 target, vec3 up)
{
    m_view_matrix = mat4::lookat(eye, target, up);
    m_position = eye;
}

void Camera::SetView(vec3 pos, quat rot)
{
    m_view_matrix = mat4::lookat(pos,
                                 pos + rot.transform(vec3(0.f, 0.f, -1.f)),
                                 rot.transform(vec3(0.f, 1.f, 0.f)));
    m_position = pos;
}

void Camera::SetProjection(mat4 const &proj)
{
    m_proj_matrix = proj;
}

mat4 Camera::GetView()
{
    return m_view_matrix;
}

mat4 Camera::GetProjection()
{
    return m_proj_matrix;
}

vec3 Camera::GetPosition()
{
    return m_position;
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

