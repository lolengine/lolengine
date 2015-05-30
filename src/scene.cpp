//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2014—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdlib>

#if defined(_WIN32)
#   define WIN32_LEAN_AND_MEAN 1
#   include <windows.h>
#endif

#include "lolgl.h"

LOLFX_RESOURCE_DECLARE(tile);
LOLFX_RESOURCE_DECLARE(palette);
LOLFX_RESOURCE_DECLARE(line);

namespace lol
{

/*
 * The global g_scenes object, initialised by Video::Init
 */

array<Scene*> Scene::g_scenes;

/*
 * A quick and dirty Tile structure for 2D blits
 */

struct Tile
{
    mat4 m_model;
    TileSet *m_tileset;
    int m_id;
};

//-----------------------------------------------------------------------------
static array<SceneDisplay*> m_scene_displays;

/*
 * Public SceneDisplay class
 */

void SceneDisplay::Add(SceneDisplay* display)
{
    m_scene_displays << display;
}

int SceneDisplay::GetCount()
{
    return m_scene_displays.count();
}

SceneDisplay* SceneDisplay::GetDisplay(int index)
{
    ASSERT(0 <= index && index < m_scene_displays.count(),
           "invalid display index %d", index);
    return m_scene_displays[index];
}

void SceneDisplay::DestroyAll()
{
    for (SceneDisplay* display : m_scene_displays)
        delete display;
    m_scene_displays.empty();
}

/* ------------------------------------------------ */
void SceneDisplay::Enable()
{
    // TODO: PROFILER STUFF
}

void SceneDisplay::Disable()
{
    // TODO: PROFILER STUFF
}

/*
 * Primitive implementation class
 */

void PrimitiveSource::Render(Scene& scene) { UNUSED(scene); }

void PrimitiveRenderer::Render(Scene& scene, PrimitiveSource* primitive)
{
    UNUSED(scene);
    UNUSED(primitive);
}

/*
 * Scene implementation class
 */

class SceneData
{
    friend class Scene;

private:
    SceneData()
    {
        /* TODO: FIX THAT */
        ASSERT(!(m_used_id & ((uint64_t)1 << 63)), "Too many scenes !!!!");
        m_mask_id = m_used_id;
        m_used_id = m_used_id << 1;
    }

    /* Mask ID */
    /* TODO: Do a mask class that handles more than 64 slots */
    static uint64_t m_used_id;
    uint64_t m_mask_id = 0;

    /* Scene display: if none has been set to the scene,
     * the default one created by the app will be used */
    SceneDisplay* m_display = nullptr;

    /* Sources are shared by all scenes.
     * Renderers are scene-dependent. They get the primitive in the identical
     * slot to render with the given scene.
     * Primitives and renderers will be kept until:
     * - Updated by entity
     * - Marked Fire&Forget
     * - Scene is destroyed */
    map<uintptr_t, array<PrimitiveRenderer*>> m_prim_renderers;
    static map<uintptr_t, array<PrimitiveSource*>> m_prim_sources;
    static mutex m_prim_mutex;

    /* Old API <P0, P1, COLOR, TIME, MASK> */
    float   m_new_line_time;
    int     m_new_line_mask;
    float   m_new_line_segment_size;
    vec4    m_new_line_color;
    array<vec3, vec3, vec4, float, int, bool, bool> m_lines;
    int m_debug_mask;
    Shader *m_line_shader;
    VertexDeclaration *m_line_vdecl;

    int m_tile_cam;
    array<Tile> m_tiles;
    array<Tile> m_palettes;
    array<Light *> m_lights;

    Shader *m_tile_shader;
    Shader *m_palette_shader;
    VertexDeclaration *m_tile_vdecl;
    array<VertexBuffer *> m_tile_bufs;

    Camera *m_default_cam;
    array<Camera *> m_camera_stack;
};
uint64_t SceneData::m_used_id = 1;
map<uintptr_t, array<PrimitiveSource*>> SceneData::m_prim_sources;
mutex SceneData::m_prim_mutex;

/*
 * Public Scene class
 */
Scene::Scene(ivec2 size)
  : data(new SceneData())
{
    /* Create a default orthographic camera, in case the user doesn’t. */
    data->m_default_cam = new Camera();
    mat4 proj = mat4::ortho(0.f, (float)size.x, 0.f, (float)size.y, -1000.f, 1000.f);
    data->m_default_cam->SetProjection(proj);
    PushCamera(data->m_default_cam);

    data->m_tile_cam = -1;
    data->m_tile_shader = 0;
    data->m_palette_shader = 0;
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

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
void Scene::AddNew(ivec2 size)
{
    Scene::g_scenes << new Scene(size);
}

void Scene::DestroyScene(Scene* scene)
{
    Scene::g_scenes.remove_item(scene);
    delete scene;
}

void Scene::DestroyAll()
{
    while (Scene::g_scenes.count())
        delete Scene::g_scenes.pop();
}

int Scene::GetCount()
{
    return g_scenes.count();
}

//-----------------------------------------------------------------------------
bool Scene::IsReady(int index)
{
    return 0 <= index && index < g_scenes.count() && !!g_scenes[index];
}

//-----------------------------------------------------------------------------
Scene& Scene::GetScene(int index)
{
    ASSERT(0 <= index && index < g_scenes.count() && !!g_scenes[index],
           "Trying to get a non-existent scene %d", index);
    return *g_scenes[index];
}

//-----------------------------------------------------------------------------
void Scene::Link(Entity* entity)
{
    entity->m_scene_mask |= data->m_mask_id;
}

//-----------------------------------------------------------------------------
bool Scene::IsRelevant(Entity* entity)
{
    return !!(entity->m_scene_mask & data->m_mask_id);
}

//-----------------------------------------------------------------------------
Camera* Scene::GetCamera(int cam_idx)
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    return (0 <= cam_idx && cam_idx < data->m_camera_stack.count()) ?
            data->m_camera_stack[cam_idx] :
            data->m_camera_stack.last();
}

//-----------------------------------------------------------------------------
int Scene::PushCamera(Camera *cam)
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    Ticker::Ref(cam);
    data->m_camera_stack.push(cam);
    return (int)data->m_camera_stack.count() - 1;
}

//-----------------------------------------------------------------------------
void Scene::PopCamera(Camera *cam)
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    /* Parse from the end because that’s probably where we’ll find
    * our camera first. */
    for (int i = data->m_camera_stack.count(); i--;)
    {
        if (data->m_camera_stack[i] == cam)
        {
            Ticker::Unref(cam);
            data->m_camera_stack.remove(i);
            return;
        }
    }

    ASSERT(false, "trying to pop a nonexistent camera from the scene");
}

//-----------------------------------------------------------------------------
void Scene::SetTileCam(int cam_idx)
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    data->m_tile_cam = cam_idx;
}

//-----------------------------------------------------------------------------
void Scene::Reset()
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    /* New scenegraph: Release fire&forget primitives */
    array<uintptr_t> keys = data->m_prim_renderers.keys();
    for (uintptr_t key : keys)
    {
        for (int idx = 0; idx < data->m_prim_renderers[key].count(); ++idx)
            if (data->m_prim_renderers[key][idx]->m_fire_and_forget)
                ReleasePrimitiveRenderer(idx--, key);
    }

    for (int i = 0; i < data->m_tile_bufs.count(); i++)
        delete data->m_tile_bufs[i];
    data->m_tile_bufs.empty();

    data->m_lights.empty();
}

//---- Primitive source stuff -------------------------------------------------
int Scene::HasPrimitiveSource(uintptr_t key)
{
    int count;
    SceneData::m_prim_mutex.lock();
    {
        count = SceneData::m_prim_sources[key].count();
    }
    SceneData::m_prim_mutex.unlock();
    return count;
}

int Scene::AddPrimitiveSource(uintptr_t key, PrimitiveSource* source)
{
    int count;
    SceneData::m_prim_mutex.lock();
    {
        count = SceneData::m_prim_sources[key].count();
        SceneData::m_prim_sources[key].push(source);
    }
    SceneData::m_prim_mutex.unlock();
    return count;
}

void Scene::SetPrimitiveSource(int index, uintptr_t key, PrimitiveSource* source)
{
    ASSERT(source);
    ASSERT(index >= 0);

    PrimitiveSource* old = nullptr;
    SceneData::m_prim_mutex.lock();
    {
        if (index < SceneData::m_prim_sources[key].count())
            old = SceneData::m_prim_sources[key][index];
        else
            SceneData::m_prim_sources[key].resize(index + 1);
        SceneData::m_prim_sources[key][index] = source;
    }
    SceneData::m_prim_mutex.unlock();

    // Delete old AFTER having released the lock
    delete old;
}

void Scene::ReleasePrimitiveSource(int index, uintptr_t key)
{
    PrimitiveSource* old = nullptr;
    SceneData::m_prim_mutex.lock();
    {
        ASSERT(0 <= index && index < SceneData::m_prim_sources[key].count());
        old = SceneData::m_prim_sources[key][index];
        SceneData::m_prim_sources[key].remove(index);
    }
    SceneData::m_prim_mutex.unlock();

    // Delete old AFTER having released the lock
    delete old;
}

void Scene::ReleaseAllPrimitiveSources(uintptr_t key)
{
    array<PrimitiveSource*> oldies;
    SceneData::m_prim_mutex.lock();
    {
        oldies.reserve(SceneData::m_prim_sources[key].count());
        for (PrimitiveSource* source : SceneData::m_prim_sources[key])
            oldies << source;
        SceneData::m_prim_sources[key].empty();
    }
    SceneData::m_prim_mutex.unlock();

    // Delete oldies AFTER having released the lock
    for (PrimitiveSource* old : oldies)
        delete old;
}

//---- Primitive renderer stuff -----------------------------------------------
int Scene::HasPrimitiveRenderer(uintptr_t key)
{
    return data->m_prim_renderers[key].count();
}

void Scene::AddPrimitiveRenderer(uintptr_t key, PrimitiveRenderer* renderer)
{
    renderer->m_fire_and_forget = true;
    data->m_prim_renderers[key].push(renderer);
}

void Scene::SetPrimitiveRenderer(int index, uintptr_t key, PrimitiveRenderer* renderer)
{
    ASSERT(renderer);
    ASSERT(index >= 0);

    if (index < data->m_prim_renderers[key].count())
    {
        ASSERT(data->m_prim_renderers[key][index]);
        delete data->m_prim_renderers[key][index];
    }
    else
        data->m_prim_renderers[key].resize(index + 1);
    data->m_prim_renderers[key][index] = renderer;
}

void Scene::ReleasePrimitiveRenderer(int index, uintptr_t key)
{
    ASSERT(0 <= index && index < data->m_prim_renderers[key].count());

    delete data->m_prim_renderers[key][index];
    data->m_prim_renderers[key].remove(index);
}

void Scene::ReleaseAllPrimitiveRenderers(uintptr_t key)
{
    for (PrimitiveRenderer*& renderer : data->m_prim_renderers[key])
    {
        delete renderer;
        renderer = nullptr;
    }
}

//-----------------------------------------------------------------------------
void Scene::AddTile(TileSet *tileset, int id, vec3 pos, int o, vec2 scale, float angle)
{
    ASSERT(id < tileset->GetTileCount());

    ivec2 size = tileset->GetTileSize(id);
    mat4 model = mat4::translate(pos)
               * mat4::scale(scale.x, scale.y, 1.f)
               * mat4::translate(size.x * 0.5f, size.y * 0.5f, 0.f)
               * mat4::rotate(scale.x * scale.y < 0 ? angle : -angle,
                              vec3::axis_z);

    AddTile(tileset, id, model);
}

void Scene::AddTile(TileSet *tileset, int id, mat4 model)
{
    ASSERT(id < tileset->GetTileCount());

    Tile t;
    t.m_model = model;
    t.m_tileset = tileset;
    t.m_id = id;

    if (tileset->GetPalette())
        data->m_palettes.push(t);
    else
        data->m_tiles.push(t);
}

//-----------------------------------------------------------------------------
void Scene::SetLineTime(float new_time)
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    data->m_new_line_time = new_time;
}

void Scene::SetLineMask(int new_mask)
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    data->m_new_line_mask = new_mask;
}

void Scene::SetLineSegmentSize(float new_segment_size)
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    data->m_new_line_segment_size = new_segment_size;
}

void Scene::SetLineColor(vec4 new_color)
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    data->m_new_line_color = new_color;
}

//-----------------------------------------------------------------------------
float Scene::GetLineSegmentSize()
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    return data->m_new_line_segment_size;
}

vec4 Scene::GetLineColor()
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    return data->m_new_line_color;
}

//-----------------------------------------------------------------------------
void Scene::AddLine(vec3 a, vec3 b, vec4 color)
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    data->m_lines.push(a, b, color,
                        data->m_new_line_time, data->m_new_line_mask, false, false);
}

//-----------------------------------------------------------------------------
void Scene::AddLight(Light *l)
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    data->m_lights.push(l);
}

//-----------------------------------------------------------------------------
array<Light *> const &Scene::GetLights()
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    return data->m_lights;
}

//-----------------------------------------------------------------------------
void Scene::SetDisplay(SceneDisplay* display)
{
    data->m_display = display;
}

//-----------------------------------------------------------------------------
void Scene::EnableDisplay()
{
    // If no display has been set, use the default one
    if (!data->m_display)
        SetDisplay(SceneDisplay::GetDisplay());
    data->m_display->Enable();
}

void Scene::DisableDisplay()
{
    ASSERT(data->m_display);
    data->m_display->Disable();
}

//-----------------------------------------------------------------------------
void Scene::RenderPrimitives()
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    /* FIXME: Temp fix for mesh having no render context*/
    RenderContext rc;
    rc.SetCullMode(CullMode::Clockwise);
    rc.SetDepthFunc(DepthFunc::LessOrEqual);

    /* new scenegraph */
    array<uintptr_t> keys = data->m_prim_renderers.keys();
    for (uintptr_t key : keys)
    {
        for (int idx = 0; idx < data->m_prim_renderers[key].count(); ++idx)
        {
            /* TODO: Not sure if thread compliant */
            data->m_prim_renderers[key][idx]->Render(*this, idx < SceneData::m_prim_sources[key].count() ? SceneData::m_prim_sources[key][idx] : nullptr);
        }
    }
}

//-----------------------------------------------------------------------------
void Scene::RenderTiles() // XXX: rename to Blit()
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    RenderContext rc;

    /* Early test if nothing needs to be rendered */
    if (!data->m_tiles.count() && !data->m_palettes.count())
        return;

    /* FIXME: we disable culling for now because we don’t have a reliable
     * way to know which side is facing the camera. */
    rc.SetCullMode(CullMode::Disabled);

    rc.SetDepthFunc(DepthFunc::LessOrEqual);
    rc.SetBlendFunc(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);
    rc.SetBlendEquation(BlendEquation::Add, BlendEquation::Max);
    rc.SetAlphaFunc(AlphaFunc::GreaterOrEqual, 0.01f);

#if defined USE_D3D9 || defined _XBOX
    /* TODO */
#elif (defined USE_GLEW || defined HAVE_GL_2X) && !defined HAVE_GLES_2X
    glEnable(GL_TEXTURE_2D);
#endif

    if (!data->m_tile_shader)
        data->m_tile_shader = Shader::Create(LOLFX_RESOURCE_NAME(tile));
    if (!data->m_palette_shader)
        data->m_palette_shader = Shader::Create(LOLFX_RESOURCE_NAME(palette));

    for (int p = 0; p < 2; p++)
    {
        Shader *shader      = (p == 0) ? data->m_tile_shader : data->m_palette_shader;
        array<Tile>& tiles  = (p == 0) ? data->m_tiles : data->m_palettes;

        if (tiles.count() == 0)
            continue;

        ShaderUniform uni_mat, uni_tex, uni_pal, uni_texsize;
        ShaderAttrib attr_pos, attr_tex;
        attr_pos = shader->GetAttribLocation(VertexUsage::Position, 0);
        attr_tex = shader->GetAttribLocation(VertexUsage::TexCoord, 0);

        shader->Bind();

        uni_mat = shader->GetUniformLocation("u_projection");
        shader->SetUniform(uni_mat, GetCamera(data->m_tile_cam)->GetProjection());
        uni_mat = shader->GetUniformLocation("u_view");
        shader->SetUniform(uni_mat, GetCamera(data->m_tile_cam)->GetView());
        uni_mat = shader->GetUniformLocation("u_model");
        shader->SetUniform(uni_mat, mat4(1.f));

        uni_tex = shader->GetUniformLocation("u_texture");
        uni_pal = data->m_palette_shader->GetUniformLocation("u_palette");
        uni_texsize = shader->GetUniformLocation("u_texsize");

        for (int buf = 0, i = 0, n; i < tiles.count(); i = n, buf += 2)
        {
            /* Count how many quads will be needed */
            for (n = i + 1; n < tiles.count(); n++)
                if (tiles[i].m_tileset != tiles[n].m_tileset)
                    break;

            /* Create a vertex array object */
            VertexBuffer *vb1 = new VertexBuffer(6 * (n - i) * sizeof(vec3));
            vec3 *vertex = (vec3 *)vb1->Lock(0, 0);
            VertexBuffer *vb2 = new VertexBuffer(6 * (n - i) * sizeof(vec2));
            vec2 *texture = (vec2 *)vb2->Lock(0, 0);

            data->m_tile_bufs.push(vb1);
            data->m_tile_bufs.push(vb2);

            for (int j = i; j < n; j++)
            {
                tiles[i].m_tileset->BlitTile(tiles[j].m_id, tiles[j].m_model,
                                vertex + 6 * (j - i), texture + 6 * (j - i));
            }

            vb1->Unlock();
            vb2->Unlock();

            /* Bind texture */
            if (tiles[i].m_tileset->GetPalette())
            {
                if (tiles[i].m_tileset->GetTexture())
                    shader->SetUniform(uni_tex, tiles[i].m_tileset->GetTexture()->GetTextureUniform(), 0);
                if (tiles[i].m_tileset->GetPalette()->GetTexture())
                    shader->SetUniform(uni_pal, tiles[i].m_tileset->GetPalette()->GetTexture()->GetTextureUniform(), 1);
            }
            else
            {
                shader->SetUniform(uni_tex, 0);
                if (tiles[i].m_tileset->GetTexture())
                    shader->SetUniform(uni_tex, tiles[i].m_tileset->GetTexture()->GetTextureUniform(), 0);
                tiles[i].m_tileset->Bind();
            }
            shader->SetUniform(uni_texsize,
                           (vec2)tiles[i].m_tileset->GetTextureSize());

            /* Bind vertex and texture coordinate buffers */
            data->m_tile_vdecl->Bind();
            data->m_tile_vdecl->SetStream(vb1, attr_pos);
            data->m_tile_vdecl->SetStream(vb2, attr_tex);

            /* Draw arrays */
            data->m_tile_vdecl->DrawElements(MeshPrimitive::Triangles, 0, (n - i) * 6);
            data->m_tile_vdecl->Unbind();
            tiles[i].m_tileset->Unbind();
        }

        tiles.empty();

        shader->Unbind();
    }


#if defined USE_D3D9 || defined _XBOX
    /* TODO */
#elif (defined USE_GLEW || defined HAVE_GL_2X) && !defined HAVE_GLES_2X
    glDisable(GL_TEXTURE_2D);
#endif
}

//-----------------------------------------------------------------------------
void Scene::RenderLines(float seconds) // XXX: rename to Blit()
{
    ASSERT(!!data, "Trying to access a non-ready scene");

    RenderContext rc;

    if (!data->m_lines.count())
        return;

    rc.SetDepthFunc(DepthFunc::LessOrEqual);
    rc.SetBlendFunc(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);
    rc.SetBlendEquation(BlendEquation::Add, BlendEquation::Max);
    rc.SetAlphaFunc(AlphaFunc::GreaterOrEqual, 0.01f);

    int linecount = (int)data->m_lines.count();

    if (!data->m_line_shader)
        data->m_line_shader = Shader::Create(LOLFX_RESOURCE_NAME(line));

    array<vec4, vec4, vec4, vec4> buff;
    buff.resize(linecount);
    int real_linecount = 0;
    mat4 const inv_view_proj = inverse(GetCamera()->GetProjection() * GetCamera()->GetView());
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
            data->m_lines.remove_swap(i--);
            linecount--;
        }
    }
    VertexBuffer *vb = new VertexBuffer(buff.bytes());
    float *vertex = (float *)vb->Lock(0, 0);
    memcpy(vertex, buff.data(), buff.bytes());
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

    //data->m_lines.empty();
    delete vb;
}

} /* namespace lol */

