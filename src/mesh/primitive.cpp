//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstring>
#include <cstdlib>

namespace lol
{

/*
 * Primitive class
 */

PrimitiveMesh::PrimitiveMesh(SubMesh *submesh, mat4 const &matrix)
  : m_submesh(submesh),
    m_matrix(matrix)
{
}

PrimitiveMesh::~PrimitiveMesh()
{
}

void PrimitiveMesh::Render() const
{
    /* TODO: this should be the main entry for rendering of all
    * primitives found in the scene graph. When we have one. */

    Shader *shader = nullptr;
    ShaderUniform u_model, u_modelview, u_normalmat, uni_tex, uni_texsize;
    ShaderAttrib a_pos, a_tex;

    {
        /* If this primitive uses a new shader, update attributes */
        if (m_submesh->GetShader() != shader)
        {
            shader = m_submesh->GetShader();

            a_pos = shader->GetAttribLocation(VertexUsage::Position, 0);
            a_tex = shader->GetAttribLocation(VertexUsage::TexCoord, 0);

            shader->Bind();

            /* Per-scene matrices */
            ShaderUniform u_mat;
            u_mat = shader->GetUniformLocation("u_projection");
            shader->SetUniform(u_mat, Scene::GetCamera()->GetProjection());
            u_mat = shader->GetUniformLocation("u_view");
            shader->SetUniform(u_mat, Scene::GetCamera()->GetView());
            u_mat = shader->GetUniformLocation("u_inv_view");
            shader->SetUniform(u_mat, inverse(Scene::GetCamera()->GetView()));

            /* Per-object matrices, will be set later */
            u_model = shader->GetUniformLocation("u_model");
            u_modelview = shader->GetUniformLocation("u_modelview");
            u_normalmat = shader->GetUniformLocation("u_normalmat");

            /* Per-scene environment */
            array<Light *> const &lights = Scene::GetLights();
            array<vec4> light_data;

            /* FIXME: the 4th component of the position can be used for other things */
            /* FIXME: GetUniform("blabla") is costly */
            for (auto l : lights)
                light_data << vec4(l->GetPosition(), (float)l->GetType()) << l->GetColor();
            while (light_data.Count() < LOL_MAX_LIGHT_COUNT)
                light_data << vec4::zero << vec4::zero;

            ShaderUniform u_lights = shader->GetUniformLocation("u_lights");
            shader->SetUniform(u_lights, light_data);
        }

        shader->SetUniform(u_model, m_matrix);
        mat4 modelview = Scene::GetCamera()->GetView() * m_matrix;
        shader->SetUniform(u_modelview, modelview);
        shader->SetUniform(u_normalmat, transpose(inverse(mat3(modelview))));

        m_submesh->Render();
    }
}

} /* namespace lol */

