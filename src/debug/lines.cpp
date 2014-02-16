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

#include "core.h"

namespace lol
{

void Debug::DrawLine(vec3 a, vec3 b, vec4 color)
{
    g_scene->AddLine(a, b, color);
}

void Debug::DrawBox(vec3 a, vec3 b, vec4 color)
{
    vec3 v[8];
    for (int i = 0; i < 8; i++)
    {
        v[i].x = i & 1 ? a.x : b.x;
        v[i].y = i & 2 ? a.y : b.y;
        v[i].z = i & 4 ? a.z : b.z;
    }

    for (int i = 0; i < 4; i++)
    {
        int j = ((i & 1) << 1) | ((i >> 1) ^ 1);

        Debug::DrawLine(v[i], v[i + 4], color);
        Debug::DrawLine(v[i], v[j], color);
        Debug::DrawLine(v[i + 4], v[j + 4], color);
    }
}

void Debug::DrawViewProj(mat4 view, mat4 proj, vec4 color)
{
     mat4 view_proj = proj * view;

     //Pos to center
    vec3 p0 = (inverse(view) * vec4(vec3::zero, 1.f)).xyz;

    //Near plane
    vec4 p[4] = { inverse(view_proj) * vec4(-1.f, 1.f, -1.f, 1.f),
                  inverse(view_proj) * vec4( 1.f, 1.f, -1.f, 1.f),
                  inverse(view_proj) * vec4( 1.f,-1.f, -1.f, 1.f),
                  inverse(view_proj) * vec4(-1.f,-1.f, -1.f, 1.f) };
    for (int i = 0; i < 4; i++)
        p[i] = p[i] / p[i].w;

    //Draw near
    for (int i = 0; i < 4; i++)
        Debug::DrawLine(p[i].xyz, p0, color);

    Debug::DrawViewProj(view_proj, color);
}

void Debug::DrawViewProj(mat4 view_proj, vec4 color)
{
    //Near plane
    vec4 p[8] = { inverse(view_proj) * vec4(-1.f, 1.f, 1.f, 1.f),
                  inverse(view_proj) * vec4( 1.f, 1.f, 1.f, 1.f),
                  inverse(view_proj) * vec4( 1.f,-1.f, 1.f, 1.f),
                  inverse(view_proj) * vec4(-1.f,-1.f, 1.f, 1.f),
                  inverse(view_proj) * vec4(-1.f, 1.f,-1.f, 1.f),
                  inverse(view_proj) * vec4( 1.f, 1.f,-1.f, 1.f),
                  inverse(view_proj) * vec4( 1.f,-1.f,-1.f, 1.f),
                  inverse(view_proj) * vec4(-1.f,-1.f,-1.f, 1.f)
                };
    for (int i = 0; i < 8; i++)
        p[i] = p[i] / p[i].w;

    //Draw near
    for (int i = 0; i < 4; i++)
        Debug::DrawLine(p[i].xyz, p[(i + 1) % 4].xyz, color);
    //Draw far
    for (int i = 4; i < 8; i++)
        Debug::DrawLine(p[i].xyz, p[(i - 4 + 1) % 4 + 4].xyz, color);
    //Draw near to far
    for (int i = 0; i < 4; i++)
        Debug::DrawLine(p[i].xyz, p[i + 4].xyz, color);
    //Draw diagonal
    for (int i = 2; i < 6; i++)
        Debug::DrawLine(p[i].xyz, p[i + ((i < 4)?(-2):(+2))].xyz, color);
}

} /* namespace lol */

