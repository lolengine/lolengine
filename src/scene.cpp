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

#include <cstdlib>

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#include "core.h"
#include "lolgl.h"

LOLFX_RESOURCE_DECLARE(tile);

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

        if (t1->pos.z > t2->pos.z)
            return 1;
        if (t1->pos.z < t2->pos.z)
            return -1;
        return 0;
    }

    Array<Tile> m_tiles;
    Array<Light *> m_lights;

    Shader *m_shader;
    VertexDeclaration *m_vdecl;
    Array<VertexBuffer *> bufs;

    Camera *m_default_cam;
    Array<Camera *> m_camera_stack;

    static Scene *scene;
};

Scene *SceneData::scene = NULL;

/*
 * Public Scene class
 */

Scene::Scene()
  : data(new SceneData())
{
    /* Create a default orthographic camera, in case the user doesn’t. */
    data->m_default_cam = new Camera();
    mat4 proj = mat4::ortho(0, Video::GetSize().x, 0, Video::GetSize().y,
                            -1000.f, 1000.f);
    data->m_default_cam->SetProjection(proj);
    PushCamera(data->m_default_cam);

    data->m_shader = 0;
    data->m_vdecl = new VertexDeclaration(VertexStream<vec3>(VertexUsage::Position),
                                          VertexStream<vec2>(VertexUsage::TexCoord));
}

Scene::~Scene()
{
    PopCamera(data->m_default_cam);

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

void Scene::PushCamera(Camera *cam)
{
    Ticker::Ref(cam);
    data->m_camera_stack.Push(cam);
}

void Scene::PopCamera(Camera *cam)
{
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
    for (int i = 0; i < data->bufs.Count(); i++)
        delete data->bufs[i];
    data->bufs.Empty();
    data->m_lights.Empty();
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

    data->m_tiles.Push(t);
}

void Scene::AddLight(Light *l)
{
    data->m_lights.Push(l);
}

Array<Light *> const &Scene::GetLights() const
{
    return data->m_lights;
}

void Scene::Render() // XXX: rename to Blit()
{
    /* Early exit if nothing needs to be rendered */
    if (!data->m_tiles.Count())
        return;

    if (!data->m_shader)
        data->m_shader = Shader::Create(LOLFX_RESOURCE_NAME(tile));

#if 0
    // Randomise, then sort.
    for (int i = 0; i < data->m_tiles.Count(); i++)
    {
        Tile tmp = data->m_tiles[i];
        int j = std::rand() % data->m_tiles.Count();
        data->m_tiles[i] = data->m_tiles[j];
        data->m_tiles[j] = tmp;
    }
#endif
    qsort(&data->m_tiles[0], data->m_tiles.Count(),
          sizeof(Tile), SceneData::Compare);

    ShaderUniform uni_mat, uni_tex;
    ShaderAttrib attr_pos, attr_tex;
    attr_pos = data->m_shader->GetAttribLocation("in_Position", VertexUsage::Position, 0);
    attr_tex = data->m_shader->GetAttribLocation("in_TexCoord", VertexUsage::TexCoord, 0);

    data->m_shader->Bind();

    uni_mat = data->m_shader->GetUniformLocation("proj_matrix");
    data->m_shader->SetUniform(uni_mat, GetCamera()->GetProjection());
    uni_mat = data->m_shader->GetUniformLocation("view_matrix");
    data->m_shader->SetUniform(uni_mat, GetCamera()->GetView());
    uni_mat = data->m_shader->GetUniformLocation("model_matrix");
    data->m_shader->SetUniform(uni_mat, mat4(1.f));

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

        data->bufs.Push(vb1);
        data->bufs.Push(vb2);

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

        /* Bind vertex and texture coordinate buffers */
        data->m_vdecl->Bind();
        data->m_vdecl->SetStream(vb1, attr_pos);
        data->m_vdecl->SetStream(vb2, attr_tex);

        /* Draw arrays */
        data->m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, (n - i) * 6);
        data->m_vdecl->Unbind();
        data->m_tiles[i].tileset->Unbind();
    }

    data->m_tiles.Empty();

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

