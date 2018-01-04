//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Mesh class
// --------------
//

#include <lol/gpu/vertexbuffer.h>
#include <lol/gpu/indexbuffer.h>

//Assimp supports http://assimp.sourceforge.net/main_features_formats.html
#if LOL_USE_ASSIMP
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
#endif // _WIN32 || _WIN64
#endif // LOL_USE_ASSIMP

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

    //TODO: Not sure about the name
    void Render(Scene& scene, mat4 const &matrix);

protected:
    void Render();

public:
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
    void AddTexture(std::string const &name, Texture* texture);

protected:
    void Render();

    MeshPrimitive m_mesh_prim;
    Shader *m_shader;
    VertexDeclaration* m_vdecl;
    array<VertexBuffer *> m_vbos;
    IndexBuffer *m_ibo;

    array<std::string, Texture*> m_textures;
};

} /* namespace lol */

