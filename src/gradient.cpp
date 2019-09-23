//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <memory>

LOLFX_RESOURCE_DECLARE(gradient);

namespace lol
{

/*
 * Gradient implementation class
 */

class GradientData
{
    friend class Gradient;

private:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<VertexDeclaration> m_vdecl;
    std::shared_ptr<VertexBuffer> m_vbo, m_cbo;
};

/*
 * Public Gradient class
 */

Gradient::Gradient(vec3 aa, vec3 bb)
  : data(std::make_unique<GradientData>())
{
    /* FIXME: this should not be hardcoded */
    m_position = aa;
    m_aabb.aa = aa;
    m_aabb.bb = bb;
}

void Gradient::tick_game(float seconds)
{
    entity::tick_game(seconds);
}

void Gradient::tick_draw(float seconds, Scene &scene)
{
    entity::tick_draw(seconds, scene);

    float const vertex[] = { m_aabb.aa.x, m_aabb.aa.y, 0.0f,
                             m_aabb.bb.x, m_aabb.aa.y, 0.0f,
                             m_aabb.aa.x, m_aabb.bb.y, 0.0f,
                             m_aabb.bb.x, m_aabb.bb.y, 0.0f,
                             m_aabb.aa.x, m_aabb.bb.y, 0.0f,
                             m_aabb.bb.x, m_aabb.aa.y, 0.0f, };

    float const color[] = { 0.73f, 0.85f, 0.85f, 1.0f,
                            0.73f, 0.85f, 0.85f, 1.0f,
                            0.0f, 0.0f, 1.0f, 1.0f,
                            0.0f, 0.0f, 1.0f, 1.0f,
                            0.0f, 0.0f, 1.0f, 1.0f,
                            0.73f, 0.85f, 0.85f, 1.0f, };

    if (!data->shader)
    {
        data->shader = Shader::Create(LOLFX_RESOURCE_NAME(gradient));

        data->m_vbo = std::make_shared<VertexBuffer>(sizeof(vertex));
        data->m_cbo = std::make_shared<VertexBuffer>(sizeof(color));

        data->m_vdecl = std::make_shared<VertexDeclaration>
                            (VertexStream<vec3>(VertexUsage::Position),
                             VertexStream<vec4>(VertexUsage::Color));
    }

    mat4 model_matrix = mat4(1.0f);

    ShaderUniform uni_mat;
    ShaderAttrib attr_pos, attr_col;
    attr_pos = data->shader->GetAttribLocation(VertexUsage::Position, 0);
    attr_col = data->shader->GetAttribLocation(VertexUsage::Color, 0);

    data->shader->Bind();

    uni_mat = data->shader->GetUniformLocation("u_projection");
    uni_mat = data->shader->GetUniformLocation("u_view");
    uni_mat = data->shader->GetUniformLocation("u_model");
    data->shader->SetUniform(uni_mat, scene.GetCamera()->GetProjection());
    data->shader->SetUniform(uni_mat, scene.GetCamera()->GetView());
    data->shader->SetUniform(uni_mat, model_matrix);

    data->shader->Bind();
    data->m_vdecl->Bind();

    data->m_vbo->set_data(vertex, sizeof(vertex));
    data->m_cbo->set_data(color, sizeof(color));

    /* Bind vertex and color buffers */
    data->m_vdecl->SetStream(data->m_vbo, attr_pos);
    data->m_vdecl->SetStream(data->m_cbo, attr_col);

    /* Draw arrays */
    data->m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
}

Gradient::~Gradient()
{
}

} /* namespace lol */
