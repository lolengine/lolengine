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

    void SetView(mat4 const &view);
    void SetView(vec3 eye, vec3 target, vec3 up);
    void SetView(vec3 pos, quat rot);

    void SetProjection(mat4 const &proj);

    mat4 GetView();
    mat4 GetProjection();

    vec3 GetPosition();

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

private:
    mat4 m_view_matrix, m_proj_matrix;
};

} /* namespace lol */

#endif /* __CAMERA_H__ */

