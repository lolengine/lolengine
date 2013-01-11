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

namespace lol
{

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::Render(mat4 const &model)
{
    for (int i = 0; i < m_submeshes.Count(); ++i)
        m_submeshes[i]->Render(model);
}

SubMesh::SubMesh()
{
}

SubMesh::~SubMesh()
{
}

void SubMesh::Render(mat4 const &model)
{
}

} /* namespace lol */

