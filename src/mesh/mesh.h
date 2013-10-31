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
// The Mesh class
// --------------
//

#if !defined __MESH_MESH_H__
#define __MESH_MESH_H__

namespace lol
{

/*
 * A mesh contains a list of submeshes. This is a convenient way to
 * handle different materials or mesh types (static, skeletal, morph
 * targets, etc.) within the same container object.
 */

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void Render(mat4 const &model);

private:
    Array<class SubMesh *> m_submeshes;
};

/*
 * A submesh contains:
 *  - a vertex declaration
 *  - a list of VBOs
 *  - a list of textures
 */

class SubMesh
{
public:
    SubMesh(VertexDeclaration* vdecl);
    ~SubMesh();

    void SetMeshPrimitive(MeshPrimitive mesh_primitive);
    void SetVertexBuffer(int index, VertexBuffer* vbo);
    void AddTexture(const char* name, Texture* texture);

    void Render(Shader* shader);

protected:
    VertexDeclaration* m_vdecl;
    MeshPrimitive m_mesh_prim;
    Array<VertexBuffer *> m_vbos;

    Array<String, Texture*> m_textures;
};

} /* namespace lol */

#endif /* __MESH_MESH_H__ */

