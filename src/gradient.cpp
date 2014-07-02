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

#include <lol/main.h>

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
    Shader *shader;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo, *m_cbo;
};

/*
 * Public Gradient class
 */

Gradient::Gradient(vec3 aa, vec3 bb)
  : data(new GradientData())
{
    /* FIXME: this should not be hardcoded */
    m_position = aa;
    m_bbox[0] = aa;
    m_bbox[1] = bb;

    data->shader = nullptr;
}

void Gradient::TickGame(float seconds)
{
    Entity::TickGame(seconds);
}

void Gradient::TickDraw(float seconds, Scene &scene)
{
    Entity::TickDraw(seconds, scene);

    float const vertex[] = { m_bbox[0].x, m_bbox[0].y, 0.0f,
                             m_bbox[1].x, m_bbox[0].y, 0.0f,
                             m_bbox[0].x, m_bbox[1].y, 0.0f,
                             m_bbox[1].x, m_bbox[1].y, 0.0f,
                             m_bbox[0].x, m_bbox[1].y, 0.0f,
                             m_bbox[1].x, m_bbox[0].y, 0.0f, };

    float const color[] = { 0.73f, 0.85f, 0.85f, 1.0f,
                            0.73f, 0.85f, 0.85f, 1.0f,
                            0.0f, 0.0f, 1.0f, 1.0f,
                            0.0f, 0.0f, 1.0f, 1.0f,
                            0.0f, 0.0f, 1.0f, 1.0f,
                            0.73f, 0.85f, 0.85f, 1.0f, };

    if (!data->shader)
    {
        data->shader = Shader::Create(LOLFX_RESOURCE_NAME(gradient));

        data->m_vbo = new VertexBuffer(sizeof(vertex));
        data->m_cbo = new VertexBuffer(sizeof(color));

        data->m_vdecl = new VertexDeclaration(VertexStream<vec3>(VertexUsage::Position),
                                              VertexStream<vec4>(VertexUsage::Color));
    }

    mat4 model_matrix = mat4(1.0f);

    ShaderUniform uni_mat;
    ShaderAttrib attr_pos, attr_col;
    attr_pos = data->shader->GetAttribLocation(VertexUsage::Position, 0);
    attr_col = data->shader->GetAttribLocation(VertexUsage::Color, 0);

    data->shader->Bind();

    uni_mat = data->shader->GetUniformLocation("u_projection");
    data->shader->SetUniform(uni_mat, g_scene->GetCamera()->GetProjection());
    uni_mat = data->shader->GetUniformLocation("u_view");
    data->shader->SetUniform(uni_mat, g_scene->GetCamera()->GetView());
    uni_mat = data->shader->GetUniformLocation("u_model");
    data->shader->SetUniform(uni_mat, model_matrix);

    data->shader->Bind();
    data->m_vdecl->Bind();

    void *tmp = data->m_vbo->Lock(0, 0);
    memcpy(tmp, vertex, sizeof(vertex));
    data->m_vbo->Unlock();

    tmp = data->m_cbo->Lock(0, 0);
    memcpy(tmp, color, sizeof(color));
    data->m_cbo->Unlock();

    /* Bind vertex and color buffers */
    data->m_vdecl->SetStream(data->m_vbo, attr_pos);
    data->m_vdecl->SetStream(data->m_cbo, attr_col);

    /* Draw arrays */
    data->m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
}

Gradient::~Gradient()
{
    /* FIXME: destroy shader */
    delete data;
}

} /* namespace lol */
