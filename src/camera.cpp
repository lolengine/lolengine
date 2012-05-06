//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstring>
#include <cstdlib>

#include "core.h"

#if defined _WIN32 || defined _XBOX
#   define strcasecmp _stricmp
#endif

namespace lol
{

Camera::Camera(vec3 const &position, vec3 const &target, vec3 const &up)
  : m_target(target),
    m_up(up)
{
    m_gamegroup = GAMEGROUP_BEFORE;
    m_drawgroup = DRAWGROUP_CAMERA;

    SetPosition(position);
}

Camera::~Camera()
{
}

void Camera::SetPosition(vec3 const &pos)
{
    m_position = pos;
}

mat4 const &Camera::GetViewMatrix()
{
    return m_view_matrix;
}

mat4 const &Camera::GetProjMatrix()
{
    return m_proj_matrix;
}

void Camera::TickGame(float seconds)
{
    WorldEntity::TickGame(seconds);

    /* Hackish keyboard support */
    float updown = 2.f * (Input::GetButtonState('w')
                           + Input::GetButtonState('z')
                           - Input::GetButtonState('s'));
    float rightleft = 2.f * (Input::GetButtonState('d')
                              - Input::GetButtonState('q')
                              - Input::GetButtonState('a'));
    float pgupdown = 2.f * (Input::GetButtonState('r')
                             - Input::GetButtonState('f'));

    /* Hackish stick support */
    static Stick *stick = NULL;
    if (!stick)
        stick = Input::TrackStick();
    if (stick && stick->GetAxisCount() >= 2)
    {
        rightleft += 2.f * stick->GetAxis(0) * std::abs(stick->GetAxis(0));
        updown += -2.f * stick->GetAxis(1) * std::abs(stick->GetAxis(1));
    }

    m_position += vec3(rightleft, pgupdown, -updown) * 200.f * seconds;
    m_target += vec3(rightleft, 0, -updown) * 200.f * seconds;

    m_view_matrix = mat4::lookat(m_position, m_target, m_up);
    m_proj_matrix = mat4::perspective(45.0f, 640.0f, 480.0f, 1.f, 10000.0f);
    //m_proj_matrix = mat4::ortho(-160, 160, -120, 120, .1f, 2000.0f);
}

void Camera::TickDraw(float seconds)
{
    WorldEntity::TickDraw(seconds);

    Scene::GetDefault()->SetViewMatrix(m_view_matrix);
    Scene::GetDefault()->SetProjMatrix(m_proj_matrix);
}

} /* namespace lol */

