//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if !defined _XBOX /* This file is meaningless on Xbox */

#include <cmath>

#include "core.h"
#include "lolgl.h"

using namespace std;

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
    GLuint bufs[2];
#if defined HAVE_GL_2X && !defined __APPLE__
    GLuint vaos[1];
#endif
};

/*
 * Public Gradient class
 */

Gradient::Gradient(vec3 aa, vec3 bb)
  : data(new GradientData())
{
    /* FIXME: this should not be hardcoded */
    position = aa;
    bbox[0] = aa;
    bbox[1] = bb;

    data->shader = NULL;
}

void Gradient::TickGame(float deltams)
{
    Entity::TickGame(deltams);
}

void Gradient::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    if (!data->shader)
    {
#if !defined __CELLOS_LV2__
        data->shader = Shader::Create(
            "#version 130\n"
            "\n"
            "in vec3 in_Vertex;\n"
            "in vec4 in_Color;\n"
            "out vec4 pass_Color;\n"
            "\n"
            "uniform mat4 proj_matrix;\n"
            "uniform mat4 view_matrix;\n"
            "uniform mat4 model_matrix;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    gl_Position = proj_matrix * view_matrix * model_matrix"
            "                * vec4(in_Vertex, 1.0);\n"
            "    pass_Color = in_Color;\n"
            "}\n",

            "#version 130\n"
            "\n"
            "in vec4 pass_Color;\n"
            "\n"
            "mat4 bayer = mat4( 0.0, 12.0,  3.0, 15.0,"
            "                   8.0,  4.0, 11.0,  7.0,"
            "                   2.0, 14.0,  1.0, 13.0,"
            "                  10.0,  6.0,  9.0,  5.0);\n"
            ""
            "mat4 cluster = mat4(12.0,  5.0,  6.0, 13.0,"
            "                     4.0,  0.0,  1.0,  7.0,"
            "                    11.0,  3.0,  2.0,  8.0,"
            "                    15.0, 10.0,  9.0, 14.0);\n"
            "\n"
            "void main()\n"
            "{\n"
            "    vec4 col = pass_Color;\n"
            "    float t = cluster[int(mod(gl_FragCoord.x, 4.0))]"
            "                     [int(mod(gl_FragCoord.y, 4.0))];\n"
            "    t = (t + 0.5) / 17.0;\n"
            "    col.x += fract(t - col.x) - t;\n"
            "    col.y += fract(t - col.y) - t;\n"
            "    col.z += fract(t - col.z) - t;\n"
            "    gl_FragColor = col;\n"
            "}\n");
#else
        data->shader = Shader::Create(
            "void main(float4 in_Vertex : POSITION,"
            "          float4 in_Color : COLOR,"
            "          uniform float4x4 proj_matrix,"
            "          uniform float4x4 view_matrix,"
            "          uniform float4x4 model_matrix,"
            "          out float4 out_Color : COLOR,"
            "          out float4 out_Position : POSITION)"
            "{"
            "    out_Position = mul(proj_matrix, mul(view_matrix, mul(model_matrix, in_Vertex)));"
            "    out_Color = in_Color;"
            "}",

            "float4x4 bayer = float4x4( 0.0, 12.0,  3.0, 15.0,"
            "                           8.0,  4.0, 11.0,  7.0,"
            "                           2.0, 14.0,  1.0, 13.0,"
            "                          10.0,  6.0,  9.0,  5.0);\n"
            ""
#if 1
            "float4x4 cluster = float4x4(12.0,  5.0,  6.0, 13.0,"
            "                             4.0,  0.0,  1.0,  7.0,"
            "                            11.0,  3.0,  2.0,  8.0,"
            "                            15.0, 10.0,  9.0, 14.0);\n"
#endif
            "\n"
            "void main(float4 in_Color : COLOR,"
            "          float4 in_FragCoord : WPOS,"
            "          out float4 out_FragColor : COLOR)"
            "{"
            "    float4 col = in_Color;"
#if 1
            "    int x = (int)in_FragCoord.x;"
            "    int y = (int)in_FragCoord.y;"
            // FIXME: we cannot address this matrix directly on the PS3
            "    float t = bayer[0][0];\n"
            "    t = (t + 0.5) / 17.0 + z - x;\n"
            "    col.x += frac(t - col.x) - t;\n"
            "    col.y += frac(t - col.y) - t;\n"
            "    col.z += frac(t - col.z) - t;\n"
#endif
            "    out_FragColor = col;"
            "}");
#endif
#if !defined __CELLOS_LV2__
        glGenBuffers(2, data->bufs);
#   if defined HAVE_GL_2X && !defined __APPLE__
        glGenVertexArrays(1, data->vaos);
#   endif
#endif
    }

    mat4 model_matrix = mat4(1.0f);

    GLuint uni_mat, attr_pos, attr_col;
#if !defined __CELLOS_LV2__
    attr_pos = data->shader->GetAttribLocation("in_Vertex");
    attr_col = data->shader->GetAttribLocation("in_Color");
#endif

    data->shader->Bind();

    uni_mat = data->shader->GetUniformLocation("proj_matrix");
    data->shader->SetUniform(uni_mat, Video::GetProjMatrix());
    uni_mat = data->shader->GetUniformLocation("view_matrix");
    data->shader->SetUniform(uni_mat, Video::GetViewMatrix());
    uni_mat = data->shader->GetUniformLocation("model_matrix");
    data->shader->SetUniform(uni_mat, model_matrix);

    float const vertex[] = {   0.0f,   0.0f, 0.0f,
                             640.0f,   0.0f, 0.0f,
                               0.0f, 480.0f, 0.0f,
                               0.0f, 480.0f, 0.0f,
                             640.0f, 480.0f, 0.0f,
                             640.0f,   0.0f, 0.0f, };

    float const color[] = { 0.73f, 0.85f, 0.85f, 1.0f,
                            0.73f, 0.85f, 0.85f, 1.0f,
                            0.0f, 0.0f, 1.0f, 1.0f,
                            0.0f, 0.0f, 1.0f, 1.0f,
                            0.0f, 0.0f, 1.0f, 1.0f,
                            0.73f, 0.85f, 0.85f, 1.0f, };

    data->shader->Bind();

    /* Bind vertex, color and texture coordinate buffers */
#if !defined __CELLOS_LV2__
#   if defined HAVE_GL_2X && !defined __APPLE__
    glBindVertexArray(data->vaos[0]);
#   endif
    glEnableVertexAttribArray(attr_pos);
    glEnableVertexAttribArray(attr_col);

    glBindBuffer(GL_ARRAY_BUFFER, data->bufs[0]);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat),
                 vertex, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(attr_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, data->bufs[1]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat),
                 color, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(attr_col, 4, GL_FLOAT, GL_FALSE, 0, 0);
#else
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertex);
    glColorPointer(4, GL_FLOAT, 0, color);
#endif

    /* Draw arrays */
    glDrawArrays(GL_TRIANGLES, 0, 6);

#if !defined __CELLOS_LV2__
#   if defined HAVE_GL_2X && !defined __APPLE__
    glBindVertexArray(0);
#   endif
    glDisableVertexAttribArray(attr_pos);
    glDisableVertexAttribArray(attr_col);
#else
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
#endif
}

Gradient::~Gradient()
{
    /* FIXME: destroy shader */
    delete data;
}

} /* namespace lol */

#endif /* !defined _XBOX */
