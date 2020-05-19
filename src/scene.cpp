//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//            © 2014—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lol/msg>  // lol::msg
#include <cassert>  // assert
#include <map>      // std::map
#include <vector>   // std::vector
#include <array>    // std::array
#include <cstdlib>

#if defined(_WIN32)
#   define WIN32_LEAN_AND_MEAN 1
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
#endif

#include "lolgl.h"

LOLFX_RESOURCE_DECLARE(gpu_tile);
LOLFX_RESOURCE_DECLARE(gpu_palette);
LOLFX_RESOURCE_DECLARE(gpu_line);

LOLFX_RESOURCE_DECLARE(gpu_blit);
LOLFX_RESOURCE_DECLARE(gpu_postprocess);

namespace lol
{

/*
 * The global g_scenes object, initialised by Video::Init
 */

std::vector<Scene*> Scene::g_scenes;

static std::vector<SceneDisplay*> g_scene_displays;

static inline void gpu_marker(char const *message)
{
#if LOL_USE_GLEW && defined glStringMarkerGREMEDY
    if (GLEW_GREMEDY_string_marker)
        glStringMarkerGREMEDY(0, message);
#else
    (void)message;
#endif
}

//
// Public SceneDisplay class
//

void SceneDisplay::Add(SceneDisplay* display)
{
    g_scene_displays.push_back(display);
}

size_t SceneDisplay::GetCount()
{
    return g_scene_displays.size();
}

SceneDisplay* SceneDisplay::GetDisplay(int index)
{
    return g_scene_displays[index];
}

void SceneDisplay::DestroyAll()
{
    for (SceneDisplay* display : g_scene_displays)
        delete display;
    g_scene_displays.clear();
}

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

void PrimitiveSource::Render(Scene &)
{
}

void PrimitiveRenderer::Render(Scene &, std::shared_ptr<PrimitiveSource>)
{
}

/*
 * Scene implementation class
 */

uint64_t Scene::g_used_id = 1;
mutex Scene::g_prim_mutex;
std::map<uintptr_t, std::vector<std::shared_ptr<PrimitiveSource>>> Scene::g_prim_sources;

/*
 * Public Scene class
 */
Scene::Scene(ivec2 size)
  : m_size(size),
    m_wanted_size(size),
    m_renderer(std::make_shared<Renderer>(size))
{
    /* TODO: FIX THAT */
    if (g_used_id & ((uint64_t)1 << 63))
        msg::error("too many scenes!\n");

    m_mask_id = g_used_id;
    g_used_id = g_used_id << 1;

    for (int i = 0; i < 4; ++i)
        m_renderbuffer[i] = std::make_shared<Framebuffer>(m_size);
    m_pp.blit_shader = Shader::Create(LOLFX_RESOURCE_NAME(gpu_blit));
    m_pp.pp_shader = Shader::Create(LOLFX_RESOURCE_NAME(gpu_postprocess));
    m_pp.blit_pos_attr = m_pp.blit_shader->GetAttribLocation(VertexUsage::Position, 0);
    m_pp.pp_pos_attr = m_pp.pp_shader->GetAttribLocation(VertexUsage::Position, 0);
    m_pp.quad_vdecl = std::make_shared<VertexDeclaration>(VertexStream<vec2>(VertexUsage::Position));
    m_pp.m_buffer_uni[0][0] = m_pp.blit_shader->GetUniformLocation("u_buffer");
    m_pp.m_buffer_uni[1][0] = m_pp.pp_shader->GetUniformLocation("u_buffer");
    m_pp.m_buffer_uni[1][1] = m_pp.pp_shader->GetUniformLocation("u_prev_buffer");
    m_pp.m_buffer_uni[1][2] = m_pp.pp_shader->GetUniformLocation("u_prev_final");

    std::vector<vec2> quad { vec2( 1.0,  1.0), vec2(-1.0, -1.0), vec2( 1.0, -1.0),
                             vec2(-1.0, -1.0), vec2( 1.0,  1.0), vec2(-1.0,  1.0), };

    m_pp.quad_vbo = std::make_shared<VertexBuffer>(quad.size() * sizeof(vec2));
    m_pp.quad_vbo->set_data(quad.data(), quad.size() * sizeof(vec2));

    /* Create a default orthographic camera, in case the user doesn’t. */
    m_default_cam = new Camera();
    mat4 proj = mat4::ortho(0.f, (float)m_size.x, 0.f, (float)m_size.y, -1000.f, 1000.f);
    m_default_cam->SetProjection(proj);
    PushCamera(m_default_cam);

    m_tile_api.m_cam = -1;
    m_tile_api.m_shader = 0;
    m_tile_api.m_palette_shader = 0;
    m_tile_api.m_vdecl = std::make_shared<VertexDeclaration>(VertexStream<vec3>(VertexUsage::Position),
                                                             VertexStream<vec2>(VertexUsage::TexCoord));

    m_line_api.m_shader = 0;
    m_line_api.m_vdecl = std::make_shared<VertexDeclaration>(VertexStream<vec4,vec4>(VertexUsage::Position, VertexUsage::Color));

    m_line_api.m_debug_mask = 1;
}

void Scene::resize(ivec2 size)
{
    m_wanted_size = size;
}

Scene::~Scene()
{
    PopCamera(m_default_cam);

    /* FIXME: this must be done while the GL context is still active.
     * Change the code architecture to make sure of that. */
    /* FIXME: also, make sure we do not add code to Reset() that will
     * reallocate stuff */
    Reset();
}

void Scene::AddNew(ivec2 size)
{
    Scene::g_scenes.push_back(new Scene(size));
}

void Scene::DestroyScene(Scene* scene)
{
    remove_item(Scene::g_scenes, scene);
    delete scene;
}

void Scene::DestroyAll()
{
    while (Scene::g_scenes.size())
    {
        delete Scene::g_scenes.back();
        Scene::g_scenes.pop_back();
    }
}

size_t Scene::GetCount()
{
    return g_scenes.size();
}

bool Scene::IsReady(int index)
{
    return 0 <= index && index < int(g_scenes.size()) && !!g_scenes[index];
}

Scene& Scene::GetScene(int index)
{
    return *g_scenes[index];
}

void Scene::Link(entity* entity)
{
    entity->m_scene_mask |= m_mask_id;
}

bool Scene::IsRelevant(entity* entity)
{
    return !!(entity->m_scene_mask & m_mask_id);
}

Camera* Scene::GetCamera(int cam_idx)
{
    return (0 <= cam_idx && cam_idx < int(m_camera_stack.size())) ?
            m_camera_stack[cam_idx] :
            m_camera_stack.back();
}

int Scene::PushCamera(Camera *cam)
{
    Ticker::Ref(cam);
    m_camera_stack.push_back(cam);
    return int(m_camera_stack.size()) - 1;
}

void Scene::PopCamera(Camera *cam)
{
    /* Parse from the end because that’s probably where we’ll find
    * our camera first. */
    for (size_t i = m_camera_stack.size(); i--;)
    {
        if (m_camera_stack[i] == cam)
        {
            Ticker::Unref(cam);
            remove_at(m_camera_stack, i);
            return;
        }
    }

    msg::error("trying to pop a nonexistent camera from the scene\n");
}

void Scene::SetTileCam(int cam_idx)
{
    m_tile_api.m_cam = cam_idx;
}

void Scene::Reset()
{
    /* New scenegraph: Release fire&forget primitives */
    for (uintptr_t key : keys(m_prim_renderers))
    {
        for (size_t idx = 0; idx < m_prim_renderers[key].size(); ++idx)
            if (m_prim_renderers[key][idx]->m_fire_and_forget)
                ReleasePrimitiveRenderer(int(idx--), key);
    }

    m_tile_api.m_bufs.clear();
    m_tile_api.m_lights.clear();
}

//
// Primitive source stuff
//

int Scene::HasPrimitiveSource(uintptr_t key)
{
    int count;
    g_prim_mutex.lock();
    {
        count = int(g_prim_sources[key].size());
    }
    g_prim_mutex.unlock();
    return count;
}

int Scene::AddPrimitiveSource(uintptr_t key, std::shared_ptr<PrimitiveSource> source)
{
    int count;
    g_prim_mutex.lock();
    {
        count = int(g_prim_sources[key].size());
        g_prim_sources[key].push_back(source);
    }
    g_prim_mutex.unlock();
    return count;
}

void Scene::SetPrimitiveSource(int index, uintptr_t key, std::shared_ptr<PrimitiveSource> source)
{
    assert(source);
    assert(index >= 0);

    // Keep reference to old source until AFTER we release the lock
    std::shared_ptr<PrimitiveSource> old;

    g_prim_mutex.lock();
    {
        if (index < int(g_prim_sources[key].size()))
            old = g_prim_sources[key][index];
        else
            g_prim_sources[key].resize(index + 1);
        g_prim_sources[key][index] = source;
    }
    g_prim_mutex.unlock();
}

void Scene::ReleasePrimitiveSource(int index, uintptr_t key)
{
    std::shared_ptr<PrimitiveSource> old;
    g_prim_mutex.lock();
    {
        assert(0 <= index && index < int(g_prim_sources[key].size()));
        old = g_prim_sources[key][index];
        remove_at(g_prim_sources[key], index);
    }
    g_prim_mutex.unlock();
}

void Scene::ReleaseAllPrimitiveSources(uintptr_t key)
{
    // Delete oldies AFTER having released the lock
    std::vector<std::shared_ptr<PrimitiveSource>> oldies;

    g_prim_mutex.lock();
    {
        oldies.reserve(g_prim_sources[key].size());
        for (auto source : g_prim_sources[key])
            oldies.push_back(source);
        g_prim_sources[key].clear();
    }
    g_prim_mutex.unlock();
}

//
// Primitive renderer stuff
//

int Scene::HasPrimitiveRenderer(uintptr_t key)
{
    return int(m_prim_renderers[key].size());
}

void Scene::AddPrimitiveRenderer(uintptr_t key, std::shared_ptr<PrimitiveRenderer> renderer)
{
    renderer->m_fire_and_forget = true;
    m_prim_renderers[key].push_back(renderer);
}

void Scene::SetPrimitiveRenderer(int index, uintptr_t key, std::shared_ptr<PrimitiveRenderer> renderer)
{
    assert(renderer);
    assert(index >= 0);

    if (index >= int(m_prim_renderers[key].size()))
        m_prim_renderers[key].resize(index + 1);
    m_prim_renderers[key][index] = renderer;
}

void Scene::ReleasePrimitiveRenderer(int index, uintptr_t key)
{
    remove_at(m_prim_renderers[key], index);
}

void Scene::ReleaseAllPrimitiveRenderers(uintptr_t key)
{
    m_prim_renderers[key].clear();
}

void Scene::AddTile(TileSet *tileset, int id, vec3 pos, vec2 scale, float radians)
{
    assert(id < tileset->GetTileCount());

    ivec2 size = tileset->GetTileSize(id);
    mat4 model = mat4::translate(pos)
               * mat4::scale(scale.x, scale.y, 1.f)
               * mat4::translate(size.x * 0.5f, size.y * 0.5f, 0.f)
               * mat4::rotate(scale.x * scale.y < 0 ? radians : -radians,
                              vec3::axis_z);

    AddTile(tileset, id, model);
}

void Scene::AddTile(TileSet *tileset, int id, mat4 model)
{
    assert(id < tileset->GetTileCount());

    Tile t;
    t.m_model = model;
    t.m_tileset = tileset;
    t.m_id = id;

    if (tileset->GetPalette())
        m_tile_api.m_palettes.push_back(t);
    else
        m_tile_api.m_tiles.push_back(t);
}

void Scene::AddLine(vec3 a, vec3 b, vec4 col)
{
    struct line l { a, b, col, -1.f, 0xFFFFFFFF, false, false };
    m_line_api.m_lines.push_back(l);
}

void Scene::AddLine(vec3 a, vec3 b, vec4 col, float duration, uint32_t mask)
{
    struct line l { a, b, col, duration, mask, false, false };
    m_line_api.m_lines.push_back(l);
}

void Scene::AddLight(Light *l)
{
    m_tile_api.m_lights.push_back(l);
}

std::vector<Light *> const &Scene::GetLights()
{
    return m_tile_api.m_lights;
}

void Scene::SetDisplay(SceneDisplay* display)
{
    m_display = display;
}

void Scene::EnableDisplay()
{
    // If no display has been set, use the default one
    if (!m_display)
        SetDisplay(SceneDisplay::GetDisplay());
    m_display->Enable();
}

void Scene::DisableDisplay()
{
    assert(m_display);
    m_display->Disable();
}

static bool do_pp = true;

void Scene::pre_render(float)
{
    gpu_marker("### begin frame");

    // Handle resize event
    if (m_size != m_wanted_size)
    {
        m_size = m_wanted_size;

        m_renderer->viewport(ibox2(ivec2::zero, m_size));
        for (int i = 0; i < 4; ++i)
            m_renderbuffer[i] = std::make_shared<Framebuffer>(m_size);

        mat4 proj = mat4::ortho(0.f, (float)m_size.x, 0.f, (float)m_size.y, -1000.f, 1000.f);
        m_default_cam->SetProjection(proj);
    }

    /* First render into the offline buffer */
    if (do_pp)
    {
        m_renderbuffer[0]->Bind();
    }

    {
        render_context rc(m_renderer);
        if (do_pp)
        {
            rc.clear_color(vec4(0.f, 0.f, 0.f, 1.f));
            rc.clear_depth(1.f);
        }

        m_renderer->clear(ClearMask::Color | ClearMask::Depth);
    }
}

/* Render everything that the scene contains */
void Scene::render(float seconds)
{
    gpu_marker("### render scene");

    // FIXME: get rid of the delta time argument
    gpu_marker("# primitives");
    render_primitives();
    gpu_marker("# tiles");
    render_tiles();
    gpu_marker("# lines");
    render_lines(seconds);
}

void Scene::post_render(float)
{
    gpu_marker("### post render");

    if (do_pp)
    {
        m_renderbuffer[0]->Unbind();

        gpu_marker("# postprocess");

        m_renderbuffer[3]->Bind();

        render_context rc(m_renderer);
        rc.clear_color(vec4(0.f, 0.f, 0.f, 1.f));
        rc.clear_depth(1.f);
        m_renderer->clear(ClearMask::Color | ClearMask::Depth);

        /* Execute post process */
        m_pp.pp_shader->Bind();
        m_pp.pp_shader->SetUniform(m_pp.m_buffer_uni[1][0], m_renderbuffer[0]->GetTextureUniform(), 0);
        m_pp.pp_shader->SetUniform(m_pp.m_buffer_uni[1][1], m_renderbuffer[1]->GetTextureUniform(), 1);
        m_pp.pp_shader->SetUniform(m_pp.m_buffer_uni[1][2], m_renderbuffer[2]->GetTextureUniform(), 2);
        m_pp.quad_vdecl->Bind();
        m_pp.quad_vdecl->SetStream(m_pp.quad_vbo, m_pp.pp_pos_attr);
        m_pp.quad_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_pp.quad_vdecl->Unbind();
        m_pp.pp_shader->Unbind();
        m_renderbuffer[3]->Unbind();
    }

    if (do_pp)
    {
        gpu_marker("# blit frame");

        m_pp.blit_shader->Bind();

        render_context rc(m_renderer);
        rc.clear_color(vec4(0.f, 0.f, 0.f, 1.f));
        rc.clear_depth(1.f);
        m_renderer->clear(ClearMask::Color | ClearMask::Depth);

        /* Blit final image to screen */
        m_pp.blit_shader->SetUniform(m_pp.m_buffer_uni[0][0], m_renderbuffer[3]->GetTextureUniform(), 3);
        m_pp.quad_vdecl->Bind();
        m_pp.quad_vdecl->SetStream(m_pp.quad_vbo, m_pp.blit_pos_attr);
        m_pp.quad_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_pp.quad_vdecl->Unbind();
        m_pp.blit_shader->Unbind();
    }

    if (do_pp)
    {
        /* Swap back buffers */
        std::swap(m_renderbuffer[0], m_renderbuffer[1]);
        std::swap(m_renderbuffer[2], m_renderbuffer[3]);
    }

    gpu_marker("### end of frame");
}

void Scene::render_primitives()
{
    /* FIXME: Temp fix for mesh having no render context*/
    render_context rc(m_renderer);
    rc.cull_mode(CullMode::Clockwise);
    rc.depth_func(DepthFunc::LessOrEqual);

    /* new scenegraph */
    for (uintptr_t key : keys(m_prim_renderers))
    {
        for (size_t idx = 0; idx < m_prim_renderers[key].size(); ++idx)
        {
            /* TODO: Not sure if thread compliant */
            std::shared_ptr<PrimitiveSource> source;
            if (idx < g_prim_sources[key].size())
                source = g_prim_sources[key][idx];
            m_prim_renderers[key][idx]->Render(*this, source);
        }
    }
}

void Scene::render_tiles() // XXX: rename to Blit()
{
    render_context rc(m_renderer);

    /* Early test if nothing needs to be rendered */
    if (m_tile_api.m_tiles.empty() && m_tile_api.m_palettes.empty())
        return;

    /* FIXME: we disable culling for now because we don’t have a reliable
     * way to know which side is facing the camera. */
    rc.cull_mode(CullMode::Disabled);

    rc.depth_func(DepthFunc::LessOrEqual);
    rc.blend_func(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);
    rc.blend_equation(BlendEquation::Add, BlendEquation::Max);
    rc.alpha_func(AlphaFunc::GreaterOrEqual, 0.01f);

    if (!m_tile_api.m_shader)
        m_tile_api.m_shader = Shader::Create(LOLFX_RESOURCE_NAME(gpu_tile));
    if (!m_tile_api.m_palette_shader && !m_tile_api.m_palettes.empty())
        m_tile_api.m_palette_shader = Shader::Create(LOLFX_RESOURCE_NAME(gpu_palette));

    for (int p = 0; p < 2; p++)
    {
        auto shader = (p == 0) ? m_tile_api.m_shader : m_tile_api.m_palette_shader;
        auto &tiles  = (p == 0) ? m_tile_api.m_tiles : m_tile_api.m_palettes;

        if (tiles.empty())
            continue;

        ShaderUniform uni_mat, uni_tex, uni_pal, uni_texsize;
        ShaderAttrib attr_pos, attr_tex;
        attr_pos = shader->GetAttribLocation(VertexUsage::Position, 0);
        attr_tex = shader->GetAttribLocation(VertexUsage::TexCoord, 0);

        shader->Bind();

        uni_mat = shader->GetUniformLocation("u_projection");
        shader->SetUniform(uni_mat, GetCamera(m_tile_api.m_cam)->GetProjection());
        uni_mat = shader->GetUniformLocation("u_view");
        shader->SetUniform(uni_mat, GetCamera(m_tile_api.m_cam)->GetView());
        uni_mat = shader->GetUniformLocation("u_model");
        shader->SetUniform(uni_mat, mat4(1.f));

        uni_tex = shader->GetUniformLocation("u_texture");
        uni_pal = m_tile_api.m_palette_shader ? m_tile_api.m_palette_shader->GetUniformLocation("u_palette") : ShaderUniform();
        uni_texsize = shader->GetUniformLocation("u_texsize");

        for (size_t buf = 0, i = 0, n; i < tiles.size(); i = n, buf += 2)
        {
            /* Count how many quads will be needed */
            for (n = i + 1; n < tiles.size(); n++)
                if (tiles[i].m_tileset != tiles[n].m_tileset)
                    break;

            /* Create a vertex array object */
            auto vb1 = std::make_shared<VertexBuffer>(6 * (n - i) * sizeof(vec3));
            vec3 *vertex = (vec3 *)vb1->lock(0, 0);
            auto vb2 = std::make_shared<VertexBuffer>(6 * (n - i) * sizeof(vec2));
            vec2 *texture = (vec2 *)vb2->lock(0, 0);

            m_tile_api.m_bufs.push_back(vb1);
            m_tile_api.m_bufs.push_back(vb2);

            for (size_t j = i; j < n; j++)
            {
                tiles[i].m_tileset->BlitTile(tiles[j].m_id, tiles[j].m_model,
                                vertex + 6 * (j - i), texture + 6 * (j - i));
            }

            vb1->unlock();
            vb2->unlock();

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
            m_tile_api.m_vdecl->Bind();
            m_tile_api.m_vdecl->SetStream(vb1, attr_pos);
            m_tile_api.m_vdecl->SetStream(vb2, attr_tex);

            /* Draw arrays */
            m_tile_api.m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, int(n - i) * 6);
            m_tile_api.m_vdecl->Unbind();
            tiles[i].m_tileset->Unbind();
        }

        tiles.clear();

        shader->Unbind();

        if (!m_tile_api.m_palette_shader)
            break;
    }
}

// FIXME: get rid of the delta time argument
// XXX: rename to Blit()
void Scene::render_lines(float seconds)
{
    render_context rc(m_renderer);

    if (m_line_api.m_lines.empty())
        return;

    rc.depth_func(DepthFunc::LessOrEqual);
    rc.blend_func(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);
    rc.blend_equation(BlendEquation::Add, BlendEquation::Max);
    rc.alpha_func(AlphaFunc::GreaterOrEqual, 0.01f);

    size_t linecount = m_line_api.m_lines.size();

    if (!m_line_api.m_shader)
        m_line_api.m_shader = Shader::Create(LOLFX_RESOURCE_NAME(gpu_line));

    std::vector<std::array<vec4,4>> buff;
    buff.resize(linecount);
    int real_linecount = 0;

    mat4 const inv_view_proj = inverse(GetCamera()->GetProjection() * GetCamera()->GetView());
    (void)inv_view_proj;

    for (size_t i = 0; i < linecount; i++)
    {
        if (m_line_api.m_lines[i].mask & m_line_api.m_debug_mask)
        {
            buff[real_linecount][0] = vec4(m_line_api.m_lines[i].a, (float)m_line_api.m_lines[i].wtf1);
            buff[real_linecount][1] = m_line_api.m_lines[i].col;
            buff[real_linecount][2] = vec4(m_line_api.m_lines[i].b, (float)m_line_api.m_lines[i].wtf2);
            buff[real_linecount][3] = m_line_api.m_lines[i].col;
            real_linecount++;
        }
        m_line_api.m_lines[i].duration -= seconds;
        if (m_line_api.m_lines[i].duration < 0.f)
        {
            remove_at(m_line_api.m_lines, i--);
            linecount--;
        }
    }
    auto vb = std::make_shared<VertexBuffer>(buff.size() * sizeof(buff[0]));
    vb->set_data(buff.data(), buff.size() * sizeof(buff[0]));

    m_line_api.m_shader->Bind();

    ShaderUniform uni_mat, uni_tex;
    ShaderAttrib attr_pos, attr_col;
    attr_pos = m_line_api.m_shader->GetAttribLocation(VertexUsage::Position, 0);
    attr_col = m_line_api.m_shader->GetAttribLocation(VertexUsage::Color, 0);

    m_line_api.m_shader->Bind();

    uni_mat = m_line_api.m_shader->GetUniformLocation("u_projection");
    m_line_api.m_shader->SetUniform(uni_mat, GetCamera()->GetProjection());
    uni_mat = m_line_api.m_shader->GetUniformLocation("u_view");
    m_line_api.m_shader->SetUniform(uni_mat, GetCamera()->GetView());

    m_line_api.m_vdecl->Bind();
    m_line_api.m_vdecl->SetStream(vb, attr_pos, attr_col);
    m_line_api.m_vdecl->DrawElements(MeshPrimitive::Lines, 0, 2 * real_linecount);
    m_line_api.m_vdecl->Unbind();
    m_line_api.m_shader->Unbind();

    //m_line_api.m_lines.clear();
}

} /* namespace lol */

