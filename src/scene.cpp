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

#include <cstdlib>

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#include "core.h"
#include "lolgl.h"

namespace lol
{

struct Tile
{
    TileSet *tileset;
    uint32_t prio;
    vec3 pos;
    vec2 scale;
    int id, o;
};

/*
 * Scene implementation class
 */

class SceneData
{
    friend class Scene;

private:
    static int Compare(void const *p1, void const *p2)
    {
        Tile const *t1 = (Tile const *)p1;
        Tile const *t2 = (Tile const *)p2;

        return t2->prio - t1->prio;
    }

    mat4 m_model_matrix;
    mat4 m_view_matrix;
    mat4 m_proj_matrix;

    Array<Tile> tiles;

    Shader *m_shader;
    VertexDeclaration *m_vdecl;
    Array<VertexBuffer *> bufs;

    static Scene *scene;
};

Scene *SceneData::scene = NULL;

/*
 * Public Scene class
 */

Scene::Scene()
  : data(new SceneData())
{
    data->m_model_matrix = mat4(1.f);
    data->m_view_matrix = mat4(1.f);
    data->m_proj_matrix = mat4::ortho(0, Video::GetSize().x,
                                      0, Video::GetSize().y, -1000.f, 1000.f);

    data->m_shader = 0;
    data->m_vdecl = new VertexDeclaration(VertexStream<vec3>(VertexUsage::Position),
                                          VertexStream<vec2>(VertexUsage::TexCoord));
}

Scene::~Scene()
{
    /* FIXME: this must be done while the GL context is still active.
     * Change the code architecture to make sure of that. */
    /* FIXME: also, make sure we do not add code to Reset() that will
     * reallocate stuff */
    Reset();

    delete data->m_vdecl;
    delete data;
}

Scene *Scene::GetDefault()
{
    if (!SceneData::scene)
        SceneData::scene = new Scene();
    return SceneData::scene;
}

void Scene::Reset()
{
    for (int i = 0; i < data->bufs.Count(); i++)
        delete data->bufs[i];
    data->bufs.Empty();
}

void Scene::SetViewMatrix(mat4 const &m)
{
    data->m_view_matrix = m;
}

void Scene::SetProjMatrix(mat4 const &m)
{
    data->m_proj_matrix = m;
}

mat4 const &Scene::GetViewMatrix(void)
{
    return data->m_view_matrix;
}

mat4 const &Scene::GetProjMatrix(void)
{
    return data->m_proj_matrix;
}

void Scene::AddTile(TileSet *tileset, int id, vec3 pos, int o, vec2 scale)
{
    Tile t;
    /* FIXME: this sorting only works for a 45-degree camera */
    t.prio = -pos.y - 2 * 32 * pos.z + (o ? 0 : 32);
    t.tileset = tileset;
    t.id = id;
    t.pos = pos;
    t.o = o;
    t.scale = scale;

    data->tiles.Push(t);
}

void Scene::Render() // XXX: rename to Blit()
{
    /* Early exit if nothing needs to be rendered */
    if (!data->tiles.Count())
        return;

    if (!data->m_shader)
    {
#if !defined _XBOX && !defined __CELLOS_LV2__ && !defined USE_D3D9
        data->m_shader = Shader::Create(
#   if !defined HAVE_GLES_2X
            "#version 130\n"
#   endif
            "\n"
#   if defined HAVE_GLES_2X
            "attribute vec3 in_Position;\n"
            "attribute vec2 in_TexCoord;\n"
            "varying vec2 pass_TexCoord;\n"
#   else
            "in vec3 in_Position;\n"
            "in vec2 in_TexCoord;\n"
#   endif
            "uniform mat4 proj_matrix;\n"
            "uniform mat4 view_matrix;\n"
            "uniform mat4 model_matrix;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    gl_Position = proj_matrix * view_matrix * model_matrix"
            "                * vec4(in_Position, 1.0);\n"
#   if defined HAVE_GLES_2X
            "    pass_TexCoord = in_TexCoord;\n"
#   else
            "    gl_TexCoord[0] = vec4(in_TexCoord, 0.0, 0.0);\n"
#   endif
            "}\n",

#   if !defined HAVE_GLES_2X
            "#version 130\n"
#   else
            "precision mediump float;\n"
#   endif
            "\n"
            "uniform sampler2D in_Texture;\n"
#   if defined HAVE_GLES_2X
            "varying vec2 pass_TexCoord;\n"
#   endif
            "\n"
            "void main()\n"
            "{\n"
#   if defined HAVE_GLES_2X
            "    vec4 col = texture2D(in_Texture, pass_TexCoord);\n"
            //"    vec4 col = vec4(0.5, 1.0, 0.0, 0.5);\n"
            //"    vec4 col = vec4(pass_TexCoord * 4.0, 0.0, 0.25);\n"
#   else
            "    vec4 col = texture2D(in_Texture, vec2(gl_TexCoord[0]));\n"
#   endif
#   if 0
            "    float mul = 2.0;\n"
#       if 1
            "    vec2 d1 = mod(vec2(gl_FragCoord), vec2(2.0, 2.0));\n"
            "    float t1 = mod(3.0 * d1.x + 2.0 * d1.y, 4.0);\n"
            "    float dx2 = mod(floor(gl_FragCoord.x * 0.5), 2.0);\n"
            "    float dy2 = mod(floor(gl_FragCoord.y * 0.5), 2.0);\n"
            "    float t2 = mod(3.0 * dx2 + 2.0 * dy2, 4.0);\n"
            "    float dx3 = mod(floor(gl_FragCoord.x * 0.25), 2.0);\n"
            "    float dy3 = mod(floor(gl_FragCoord.y * 0.25), 2.0);\n"
            "    float t3 = mod(3.0 * dx3 + 2.0 * dy3, 4.0);\n"
            "    t1 = (1.0 + 16.0 * t1 + 4.0 * t2 + t3) / 65.0;\n"
            "    t2 = t1;\n"
            "    t3 = t1;\n"
#       else
            "    float rand = sin(gl_FragCoord.x * 1.23456) * 123.456\n"
            "               + cos(gl_FragCoord.y * 2.34567) * 789.012;\n"
            "    float t1 = mod(sin(rand) * 17.13043, 1.0);\n"
            "    float t2 = mod(sin(rand) * 27.13043, 1.0);\n"
            "    float t3 = mod(sin(rand) * 37.13043, 1.0);\n"
#       endif
            "    float fracx = fract(col.x * mul);\n"
            "    float fracy = fract(col.y * mul);\n"
            "    float fracz = fract(col.z * mul);\n"
            "    fracx = fracx > t1 ? 1.0 : 0.0;\n"
            "    fracy = fracy > t2 ? 1.0 : 0.0;\n"
            "    fracz = fracz > t3 ? 1.0 : 0.0;\n"
            "    col.x = (floor(col.x * mul) + fracx) / mul;\n"
            "    col.y = (floor(col.y * mul) + fracy) / mul;\n"
            "    col.z = (floor(col.z * mul) + fracz) / mul;\n"
#   endif
            "    gl_FragColor = col;\n"
            "}\n");
#else
        data->m_shader = Shader::Create(
            "void main(float4 in_Position : POSITION,"
            "          float2 in_TexCoord : TEXCOORD0,"
            "          uniform float4x4 proj_matrix,"
            "          uniform float4x4 view_matrix,"
            "          uniform float4x4 model_matrix,"
            "          out float2 out_TexCoord : TEXCOORD0,"
            "          out float4 out_Position : POSITION)"
            "{"
            "    out_Position = mul(proj_matrix, mul(view_matrix, mul(model_matrix, in_Position)));"
            "    out_TexCoord = in_TexCoord;"
            "}",

            "void main(float2 in_TexCoord : TEXCOORD0,"
#   if 0
            "          float4 in_FragCoord : WPOS,"
#   endif
            "          uniform sampler2D tex,"
            "          out float4 out_FragColor : COLOR)"
            "{"
            "    float4 col = tex2D(tex, in_TexCoord);"
#   if 0
            "    float mul = 2.0;\n"
            "    float t1, t2, t3;\n"
#       if 1
            "    float dx1 = frac(in_FragCoord.x * 0.5) * 2.0;\n"
            "    float dy1 = frac(in_FragCoord.y * 0.5) * 2.0;\n"
            "    t1 = frac((3.0 * dx1 + 2.0 * dy1) / 4.0) * 4.0;\n"
            "    float dx2 = frac(floor(in_FragCoord.x * 0.5) * 0.5) * 2.0;\n"
            "    float dy2 = frac(floor(in_FragCoord.y * 0.5) * 0.5) * 2.0;\n"
            "    t2 = frac((3.0 * dx2 + 2.0 * dy2) / 4.0) * 4.0;\n"
            "    float dx3 = frac(floor(in_FragCoord.x * 0.25) * 0.5) * 2.0;\n"
            "    float dy3 = frac(floor(in_FragCoord.y * 0.25) * 0.5) * 2.0;\n"
            "    t3 = frac((3.0 * dx3 + 2.0 * dy3) / 4.0) * 4.0;\n"
            "    t1 = (1.0 + 4.0 * t1 + t2) / 17.0;\n"
            "    t2 = t1;\n"
            "    t3 = t1;\n"
#       else
            "    float rand = sin(in_FragCoord.x * 1.23456) * 123.456\n"
            "               + cos(in_FragCoord.y * 2.34567) * 789.012;\n"
            "    t1 = frac(sin(rand) * 17.13043);\n"
            "    t2 = frac(sin(rand) * 27.13043);\n"
            "    t3 = frac(sin(rand) * 37.13043);\n"
#       endif
            "    float fracx = frac(col.x * mul);\n"
            "    float fracy = frac(col.y * mul);\n"
            "    float fracz = frac(col.z * mul);\n"
            "    fracx = fracx > t1 ? 1.0 : 0.0;\n"
            "    fracy = fracy > t2 ? 1.0 : 0.0;\n"
            "    fracz = fracz > t3 ? 1.0 : 0.0;\n"
            "    col.x = (floor(col.x * mul) + fracx) / mul;\n"
            "    col.y = (floor(col.y * mul) + fracy) / mul;\n"
            "    col.z = (floor(col.z * mul) + fracz) / mul;\n"
#   endif
            "    out_FragColor = col;"
            "}");
#endif
    }

#if 0
    // Randomise, then sort.
    for (int i = 0; i < data->tiles.Count(); i++)
    {
        Tile tmp = data->tiles[i];
        int j = rand() % data->tiles.Count();
        data->tiles[i] = data->tiles[j];
        data->tiles[j] = tmp;
    }
#endif
    qsort(&data->tiles[0], data->tiles.Count(),
          sizeof(Tile), SceneData::Compare);

    // XXX: debug stuff
    data->m_model_matrix = mat4::translate(320.0f, 240.0f, 0.0f);
#if 0
    static float f = 0.0f;
    f += 0.01f;
    data->m_model_matrix *= mat4::rotate(6.0f * sinf(f), 1.0f, 0.0f, 0.0f);
    data->m_model_matrix *= mat4::rotate(17.0f * cosf(f), 0.0f, 0.0f, 1.0f);
#endif
    data->m_model_matrix *= mat4::translate(-320.0f, -240.0f, 0.0f);
    // XXX: end of debug stuff

    ShaderUniform uni_mat, uni_tex;
    ShaderAttrib attr_pos, attr_tex;
    attr_pos = data->m_shader->GetAttribLocation("in_Position", VertexUsage::Position, 0);
    attr_tex = data->m_shader->GetAttribLocation("in_TexCoord", VertexUsage::TexCoord, 0);

    data->m_shader->Bind();

    uni_mat = data->m_shader->GetUniformLocation("proj_matrix");
    data->m_shader->SetUniform(uni_mat, data->m_proj_matrix);
    uni_mat = data->m_shader->GetUniformLocation("view_matrix");
    data->m_shader->SetUniform(uni_mat, data->m_view_matrix);
    uni_mat = data->m_shader->GetUniformLocation("model_matrix");
    data->m_shader->SetUniform(uni_mat, data->m_model_matrix);

#if defined USE_D3D9 || defined _XBOX
#else
    uni_tex = data->m_shader->GetUniformLocation("in_Texture");
    data->m_shader->SetUniform(uni_tex, 0);

#if !defined HAVE_GLES_2X
    glEnable(GL_TEXTURE_2D);
#endif
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
#if defined HAVE_GL_2X && !defined __APPLE__
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.01f);
#endif
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

    for (int buf = 0, i = 0, n; i < data->tiles.Count(); i = n, buf += 2)
    {
        /* Count how many quads will be needed */
        for (n = i + 1; n < data->tiles.Count(); n++)
            if (data->tiles[i].tileset != data->tiles[n].tileset)
                break;

        /* Create a vertex array object */
        VertexBuffer *vb1 = new VertexBuffer(6 * 3 * (n - i) * sizeof(float));
        float *vertex = (float *)vb1->Lock(0, 0);
        VertexBuffer *vb2 = new VertexBuffer(6 * 2 * (n - i) * sizeof(float));
        float *texture = (float *)vb2->Lock(0, 0);

        data->bufs.Push(vb1);
        data->bufs.Push(vb2);

        for (int j = i; j < n; j++)
        {
            data->tiles[i].tileset->BlitTile(data->tiles[j].id,
                            data->tiles[j].pos, data->tiles[j].o,
                            data->tiles[j].scale,
                            vertex + 18 * (j - i), texture + 12 * (j - i));
        }

        vb1->Unlock();
        vb2->Unlock();

        /* Bind texture */
        data->tiles[i].tileset->Bind();

        /* Bind vertex and texture coordinate buffers */
        data->m_vdecl->Bind();
        data->m_vdecl->SetStream(vb1, attr_pos);
        data->m_vdecl->SetStream(vb2, attr_tex);

        /* Draw arrays */
        data->m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, (n - i) * 2);
        data->m_vdecl->Unbind();
        data->tiles[i].tileset->Unbind();
    }

    data->tiles.Empty();

    data->m_shader->Unbind();

#if defined USE_D3D9 || defined _XBOX
    /* TODO */
#else
#if !defined HAVE_GLES_2X
    glDisable(GL_TEXTURE_2D);
#endif
    glDisable(GL_DEPTH_TEST);
#if defined HAVE_GL_2X && !defined __APPLE__
    glDisable(GL_ALPHA_TEST);
#endif
    glDisable(GL_BLEND);
#endif
}

} /* namespace lol */

