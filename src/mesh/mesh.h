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
// The Mesh class
// --------------
//

#include <lol/gpu/vertexbuffer.h>
#include <lol/gpu/indexbuffer.h>

//Assimp supports http://assimp.sourceforge.net/main_features_formats.html
#if USE_ASSIMP
//Cause build has a problem with function choice.
#if _WIN32 || _WIN64
#define sin  lol::sin
#define asin lol::asin
#define cos  lol::cos
#define acos lol::acos
#endif //_WIN32 || _WIN64
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#if _WIN32 || _WIN64
#undef sin
#undef asin
#undef cos
#undef acos
#endif //_WIN32 || _WIN64
#endif //USE_ASSIMP

namespace lol
{

/*
 * A mesh contains a list of submeshes. This is a convenient way to
 * handle different materials or mesh types (static, skeletal, morph targets, etc.)
 * within the same container object.
 */

class Mesh
{
    friend class Scene;

public:
    Mesh();
    ~Mesh();

    /* FIXME: this should eventually take a “material” as argument, which
     * may behave differently between submeshes. */
    void SetMaterial(Shader *shader);

protected:
    void Render();

    array<class SubMesh *> m_submeshes;
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
    friend class PrimitiveMesh;
    friend class Mesh;

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

protected:
    void Render();

    MeshPrimitive m_mesh_prim;
    Shader *m_shader;
    VertexDeclaration* m_vdecl;
    array<VertexBuffer *> m_vbos;
    IndexBuffer *m_ibo;

    array<String, Texture*> m_textures;
};

} /* namespace lol */

