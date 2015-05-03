//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Camera class
// ----------------
//

#include <lol/math/matrix.h>

#include "engine/worldentity.h"

namespace lol
{

class Camera : public WorldEntity
{
public:
    Camera();
    ~Camera();

    char const *GetName() { return "<camera>"; }

    //View functions
    void SetView(mat4 const &view);
    void SetView(vec3 eye, vec3 target, vec3 up);
    void SetView(vec3 pos, vec3 rot);
    void SetView(vec3 pos, quat rot);

    mat4 GetView() const;

    //Projections functions
//private:
    void SetProjection(mat4 const &proj);
public:
    void SetProjection(float fov, float near, float far);
    void SetProjection(float fov, float near, float far, float screen_size, float screen_ratio);

    mat4 GetProjection() const;

    //Projections manipulation functions
    void SetFov(float fov);
    void SetScreenInfos(float screen_size);
    void SetScreenInfos(float screen_size, float screen_ratio);
    void SetDrawInfos(float far);
    void SetDrawInfos(float near, float far);
    void SetScreenScale(vec2 scale);
    void UseShift(bool should_shift);
    void UseTarget(bool use_target);

    float GetFov() const         { return m_fov; }
    float GetScreenSize() const  { return m_screen_size; }
    float GetScreenRatio() const { return m_screen_ratio; }
    float GetNear() const        { return m_near; }
    float GetFar() const         { return m_far; }
    vec2  GetScreenScale() const { return m_screen_scale; }
    bool  IsShifted() const      { return m_is_shifted; }
    bool  IsTargeting() const    { return (m_target_distance != .0f); }

    //camera manipulation Functions
    void SetPosition(vec3 pos, bool keep_target=false);
    void SetTarget(vec3 target, vec3 up);
    void SetRotation(vec3 rot);
    void SetRotation(quat rot);

    vec3 GetPosition() const;
    vec3 GetTarget() const;
    vec3 GetUp() const;
    vec3 GetRotationEuler() const;
    quat GetRotation() const;

    //Convenience functions
    float GetFrustumHeightAtDistance(float distance, float fov) const;
    float GetFOVForHeightAndDistance(float distance, float height) const;

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds, Scene &scene);

//private:
public:
    //Work datas
    mat4    m_view_matrix;
    mat4    m_proj_matrix;

    //Datas used to create above matrix
    float   m_target_distance;
    float   m_fov;
    float   m_screen_size;
    float   m_screen_ratio;
    float   m_near;
    float   m_far;
    vec2    m_screen_scale;
    bool    m_is_shifted;
    bool    m_fix_up;
};

} /* namespace lol */

