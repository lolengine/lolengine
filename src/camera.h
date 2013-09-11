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
    Camera();
    ~Camera();

    char const *GetName() { return "<camera>"; }

    //View functions
    void SetView(mat4 const &view);
    void SetView(vec3 eye, vec3 target, vec3 up);
    void SetView(vec3 pos, vec3 rot);
    void SetView(vec3 pos, quat rot);

    mat4 GetView();

    //Projections functions
//private:
    void SetProjection(mat4 const &proj);
public:
    void SetProjection(float fov, float near, float far);
    void SetProjection(float fov, float near, float far, float screen_size, float screen_ratio);

    mat4 GetProjection();

    //Projections manipulation functions
    void SetFov(float fov);
    void SetScreenInfos(float screen_size);
    void SetScreenInfos(float screen_size, float screen_ratio);
    void SetDrawInfos(float far);
    void SetDrawInfos(float near, float far);
    void UseShift(bool should_shift);

    float GetFov()          { return m_fov; }
    float GetScreenSize()   { return m_screen_size; }
    float GetScreenRatio()  { return m_screen_ratio; }
    float GetNear()         { return m_near; }
    float GetFar()          { return m_far; }
    bool  IsShifted()       { return m_is_shifted; }

    //camera manipulation Functions
    void SetPosition(vec3 pos);
    void SetTarget(vec3 target);
    void SetUp(vec3 up);
    void SetRotation(vec3 rot);
    void SetRotation(quat rot);

    vec3 GetPosition();
    vec3 GetTarget();
    vec3 GetUp();
    vec3 GetRotationEuler();
    quat GetRotation();

    //Convenience functions
    float GetFrustumHeightAtDistance(float distance, float fov);
    float GetFOVForHeightAndDistance(float distance, float height);

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

private:
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
    bool    m_is_shifted;
};

} /* namespace lol */

#endif /* __CAMERA_H__ */
