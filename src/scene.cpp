//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>

#if WIN32
#   define WIN32_LEAN_AND_MEAN 1
#   include <windows.h>
#endif

#include "core.h"
#include "lolgl.h"

LOLFX_RESOURCE_DECLARE(tile);
LOLFX_RESOURCE_DECLARE(line);

namespace lol
{

/*
 * The global g_scene object, initialised by Video::Init
 */

Scene *g_scene = nullptr;

/*
 * A quick and dirty Tile structure for 2D blits
 */

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
    /* New scenegraph */
    Array<Primitive> m_primitives;

    /* Old API <P0, P1, COLOR, TIME, MASK> */
    float   m_new_line_time;
    int     m_new_line_mask;
    float   m_new_line_segment_size;
    vec4    m_new_line_color;
    Array<vec3, vec3, vec4, float, int, bool, bool> m_lines;
    int m_debug_mask;
    Shader *m_line_shader;
    VertexDeclaration *m_line_vdecl;

    Array<Tile> m_tiles;
    Array<Light *> m_lights;

    Shader *m_tile_shader;
    VertexDeclaration *m_tile_vdecl;
    Array<VertexBuffer *> m_tile_bufs;

    Camera *m_default_cam;
    Array<Camera *> m_camera_stack;
};

/*
 * Public Scene class
 */

Scene::Scene(ivec2 size)
  : data(new SceneData())
{
    /* Create a default orthographic camera, in case the user doesn’t. */
    data->m_default_cam = new Camera();
    mat4 proj = mat4::ortho(0.f, size.x, 0.f, size.y, -1000.f, 1000.f);
    data->m_default_cam->SetProjection(proj);
    PushCamera(data->m_default_cam);

    data->m_tile_shader = 0;
    data->m_tile_vdecl = new VertexDeclaration(VertexStream<vec3>(VertexUsage::Position),
                                               VertexStream<vec2>(VertexUsage::TexCoord));

    data->m_line_shader = 0;
    data->m_line_vdecl = new VertexDeclaration(VertexStream<vec4,vec4>(VertexUsage::Position, VertexUsage::Color));

    data->m_debug_mask = 1;

    SetLineTime();
    SetLineMask();
    SetLineSegmentSize();
    SetLineColor();
}

Scene::~Scene()
{
    PopCamera(data->m_default_cam);

    /* FIXME: this must be done while the GL context is still active.
     * Change the code architecture to make sure of that. */
    /* FIXME: also, make sure we do not add code to Reset() that will
     * reallocate stuff */
    Reset();

    delete data->m_line_vdecl;
    delete data->m_tile_vdecl;
    delete data;
}

void Scene::PushCamera(Camera *cam)
{
    ASSERT(this, "trying to push a camera before g_scene is ready");

    Ticker::Ref(cam);
    data->m_camera_stack.Push(cam);
}

void Scene::PopCamera(Camera *cam)
{
    ASSERT(this, "trying to pop a camera before g_scene is ready");

    /* Parse from the end because that’s probably where we’ll find
     * our camera first. */
    for (int i = data->m_camera_stack.Count(); i--; )
    {
        if (data->m_camera_stack[i] == cam)
        {
            Ticker::Unref(cam);
            data->m_camera_stack.Remove(i);
            return;
        }
    }

    ASSERT(false, "trying to pop a nonexistent camera from the scene");
}

Camera *Scene::GetCamera()
{
    return data->m_camera_stack.Last();
}

void Scene::Reset()
{
    for (int i = 0; i < data->m_tile_bufs.Count(); i++)
        delete data->m_tile_bufs[i];
    data->m_tile_bufs.Empty();

    data->m_lights.Empty();
}

void Scene::AddPrimitive(Mesh *mesh, Shader *shader, mat4 const &matrix)
{
    data->m_primitives.Push(Primitive(mesh, shader, matrix));
}

void Scene::AddTile(TileSet *tileset, int id, vec3 pos, int o, vec2 scale)
{
    ASSERT(id < tileset->GetTileCount());

    Tile t;
    /* FIXME: this sorting only works for a 45-degree camera */
    t.prio = -pos.y - 2 * 32 * pos.z + ((float)o ? 0 : 32);
    t.tileset = tileset;
    t.id = id;
    t.pos = pos;
    t.o = o;
    t.scale = scale;

    data->m_tiles.Push(t);
}

void Scene::SetLineTime(float new_time)     { data->m_new_line_time = new_time; }
void Scene::SetLineMask(int new_mask)       { data->m_new_line_mask = new_mask; }
void Scene::SetLineSegmentSize(float new_segment_size) { data->m_new_line_segment_size = new_segment_size; }
float Scene::GetLineSegmentSize()           { return data->m_new_line_segment_size; }
void Scene::SetLineColor(vec4 new_color)    { data->m_new_line_color = new_color; }
vec4 Scene::GetLineColor()                  { return data->m_new_line_color; }

void Scene::AddLine(vec3 a, vec3 b, vec4 color)
{
    data->m_lines.Push(a, b, color, data->m_new_line_time, data->m_new_line_mask, false, false);
}

void Scene::AddLine(vec2 a, vec3 b, vec4 color, float az)
{
    data->m_lines.Push(vec3(a, az), b, color, data->m_new_line_time, data->m_new_line_mask, true, false);
}

void Scene::AddLine(vec2 a, vec2 b, vec4 color, float az, float bz)
{
    data->m_lines.Push(vec3(a, az), vec3(b, bz), color, data->m_new_line_time, data->m_new_line_mask, true, true);
}

void Scene::AddLight(Light *l)
{
    data->m_lights.Push(l);
}

Array<Light *> const &Scene::GetLights() const
{
    return data->m_lights;
}

void Scene::RenderPrimitives()
{
    /* TODO: this should be the main entry for rendering of all
     * primitives found in the scene graph. When we have one. */

    Shader *shader = nullptr;
    ShaderUniform u_model, uni_tex, uni_texsize;
    ShaderAttrib a_pos, a_tex;

    for (int i = 0; i < data->m_primitives.Count(); ++i)
    {
        Primitive &p = data->m_primitives[i];

        /* If this primitive uses a new shader, update attributes */
        if (p.m_shader != shader)
        {
            shader = p.m_shader;

            a_pos = shader->GetAttribLocation(VertexUsage::Position, 0);
            a_tex = shader->GetAttribLocation(VertexUsage::TexCoord, 0);

            shader->Bind();

            ShaderUniform u_mat;
            u_mat = shader->GetUniformLocation("u_projection");
            shader->SetUniform(u_mat, GetCamera()->GetProjection());
            u_mat = shader->GetUniformLocation("u_view");
            shader->SetUniform(u_mat, GetCamera()->GetView());

            u_model = shader->GetUniformLocation("u_model");
        }

        shader->SetUniform(u_model, p.m_matrix);
    }
}

void Scene::RenderTiles() // XXX: rename to Blit()
{
    RenderContext rc;

    /* Early test if nothing needs to be rendered */
    if (!data->m_tiles.Count())
        return;

    rc.SetDepthFunc(DepthFunc::LessOrEqual);
    rc.SetBlendFunc(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);
    rc.SetAlphaFunc(AlphaFunc::GreaterOrEqual, 0.01f);

#if defined USE_D3D9 || defined _XBOX
#elif !defined HAVE_GLES_2X
    glEnable(GL_TEXTURE_2D);
#endif
    if (!data->m_tile_shader)
        data->m_tile_shader = Shader::Create(LOLFX_RESOURCE_NAME(tile));

    ShaderUniform uni_mat, uni_tex, uni_texsize;
    ShaderAttrib attr_pos, attr_tex;
    attr_pos = data->m_tile_shader->GetAttribLocation(VertexUsage::Position, 0);
    attr_tex = data->m_tile_shader->GetAttribLocation(VertexUsage::TexCoord, 0);

    data->m_tile_shader->Bind();

    uni_mat = data->m_tile_shader->GetUniformLocation("u_projection");
    data->m_tile_shader->SetUniform(uni_mat, GetCamera()->GetProjection());
    uni_mat = data->m_tile_shader->GetUniformLocation("u_view");
    data->m_tile_shader->SetUniform(uni_mat, GetCamera()->GetView());
    uni_mat = data->m_tile_shader->GetUniformLocation("u_model");
    data->m_tile_shader->SetUniform(uni_mat, mat4(1.f));

    uni_tex = data->m_tile_shader->GetUniformLocation("u_texture");
    data->m_tile_shader->SetUniform(uni_tex, 0);
    uni_texsize = data->m_tile_shader->GetUniformLocation("u_texsize");

    for (int buf = 0, i = 0, n; i < data->m_tiles.Count(); i = n, buf += 2)
    {
        /* Count how many quads will be needed */
        for (n = i + 1; n < data->m_tiles.Count(); n++)
            if (data->m_tiles[i].tileset != data->m_tiles[n].tileset)
                break;

        /* Create a vertex array object */
        VertexBuffer *vb1 = new VertexBuffer(6 * 3 * (n - i) * sizeof(float));
        float *vertex = (float *)vb1->Lock(0, 0);
        VertexBuffer *vb2 = new VertexBuffer(6 * 2 * (n - i) * sizeof(float));
        float *texture = (float *)vb2->Lock(0, 0);

        data->m_tile_bufs.Push(vb1);
        data->m_tile_bufs.Push(vb2);

        for (int j = i; j < n; j++)
        {
            data->m_tiles[i].tileset->BlitTile(data->m_tiles[j].id,
                            data->m_tiles[j].pos, data->m_tiles[j].o,
                            data->m_tiles[j].scale,
                            vertex + 18 * (j - i), texture + 12 * (j - i));
        }

        vb1->Unlock();
        vb2->Unlock();

        /* Bind texture */
        data->m_tiles[i].tileset->Bind();
        data->m_tile_shader->SetUniform(uni_texsize,
                       (vec2)data->m_tiles[i].tileset->GetTextureSize());

        /* Bind vertex and texture coordinate buffers */
        data->m_tile_vdecl->Bind();
        data->m_tile_vdecl->SetStream(vb1, attr_pos);
        data->m_tile_vdecl->SetStream(vb2, attr_tex);

        /* Draw arrays */
        data->m_tile_vdecl->DrawElements(MeshPrimitive::Triangles, 0, (n - i) * 6);
        data->m_tile_vdecl->Unbind();
        data->m_tiles[i].tileset->Unbind();
    }

    data->m_tiles.Empty();

    data->m_tile_shader->Unbind();

#if defined USE_D3D9 || defined _XBOX
    /* TODO */
#elif !defined HAVE_GLES_2X
    glDisable(GL_TEXTURE_2D);
#endif
}

void Scene::RenderLines(float seconds) // XXX: rename to Blit()
{
    RenderContext rc;

    if (!data->m_lines.Count())
        return;

    rc.SetDepthFunc(DepthFunc::LessOrEqual);
    rc.SetBlendFunc(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);
    rc.SetAlphaFunc(AlphaFunc::GreaterOrEqual, 0.01f);

    int linecount = data->m_lines.Count();

    if (!data->m_line_shader)
        data->m_line_shader = Shader::Create(LOLFX_RESOURCE_NAME(line));

    Array<vec4, vec4, vec4, vec4> buff;
    buff.Resize(linecount);
    int real_linecount = 0;
    mat4 const inv_view_proj = inverse(g_scene->GetCamera()->GetProjection() * g_scene->GetCamera()->GetView());
    for (int i = 0; i < linecount; i++)
    {
        if (data->m_lines[i].m5 & data->m_debug_mask)
        {
            buff[real_linecount].m1 = vec4(data->m_lines[i].m1, (float)data->m_lines[i].m6);
            buff[real_linecount].m2 = data->m_lines[i].m3;
            buff[real_linecount].m3 = vec4(data->m_lines[i].m2, (float)data->m_lines[i].m7);
            buff[real_linecount].m4 = data->m_lines[i].m3;
            real_linecount++;
        }
        data->m_lines[i].m4 -= seconds;
        if (data->m_lines[i].m4 < 0.f)
        {
            data->m_lines.RemoveSwap(i--);
            linecount--;
        }
    }
    int vb_size = sizeof(vec4) * 4 * real_linecount;
    VertexBuffer *vb = new VertexBuffer(vb_size);
    float *vertex = (float *)vb->Lock(0, 0);
    memcpy(vertex, buff.Data(), vb_size);
    vb->Unlock();

    data->m_line_shader->Bind();

    ShaderUniform uni_mat, uni_tex;
    ShaderAttrib attr_pos, attr_col;
    attr_pos = data->m_line_shader->GetAttribLocation(VertexUsage::Position, 0);
    attr_col = data->m_line_shader->GetAttribLocation(VertexUsage::Color, 0);

    data->m_line_shader->Bind();

    uni_mat = data->m_line_shader->GetUniformLocation("u_projection");
    data->m_line_shader->SetUniform(uni_mat, GetCamera()->GetProjection());
    uni_mat = data->m_line_shader->GetUniformLocation("u_view");
    data->m_line_shader->SetUniform(uni_mat, GetCamera()->GetView());

    data->m_line_vdecl->Bind();
    data->m_line_vdecl->SetStream(vb, attr_pos, attr_col);
    data->m_line_vdecl->DrawElements(MeshPrimitive::Lines, 0, 2 * real_linecount);
    data->m_line_vdecl->Unbind();
    data->m_line_shader->Unbind();

    //data->m_lines.Empty();
    delete vb;
}

} /* namespace lol */

