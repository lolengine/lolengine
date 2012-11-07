//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Mesh class
// --------------
//

#if !defined __MESH_MESH_H__
#define __MESH_MESH_H__

namespace lol
{

class SubMesh
{
public:
    SubMesh();
    ~SubMesh();

    void Render(mat4 const &model);

private:
    Array<vec3> m_vertices;
    Array<vec2> m_uvs;
    Array<vec3> m_normals;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void Render(mat4 const &model);

private:
    Array<SubMesh *> m_submeshes;
};

} /* namespace lol */

#endif /* __MESH_MESH_H__ */

