//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Camera class
// ----------------
//

#if !defined __CAMERA_H__
#define __CAMERA_H__

#include "worldentity.h"

namespace lol
{

class Camera : public WorldEntity
{
public:
    Camera(vec3 const &position, vec3 const &target, vec3 const &up);
    ~Camera();

    char const *GetName() { return "<camera>"; }

    void SetPosition(vec3 const &pos);
    void SetRotation(quat const &rot);
    void SetOrtho(float width, float height, float near, float far);
    void SetPerspective(float fov, float width, float height,
                        float near, float far);
    void SetTarget(vec3 const &pos);
    vec3 GetTarget();
    vec3 GetPosition();

    mat4 const &GetViewMatrix();
    mat4 const &GetProjMatrix();
    void ForceSceneUpdate();

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

private:
    mat4 m_view_matrix, m_proj_matrix;
    vec3 m_target, m_up;
};

} /* namespace lol */

#endif /* __CAMERA_H__ */

