//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <memory>
#include <cstring>
#include <cstdlib>

namespace lol
{

/*
 * Primitive class
 */

PrimitiveMesh::PrimitiveMesh(std::shared_ptr<SubMesh> submesh, mat4 const &matrix)
  : m_submesh(submesh),
    m_matrix(matrix)
{
}

PrimitiveMesh::~PrimitiveMesh()
{
}

void PrimitiveMesh::Render(Scene& scene)
{
    /* TODO: this should be the main entry for rendering of all
    * primitives found in the scene graph. When we have one. */

    std::shared_ptr<Shader> shader;
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
            shader->SetUniform(u_mat, scene.GetCamera()->GetProjection());
            u_mat = shader->GetUniformLocation("u_view");
            shader->SetUniform(u_mat, scene.GetCamera()->GetView());
            u_mat = shader->GetUniformLocation("u_inv_view");
            shader->SetUniform(u_mat, inverse(scene.GetCamera()->GetView()));

            /* Per-object matrices, will be set later */
            u_model = shader->GetUniformLocation("u_model");
            u_modelview = shader->GetUniformLocation("u_modelview");
            u_normalmat = shader->GetUniformLocation("u_normalmat");

            /* Per-scene environment */
            std::vector<Light *> const &lights = scene.GetLights();
            std::vector<vec4> light_data;

            /* FIXME: the 4th component of the position can be used for other things */
            /* FIXME: GetUniform("blabla") is costly */
            for (auto l : lights)
            {
                light_data.push_back(vec4(l->GetPosition(), (float)l->GetType()));
                light_data.push_back(l->GetColor());
            }
            while (light_data.size() < LOL_MAX_LIGHT_COUNT)
            {
                light_data.push_back(vec4::zero);
                light_data.push_back(vec4::zero);
            }

            ShaderUniform u_lights = shader->GetUniformLocation("u_lights");
            shader->SetUniform(u_lights, light_data);
        }

        shader->SetUniform(u_model, m_matrix);
        mat4 modelview = scene.GetCamera()->GetView() * m_matrix;
        shader->SetUniform(u_modelview, modelview);
        shader->SetUniform(u_normalmat, transpose(inverse(mat3(modelview))));

        m_submesh->Render();
    }
}

} // namespace lol
