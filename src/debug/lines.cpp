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
    Scene::GetDefault()->AddLine(a, b, color);
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

    Scene::GetDefault()->AddLine(v[0], v[1], color);
    Scene::GetDefault()->AddLine(v[1], v[3], color);
    Scene::GetDefault()->AddLine(v[3], v[2], color);
    Scene::GetDefault()->AddLine(v[2], v[0], color);

    Scene::GetDefault()->AddLine(v[4], v[5], color);
    Scene::GetDefault()->AddLine(v[5], v[7], color);
    Scene::GetDefault()->AddLine(v[7], v[6], color);
    Scene::GetDefault()->AddLine(v[6], v[4], color);

    Scene::GetDefault()->AddLine(v[0], v[4], color);
    Scene::GetDefault()->AddLine(v[1], v[5], color);
    Scene::GetDefault()->AddLine(v[2], v[6], color);
    Scene::GetDefault()->AddLine(v[3], v[7], color);
}

} /* namespace lol */

