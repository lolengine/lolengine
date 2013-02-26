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

    for (int i = 0; i < 4; i++)
    {
        int j = ((i & 1) << 1) | ((i >> 1) ^ 1);

        Scene::GetDefault()->AddLine(v[i], v[i + 4], color);
        Scene::GetDefault()->AddLine(v[i], v[j], color);
        Scene::GetDefault()->AddLine(v[i + 4], v[j + 4], color);
    }
}

} /* namespace lol */

