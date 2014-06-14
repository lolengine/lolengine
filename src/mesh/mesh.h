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

    /* FIXME: this should eventually take a “material” as argument, which
     * may behave differently between submeshes. */
    void SetMaterial(Shader *shader);
    void Render();

public:
    Array<class SubMesh *> m_submeshes;
};

/*
 * A submesh contains:
 *  - a vertex declaration
 *  - a list of VBOs
 *  - a list of textures
 *  - a shader
 */

class SubMesh
{
public:
    SubMesh(Shader *shader, VertexDeclaration* vdecl);
    ~SubMesh();

    void SetMeshPrimitive(MeshPrimitive mesh_primitive);
    void SetShader(Shader *shader);
    Shader *GetShader();
    void SetVertexDeclaration(VertexDeclaration *vdecl);
    void SetVertexBuffer(int index, VertexBuffer* vbo);
    void SetIndexBuffer(IndexBuffer* ibo);
    void AddTexture(const char* name, Texture* texture);

    void Render();

protected:
    MeshPrimitive m_mesh_prim;
    Shader *m_shader;
    VertexDeclaration* m_vdecl;
    Array<VertexBuffer *> m_vbos;
    IndexBuffer *m_ibo;

    Array<String, Texture*> m_textures;
};

} /* namespace lol */

#endif /* __MESH_MESH_H__ */

