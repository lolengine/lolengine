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

#pragma once

//
// The Scene class
// ---------------
//

#include "light.h"
#include "camera.h"
#include "mesh/mesh.h"
#include "application/application.h"

#include <lol/gpu/renderer.h>
#include <lol/gpu/framebuffer.h>
#include <lol/thread>

#include <vector>   // std::vector
#include <memory>   // std::shared_ptr
#include <stdint.h> // uintptr_t

#define LOL_MAX_LIGHT_COUNT 8

namespace lol
{

//
// A quick and dirty Tile structure for 2D blits
//

struct Tile
{
    mat4 m_model;
    class TileSet *m_tileset;
    int m_id;
};

class PrimitiveRenderer
{
    friend class Scene;

public:
    PrimitiveRenderer() { }
    virtual ~PrimitiveRenderer() { }
    virtual void Render(Scene& scene);

private:
    bool m_fire_and_forget = false;
};

class Scene
{
    friend class Video;

private:
    static std::vector<Scene*> g_scenes;

    Scene(ivec2 size);
    ~Scene();

public:
    static void AddNew(ivec2 size);
private: //Private because I don't know if we should have it
    static void DestroyScene(Scene* scene);
private:
    static void DestroyAll();
public:
    static size_t GetCount();

    static void add_display(std::shared_ptr<app::display> display);
    static std::shared_ptr<app::display> get_display(int index = 0);

    static bool IsReady(int index = 0);
    static Scene& GetScene(int index = 0);

public:
    //TODO: don't like the name
    void Link(entity* entity);
    bool IsRelevant(entity* entity);

public:
    Camera* GetCamera(int cam_idx = -1);
    int PushCamera(Camera *cam);
    void PopCamera(Camera *cam);
    void SetTileCam(int cam_idx);

    void Reset();

    std::shared_ptr<Renderer> get_renderer() { return m_renderer; }

    // FIXME: this PrimitiveRenderer stuff was never really finished; maybe remove it?

    /* ============================== */
#   define _KEY_IDX (uintptr_t)key /* TOUKY: I don't like that. hash should be fixed to handle these custom stuff */
    /* ============================== */
private:
    int HasPrimitiveRenderer(uintptr_t key);

    void AddPrimitiveRenderer(uintptr_t key, std::shared_ptr<class PrimitiveRenderer> renderer);
    void SetPrimitiveRenderer(int index, uintptr_t key, std::shared_ptr<class PrimitiveRenderer> renderer);
    void ReleasePrimitiveRenderer(int index, uintptr_t key);
    void ReleaseAllPrimitiveRenderers(uintptr_t key);
public:
    /* === Primitive renderer stuff === */
    /* Returns the number of primitive renderer set to the given entity */
    template <typename T>
    int HasPrimitiveRenderer(T* key)
    {
        return HasPrimitiveRenderer(_KEY_IDX);
    }
    /* Add a primitive renderer linked to the given entity
     * The primitive is considered as Fire&Forget and
     * will be destroyed at the end of the frame */
    template <typename T>
    void AddPrimitiveRenderer(T* key, std::shared_ptr<class PrimitiveRenderer> renderer)
    {
        AddPrimitiveRenderer(_KEY_IDX, renderer);
    }
    /* Update the primitive renderer linked to the given entity
     * Deletes the old one
     * Will assert if renderer == nullptr */
    template <typename T>
    void SetPrimitiveRenderer(int index, T* key, std::shared_ptr<class PrimitiveRenderer> renderer)
    {
        SetPrimitiveRenderer(index, _KEY_IDX, renderer);
    }
    /* Remove primitive renderer at index set to the given entity */
    template <typename T>
    void ReleasePrimitiveRenderer(int index, T* key)
    {
        ReleasePrimitiveRenderer(index, _KEY_IDX);
    }
    /* Remove all primitive renderer set to the given entity */
    template <typename T>
    void ReleaseAllPrimitiveRenderers(T* key)
    {
        ReleaseAllPrimitiveRenderers(_KEY_IDX);
    }
    /* ============================== */
#   undef _KEY_IDX /* (uintptr_t)key *//* TOUKY: I don't like that. hash should be fixed to handle these custom stuff */
    /* ============================== */

    /* FIXME: this should be deprecated -- it doesn't really match
     * the architecture we want to build */
    void AddTile(class TileSet *tileset, int id, vec3 pos, vec2 scale, float radians);
    void AddTile(class TileSet *tileset, int id, mat4 model);

public:
    void AddLine(vec3 a, vec3 b, vec4 color);
    void AddLine(vec3 a, vec3 b, vec4 color, float duration, uint32_t mask);

    void AddLight(Light *light);
    std::vector<Light *> const &GetLights();

    void resize(ivec2 size);

    void start_frame();
    void end_frame();

    void pre_render(float seconds);
    void render(float seconds);
    void post_render(float seconds);

private:
    void render_primitives();
    void render_tiles();
    void render_lines(float seconds);

    ivec2 m_size, m_wanted_size;

    std::shared_ptr<Renderer> m_renderer;

    //
    // The old SceneData stuff
    //

    /* Mask ID */
    /* TODO: Do a mask class that handles more than 64 slots */
    static uint64_t g_used_id;
    uint64_t m_mask_id = 0;

    // Render buffers: where to render to.
    std::shared_ptr<Framebuffer> m_renderbuffer[4];

    struct postprocess
    {
        std::shared_ptr<Shader> blit_shader, pp_shader;
        std::shared_ptr<VertexBuffer> quad_vbo;
        std::shared_ptr<VertexDeclaration> quad_vdecl;
        ShaderUniform m_buffer_uni[2][3];
        ShaderAttrib blit_pos_attr, pp_pos_attr;
    }
    m_pp;

    /* Sources are shared by all scenes.
     * Renderers are scene-dependent. They get the primitive in the identical
     * slot to render with the given scene.
     * Primitives and renderers will be kept until:
     * - Updated by entity
     * - Marked Fire&Forget
     * - Scene is destroyed */
    std::map<uintptr_t, std::vector<std::shared_ptr<PrimitiveRenderer>>> m_prim_renderers;

    Camera *m_default_cam;
    std::vector<Camera *> m_camera_stack;

    struct line
    {
        vec3 a, b;
        vec4 col;
        float duration;
        uint32_t mask;
        bool wtf1, wtf2; // FIXME: document this
    };

    /* Old line API <P0, P1, COLOR, TIME, MASK> */
    struct line_api
    {
        //float m_duration, m_segment_size;
        //vec4 m_color;
        std::vector<line> m_lines;
        int /*m_mask,*/ m_debug_mask;
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<VertexDeclaration> m_vdecl;
    }
    m_line_api;

    /* The old tiles API */
    struct tile_api
    {
        int m_cam;
        std::vector<Tile> m_tiles;
        std::vector<Tile> m_palettes;
        std::vector<Light *> m_lights;

        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<Shader> m_palette_shader;

        std::shared_ptr<VertexDeclaration> m_vdecl;
        std::vector<std::shared_ptr<VertexBuffer>> m_bufs;
    }
    m_tile_api;
};

} /* namespace lol */

